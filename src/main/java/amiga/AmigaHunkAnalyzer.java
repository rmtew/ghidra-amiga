/* ###
 * IP: GHIDRA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package amiga;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

import fd.FdFunction;
import fd.FdFunctionsInLibs;
import fd.FdLibFunctions;
import fd.FdParser;
import ghidra.app.services.AbstractAnalyzer;
import ghidra.app.services.AnalysisPriority;
import ghidra.app.services.AnalyzerType;
import ghidra.app.util.importer.MessageLog;
import ghidra.framework.Application;
import ghidra.framework.options.Options;
import ghidra.program.flatapi.FlatProgramAPI;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.block.BasicBlockModel;
import ghidra.program.model.block.CodeBlock;
import ghidra.program.model.block.CodeBlockIterator;
import ghidra.program.model.block.CodeBlockReferenceIterator;
import ghidra.program.model.data.ArrayDataType;
import ghidra.program.model.data.ByteDataType;
import ghidra.program.model.data.DataType;
import ghidra.program.model.data.DataTypeComponent;
import ghidra.program.model.data.DataUtilities;
import ghidra.program.model.data.FileDataTypeManager;
import ghidra.program.model.data.FunctionDefinitionDataType;
import ghidra.program.model.data.DataUtilities.ClearDataMode;
import ghidra.program.model.data.PointerDataType;
import ghidra.program.model.data.Pointer;
import ghidra.program.model.data.Structure;
import ghidra.program.model.data.StringDataInstance;
import ghidra.program.model.data.Undefined;
import ghidra.program.model.data.VoidDataType;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.DataIterator;
import ghidra.program.model.listing.ParameterImpl;
import ghidra.program.model.listing.Parameter;
import ghidra.program.model.listing.Program;
import ghidra.program.model.listing.ReturnParameterImpl;
import ghidra.program.model.listing.VariableStorage;
import ghidra.program.model.listing.Function.FunctionUpdateType;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.RefType;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.util.CodeUnitInsertionException;
import ghidra.util.exception.CancelledException;
import ghidra.util.exception.DuplicateNameException;
import ghidra.util.exception.InvalidInputException;
import ghidra.util.task.TaskMonitor;

public class AmigaHunkAnalyzer extends AbstractAnalyzer {
	//private static final int imageBaseOffset = 0x10000;
	private static final String AUTO_DISCOVER_LIBRARIES_OPTION = "Automatically discover referenced APIs";
	private final List<String> filter = new ArrayList<>();
	private Set<String> activeLibraries = Set.of();
	private FdFunctionsInLibs funcsList;
	private boolean autoDiscoverReferencedLibraries = true;
	
	public AmigaHunkAnalyzer() {
		super("Amiga Library Calls", "Analyses calls to system libraries", AnalyzerType.INSTRUCTION_ANALYZER);
		
		filter.add(FdParser.EXEC_LIB);
		filter.add(FdParser.DOS_LIB);
		setPriority(AnalysisPriority.DATA_ANALYSIS.after());
	}

	@Override
	public boolean getDefaultEnablement(Program program) {
		return program.getExecutableFormat().contains("Amiga") && !program.getExecutableFormat().contains("Kickstart");
	}

	@Override
	public boolean canAnalyze(Program program) {
		if(program.getLanguage().getProcessor().toString().equals("68000")) {
			funcsList = new FdFunctionsInLibs();
			return true;
		}
		funcsList = null;
		return false;
	}
	
	@Override
	public void registerOptions(Options options, Program program) {
		if (funcsList == null) {
			return;
		}

		String[] libsList = funcsList.getLibsList(null);
		for (String lib : libsList) {
			boolean defaultValue = filter.contains(lib);
			options.registerOption(lib, defaultValue, null, String.format("Analyze calls from %s", lib));
		}
		options.registerOption(AUTO_DISCOVER_LIBRARIES_OPTION, true, null,
				"Create API tables for known libraries, devices, and resources when their names are proven arguments to an Exec opener.");
	}
	
	@Override
	public void optionsChanged(Options options, Program program) {
		super.optionsChanged(options, program);

		if (funcsList == null) {
			return;
		}
		
		filter.clear();
		autoDiscoverReferencedLibraries = options.getBoolean(AUTO_DISCOVER_LIBRARIES_OPTION, true);
		
		String[] libsList = funcsList.getLibsList(filter);
		for (String lib : libsList) {
			if (options.getBoolean(lib, false)) {
				filter.add(lib);
			}
		}
	}

	@Override
	public boolean added(Program program, AddressSetView set, TaskMonitor monitor, MessageLog log) {
		monitor.setMessage("Creating library functions...");
		for (String warning : funcsList.getLoadWarnings()) {
			log.appendMsg(warning);
		}
		
		Set<String> selectedLibraries = new HashSet<>(filter);
		activeLibraries = Set.copyOf(selectedLibraries);
		FlatProgramAPI fpa = new FlatProgramAPI(program);
		FileDataTypeManager fdm;
		try {
			fdm = fpa.openDataTypeArchive(getModuleDataFile("amiga_ndk39.gdt"), true);
			// Exec is selected by default. Materialise the explicitly selected tables
			// first so discovery can prove an actual Exec vector call, rather than
			// guessing from a vector displacement shared by unrelated APIs.
			for(String lib : funcsList.getLibsList(new ArrayList<>(activeLibraries))) {
				createFunctionsSegment(fpa, fdm, lib, funcsList.getFunctionTableByLib(lib), log);
			}
			if (autoDiscoverReferencedLibraries) {
				Map<Address, String> knownBases = new HashMap<>();
				knownBases.put(program.getAddressFactory().getDefaultAddressSpace().getAddress(4), FdParser.EXEC_LIB);
				propagateKnownBaseCopies(program, knownBases, new HashSet<>());
				for (ApiDiscoveryEvidence evidence : findReferencedApiLibraries(program,
						funcsList.getLibsList(null), knownBases)) {
					if (selectedLibraries.add(evidence.apiKey())) {
						log.appendMsg(String.format("Discovered %s through %s at %s using string at %s",
								evidence.apiKey(), evidence.openerName(), evidence.openerAddress(),
								evidence.stringAddress()));
					}
				}
				activeLibraries = Set.copyOf(selectedLibraries);
				for (String lib : funcsList.getLibsList(new ArrayList<>(activeLibraries))) {
					createFunctionsSegment(fpa, fdm, lib, funcsList.getFunctionTableByLib(lib), log);
				}
			}
		} catch (Exception e) {
			log.appendException(e);
			return false;
		}
		
		monitor.setMessage("Analysing library calls...");
		Set<Address> ambiguousApiBaseStorages = new HashSet<>();
		Map<Address, String> apiBaseStorages = discoverApiBaseStorages(program, ambiguousApiBaseStorages);
		Map<Address, FdFunction> openWrappers = findOpenApiWrappers(program, apiBaseStorages);
		try {
			applyOpenApiWrapperSignatures(program, openWrappers);
		} catch (InvalidInputException | DuplicateNameException e) {
			log.appendException(e);
			return false;
		}
		boolean changed;
		try {
			do {
				changed = false;
				FunctionIterator fiter = program.getFunctionManager().getFunctions(set, true);
				while (fiter.hasNext()) {
					monitor.checkCancelled();
					changed |= resolveApiCallsInFunction(program, fiter.next(), apiBaseStorages,
							ambiguousApiBaseStorages, openWrappers, monitor);
				}
			} while (changed);
		} catch (CancelledException e) {
			log.appendException(e);
			return false;
		}
		Map<Address, ForwardingApiWrapper> forwardingWrappers = findForwardingApiWrappers(program,
				apiBaseStorages);
		try {
			applyForwardingApiWrapperSignatures(program, forwardingWrappers);
			List<AmigaAbiModel.DeviceDispatch> deviceDispatches = AmigaAbiModel.loadDeviceDispatches();
			Map<Integer, Address> deviceAbiTargets = createDeviceAbiTargets(fpa, fdm, deviceDispatches, log);
			Map<Address, AmigaAbiModel.DeviceDispatch> deviceWrappers = findDeviceDispatchWrappers(program, fdm,
					deviceDispatches);
			applyDeviceDispatchWrapperSignatures(program, fdm, deviceWrappers);
			installDeviceDispatchCallOverrides(program, deviceWrappers, deviceAbiTargets);
			propagateOpenDeviceRequestTypes(program, fdm, forwardingWrappers,
					AmigaAbiModel.loadDeviceRequestTypes(), monitor);
			propagateTypedStackParameters(program, monitor);
			if (specializePointerParametersByStructureAccess(program, fdm, monitor)) {
				propagateTypedStackParameters(program, monitor);
			}
			specializePrivateDeviceDispatchWrappers(program, deviceWrappers, monitor);
			propagateTypedPointerArgumentsToStorage(program, monitor);
			materializeApiBaseStorages(program, fdm, apiBaseStorages, ambiguousApiBaseStorages);
		} catch (java.io.IOException | InvalidInputException | DuplicateNameException | CodeUnitInsertionException |
				CancelledException e) {
			log.appendException(e);
			return false;
		}

		return true;
	}

	private static java.io.File getModuleDataFile(String name) throws java.io.IOException {
		try {
			return Application.getModuleDataFile(name).getFile(false);
		}
		catch (java.io.IOException e) {
			java.io.File sourceData = new java.io.File("data", name);
			if (sourceData.isFile()) {
				return sourceData;
			}
			throw e;
		}
	}

	/**
	 * Finds API names only when a known defined string is proven to reach the name
	 * argument of a direct Exec opener. An executable reference alone is not
	 * enough: it could be a diagnostic, filename, or display string.
	 */
	Set<ApiDiscoveryEvidence> findReferencedApiLibraries(Program program, String[] knownLibraries,
			Map<Address, String> knownBases) {
		Set<String> known = new HashSet<>();
		for (String library : knownLibraries) {
			known.add(library.toLowerCase(Locale.ROOT));
		}
		Set<ApiDiscoveryEvidence> discovered = new LinkedHashSet<>();
		InstructionIterator instructions = program.getListing().getInstructions(true);
		while (instructions.hasNext()) {
			Instruction call = instructions.next();
			FdFunction opener = getDirectOpenApiCall(program, call, knownBases);
			ApiNameArgument argument = opener == null ? null : getApiNameArgument(program, call, opener);
			if (argument != null && known.contains(argument.apiKey())) {
				discovered.add(new ApiDiscoveryEvidence(argument.apiKey(), argument.stringAddress(),
						call.getAddress(), opener.getName(false)));
			}
		}
		return discovered;
	}

	/** Proven direct acquisition of a bundled Amiga API definition. */
	static record ApiDiscoveryEvidence(String apiKey, Address stringAddress, Address openerAddress,
			String openerName) {
	}

	private record ApiNameArgument(String apiKey, Address stringAddress) {
	}

	static String toApiBaseName(String externalName) {
		if (externalName == null) {
			return null;
		}
		String normalized = externalName.strip().toLowerCase(Locale.ROOT);
		if (!(normalized.endsWith(".library") || normalized.endsWith(".device") || normalized.endsWith(".resource"))) {
			return null;
		}
		return normalized.replace('.', '_');
	}

	static DataType getAmigaDataType(String type, FileDataTypeManager fdm) {
		if (type == null || type.isBlank() || type.equalsIgnoreCase("VOID")) {
			return VoidDataType.dataType;
		}
		DataType dataType = PointerDataType.dataType;
		type = type.trim().replaceAll("\\s+", " ");
		type = type.replace("struct ", "");
		type = type.replace("const ", "");
		type = type.replace("CONST ", "");
		type = type.replace("volatile ", "");
		type = type.replace("VOLATILE ", "");
		if(type.contains("("))
			return new PointerDataType(new FunctionDefinitionDataType("FUNC")); // TODO: correct function pointer type
		for(var word : type.split("\\s+")) {
			if(word.equals("*")) {
				dataType = new PointerDataType(dataType);
			} else if(word.equals("**")) {
				dataType = new PointerDataType(new PointerDataType(dataType));
			} else {
				var list = new ArrayList<DataType>();
				fdm.findDataTypes(word, list);
				if (!list.isEmpty()) {
					dataType = list.get(0);
				}
			}
		}
		return dataType;
	}

	private static void createFunctionsSegment(FlatProgramAPI fpa, FileDataTypeManager fdm, String lib, FdLibFunctions funcs, MessageLog log) throws InvalidInputException, DuplicateNameException, CodeUnitInsertionException {
		if ((null == funcs) || (fpa.getMemoryBlock(lib) != null)) {
			return;
		}
		FdFunction[] funcArr = funcs.getFunctions();
		long segAlign = Math.max(2, 0x1000);
		long segSize = 6 * Math.max(5, 7);  // Library 5+, Device 7+
		for (FdFunction func : funcArr) {
			segSize = Math.max(segSize, Math.abs(func.getBias()) + 6);
		}
		segSize = ((segSize + (segAlign - 1)) / segAlign) * segAlign;
		Address segAddr = fpa.toAddr(AmigaHunkLoader.getImageBase(0));
		for (MemoryBlock memBlock : fpa.getMemoryBlocks()) {
			if (memBlock.contains(segAddr) || memBlock.contains(segAddr.add(segSize - 1)) || (
				(segAddr.getOffset() <= memBlock.getStart().getOffset()) &&
				(memBlock.getEnd().getOffset() <= segAddr.add(segSize - 1).getOffset()))) {
				segAddr = memBlock.getEnd().add(1);
				long segRem = segAddr.getOffset() % segAlign;
				if (segRem > 0) {
					segAddr = segAddr.add(segAlign - segRem);
				}
			}
		}
		
		AmigaUtils.createSegment(null, fpa, lib, segAddr.getOffset(), segSize, true, true, log);
		
		for (FdFunction func : funcArr) {
			Address funcAddress = segAddr.add(Math.abs(func.getBias()));
			AmigaUtils.setFunction(fpa, funcAddress, func.getName(true).replace(FdFunction.LIB_SPLITTER, "_"), log);
			Function function = fpa.getFunctionAt(funcAddress);
			function.setCustomVariableStorage(true);

			List<ParameterImpl> params = new ArrayList<>();
			Program program = fpa.getCurrentProgram();
			for (var arg : func.getArgs()) {
				var dataType = getAmigaDataType(arg.type, fdm);
				Register register = program.getRegister(arg.reg);
				if (register == null) {
					log.appendMsg(String.format("Skipping parameter %s for %s: unknown register '%s'",
							arg.name, func.getName(true), arg.reg));
					continue;
				}
				try {
					params.add(new ParameterImpl(arg.name, dataType, register, program));
				}
				catch (InvalidInputException exception) {
					log.appendMsg(String.format("Skipping parameter %s for %s: %s",
							arg.name, func.getName(true), exception.getMessage()));
				}
			}

			var returnType = getAmigaDataType(func.getReturnType(), fdm);
			var returnValue = isVoidDataType(returnType)
					? new ReturnParameterImpl(VoidDataType.dataType, VariableStorage.VOID_STORAGE, program)
					: new ReturnParameterImpl(returnType, program.getRegister("D0"), program);
			function.updateFunction(null, returnValue, FunctionUpdateType.CUSTOM_STORAGE, true, SourceType.ANALYSIS, params.toArray(ParameterImpl[]::new));
			DataUtilities.createData(program, funcAddress, new ArrayDataType(ByteDataType.dataType, 6, -1), -1, false, ClearDataMode.CLEAR_ALL_UNDEFINED_CONFLICT_DATA);
		}
	}

	private static Map<Integer, Address> createDeviceAbiTargets(FlatProgramAPI fpa, FileDataTypeManager fdm,
			List<AmigaAbiModel.DeviceDispatch> dispatches, MessageLog log)
			throws InvalidInputException, DuplicateNameException, CodeUnitInsertionException {
		Map<Integer, Address> targets = new HashMap<>();
		MemoryBlock block = fpa.getMemoryBlock("device_abi");
		if (block == null) {
			Address address = fpa.toAddr(AmigaHunkLoader.getImageBase(0));
			for (MemoryBlock existing : fpa.getMemoryBlocks()) {
				if (existing.contains(address)) {
					address = existing.getEnd().add(0x1000 - (existing.getEnd().getOffset() % 0x1000));
				}
			}
			AmigaUtils.createSegment(null, fpa, "device_abi", address.getOffset(), 0x1000, true, true, log);
			block = fpa.getMemoryBlock("device_abi");
		}
		Program program = fpa.getCurrentProgram();
		for (AmigaAbiModel.DeviceDispatch dispatch : dispatches) {
			Address target = block.getStart().add(Math.abs(dispatch.vector));
			AmigaUtils.setFunction(fpa, target, "device_abi_" + dispatch.name, log);
			Function function = fpa.getFunctionAt(target);
			List<ParameterImpl> parameters = new ArrayList<>();
			for (AmigaAbiModel.Parameter parameter : dispatch.parameters) {
				parameters.add(new ParameterImpl(parameter.name, getAmigaDataType(parameter.type, fdm),
						program.getRegister(parameter.register), program));
			}
			function.updateFunction(null, new ReturnParameterImpl(VoidDataType.dataType, VariableStorage.VOID_STORAGE, program),
					FunctionUpdateType.CUSTOM_STORAGE, true, SourceType.ANALYSIS, parameters.toArray(ParameterImpl[]::new));
			targets.put(dispatch.vector, target);
		}
		return targets;
	}

	private static void installDeviceDispatchCallOverrides(Program program,
			Map<Address, AmigaAbiModel.DeviceDispatch> wrappers, Map<Integer, Address> targets) {
		for (Map.Entry<Address, AmigaAbiModel.DeviceDispatch> entry : wrappers.entrySet()) {
			Function wrapper = program.getFunctionManager().getFunctionAt(entry.getKey());
			Address target = targets.get(entry.getValue().vector);
			if (wrapper == null || target == null) {
				continue;
			}
			InstructionIterator instructions = program.getListing().getInstructions(wrapper.getBody(), true);
			while (instructions.hasNext()) {
				Instruction instruction = instructions.next();
				if (Integer.valueOf(entry.getValue().vector).equals(getA6VectorBias(instruction))) {
					instruction.addOperandReference(0, target, RefType.CALL_OVERRIDE_UNCONDITIONAL, SourceType.ANALYSIS);
				}
			}
		}
	}

	/**
	 * Tracks statically visible Library bases.  ExecBase originates at address 4;
	 * executable copies of that value are followed through global storage.  A
	 * small forwarding wrapper around Exec OpenLibrary/OldOpenLibrary/OpenResource
	 * is then enough to associate its returned D0 with the caller's global base.
	 */
	private Map<Address, String> discoverApiBaseStorages(Program program, Set<Address> ambiguousStorages) {
		Map<Address, String> bases = new HashMap<>();
		bases.put(program.getAddressFactory().getDefaultAddressSpace().getAddress(4), FdParser.EXEC_LIB);
		propagateKnownBaseCopies(program, bases, ambiguousStorages);

		Map<Address, FdFunction> openWrappers = findOpenApiWrappers(program, bases);
		InstructionIterator instructions = program.getListing().getInstructions(true);
		while (instructions.hasNext()) {
			Instruction instruction = instructions.next();
			Address target = getDirectCallTarget(instruction);
			FdFunction opener = target == null ? null : openWrappers.get(target);
			if (opener == null) {
				opener = getDirectOpenApiCall(program, instruction, bases);
			}
			if (opener == null) {
				continue;
			}
			if (!isApiBaseOpener(opener)) {
				continue;
			}
			ApiNameArgument argument = getApiNameArgument(program, instruction, opener);
			String library = argument == null ? null : argument.apiKey();
			String returnRegister = getApiOpenerReturnRegister(program, opener);
			Address resultStorage = getReturnedRegisterStorage(program, instruction, returnRegister);
			if (library != null && resultStorage != null && activeLibraries.contains(library)) {
				recordApiBaseStorage(bases, ambiguousStorages, resultStorage, library);
			}
		}
		inferApiBaseSlots(program, bases, ambiguousStorages, openWrappers);
		return bases;
	}

	private static void propagateKnownBaseCopies(Program program, Map<Address, String> bases,
			Set<Address> ambiguousStorages) {
		boolean changed;
		do {
			changed = false;
			FunctionIterator functions = program.getFunctionManager().getFunctions(true);
			while (functions.hasNext()) {
				Function function = functions.next();
				String a6Base = null;
				InstructionIterator functionInstructions = program.getListing().getInstructions(function.getBody(), true);
				while (functionInstructions.hasNext()) {
					Instruction instruction = functionInstructions.next();
					if (writesRegister(instruction, "A6")) {
						Address source = getMemoryReference(instruction, true);
						a6Base = source == null ? null : bases.get(source);
					}
					if (a6Base != null && hasSourceRegister(instruction, "A6")) {
						Address destination = getMemoryReference(instruction, false);
						if (destination != null) {
							changed |= recordApiBaseStorage(bases, ambiguousStorages, destination, a6Base);
						}
					}
				}
			}
			InstructionIterator instructions = program.getListing().getInstructions(true);
			while (instructions.hasNext()) {
				Instruction instruction = instructions.next();
				Address source = getMemoryReference(instruction, true);
				Address destination = getMemoryReference(instruction, false);
				String library = source == null ? null : bases.get(source);
				if (library != null && destination != null) {
					changed |= recordApiBaseStorage(bases, ambiguousStorages, destination, library);
				}
			}
		} while (changed);
	}

	private static boolean recordApiBaseStorage(Map<Address, String> bases, Set<Address> ambiguousStorages,
			Address storage, String library) {
		if (ambiguousStorages.contains(storage)) {
			return false;
		}
		String existing = bases.get(storage);
		if (existing == null) {
			bases.put(storage, library);
			return true;
		}
		if (existing.equals(library)) {
			return false;
		}
		bases.remove(storage);
		ambiguousStorages.add(storage);
		return true;
	}

	/**
	 * Makes a proven API-base role visible in the program, rather than retaining
	 * it only in the resolver's private state.  This is deliberately limited to
	 * library/device/resource bases: their identity follows from their creation
	 * path, while ordinary object pointers require stronger, object-specific
	 * evidence before they can be typed globally.
	 */
	private void materializeApiBaseStorages(Program program, FileDataTypeManager fdm, Map<Address, String> bases,
			Set<Address> ambiguousStorages)
			throws CodeUnitInsertionException, DuplicateNameException, InvalidInputException {
		Address sysBaseAddress = program.getAddressFactory().getDefaultAddressSpace().getAddress(4);
		for (Map.Entry<Address, String> entry : bases.entrySet()) {
			Address storage = entry.getKey();
			if (ambiguousStorages.contains(storage) || program.getMemory().getBlock(storage) == null) {
				continue;
			}
			if (storage.equals(sysBaseAddress)) {
				// Address $4 is the system ABI origin of the Exec base, not
				// application-owned storage. Its conventional C name is SysBase.
				removeObsoleteSysBaseAnalysisLabel(program, storage);
				Data existing = program.getListing().getDefinedDataAt(storage);
				if (existing == null || existing.getDataType().getName().startsWith("undefined")) {
					DataUtilities.createData(program, storage,
							new PointerDataType(getAmigaDataType("ExecBase", fdm)), -1, false,
							ClearDataMode.CLEAR_ALL_UNDEFINED_CONFLICT_DATA);
				}
				AmigaUtils.applyAnalysisGlobalLabel(program, storage, "SysBase");
				continue;
			}
			Data existing = program.getListing().getDefinedDataAt(storage);
			if (program.getListing().getInstructionAt(storage) != null) {
				continue;
			}
			if (existing == null || existing.getDataType().getName().startsWith("undefined")) {
				DataUtilities.createData(program, storage, PointerDataType.dataType, -1,
						false, ClearDataMode.CLEAR_ALL_UNDEFINED_CONFLICT_DATA);
			}
			AmigaUtils.applyAnalysisGlobalLabel(program, storage, getLibraryBaseLabel(entry.getValue()));
		}
	}

	private static void removeObsoleteSysBaseAnalysisLabel(Program program, Address sysBaseAddress) {
		for (Symbol symbol : program.getSymbolTable().getSymbols(sysBaseAddress)) {
			if (symbol.getSource() == SourceType.ANALYSIS && "g_ExecLibraryBase".equals(symbol.getName())) {
				symbol.delete();
			}
		}
	}

	private static String getLibraryBaseLabel(String library) {
		StringBuilder name = new StringBuilder("g_");
		for (String part : library.split("_")) {
			if (!part.isEmpty()) {
				name.append(Character.toUpperCase(part.charAt(0))).append(part.substring(1));
			}
		}
		return name.append("Base").toString();
	}

	private Map<Address, AmigaAbiModel.DeviceDispatch> findDeviceDispatchWrappers(Program program,
			FileDataTypeManager fdm, List<AmigaAbiModel.DeviceDispatch> dispatches) {
		Map<Address, AmigaAbiModel.DeviceDispatch> wrappers = new HashMap<>();
		for (AmigaAbiModel.DeviceDispatch dispatch : dispatches) {
			Integer fieldOffset = getStructureFieldOffset(dispatch.structureType, dispatch.structureField, fdm);
			if (fieldOffset == null) {
				continue;
			}
			FunctionIterator functions = program.getFunctionManager().getFunctions(true);
			while (functions.hasNext()) {
				Function wrapper = functions.next();
				InstructionIterator instructions = program.getListing().getInstructions(wrapper.getBody(), true);
				while (instructions.hasNext()) {
					Instruction vectorCall = instructions.next();
					if (!Integer.valueOf(dispatch.vector).equals(getA6VectorBias(vectorCall)) ||
							!isDeviceDispatchWrapper(program, wrapper, vectorCall, dispatch, fieldOffset)) {
						continue;
					}
					wrappers.put(wrapper.getEntryPoint(), dispatch);
				}
			}
		}
		return wrappers;
	}

	private static Integer getStructureFieldOffset(String typeName, String fieldName, FileDataTypeManager fdm) {
		List<DataType> candidates = new ArrayList<>();
		fdm.findDataTypes(typeName, candidates);
		for (DataType candidate : candidates) {
			if (!(candidate instanceof Structure structure)) {
				continue;
			}
			for (DataTypeComponent component : structure.getComponents()) {
				if (fieldName.equals(component.getFieldName())) {
					return component.getOffset();
				}
			}
		}
		return null;
	}

	private static boolean isDeviceDispatchWrapper(Program program, Function wrapper, Instruction vectorCall,
			AmigaAbiModel.DeviceDispatch dispatch, int fieldOffset) {
		for (AmigaAbiModel.Parameter parameter : dispatch.parameters) {
			Register register = program.getRegister(parameter.register);
			if (register == null || !findStackArgumentSource(vectorCall, wrapper, register)) {
				return false;
			}
		}
		Instruction baseLoad = findPreviousRegisterWrite(wrapper, vectorCall, "A6");
		String receiverRegister = dispatch.parameters.get(0).register;
		if (baseLoad == null || !hasSourceRegister(baseLoad, receiverRegister) ||
				!hasOperandDisplacement(baseLoad, fieldOffset)) {
			return false;
		}
		return isStraightForwarder(wrapper, vectorCall,
				dispatch.returnRegister.isBlank() ? null : dispatch.returnRegister);
	}

	private static Instruction findPreviousRegisterWrite(Function function, Instruction start, String registerName) {
		for (Instruction instruction = start.getPrevious(); instruction != null &&
				function.getBody().contains(instruction.getAddress()); instruction = instruction.getPrevious()) {
			if (writesRegister(instruction, registerName)) {
				return instruction;
			}
		}
		return null;
	}

	private static boolean hasOperandDisplacement(Instruction instruction, int expectedOffset) {
		for (Object object : instruction.getOpObjects(0)) {
			if (object instanceof Scalar scalar && scalar.getSignedValue() == expectedOffset) {
				return true;
			}
		}
		return false;
	}

	private static boolean isStraightForwarder(Function wrapper, Instruction vectorCall, String returnRegister) {
		boolean sawReturn = false;
		InstructionIterator instructions = wrapper.getProgram().getListing().getInstructions(wrapper.getBody(), true);
		while (instructions.hasNext()) {
			Instruction instruction = instructions.next();
			if (instruction.getAddress().equals(vectorCall.getAddress())) {
				continue;
			}
			if (instruction.getFlowType().isCall() || instruction.getFlowType().isJump()) {
				return false;
			}
			if (returnRegister != null && instruction.getAddress().compareTo(vectorCall.getAddress()) > 0 &&
					writesRegister(instruction, returnRegister)) {
				return false;
			}
			if (instruction.getAddress().compareTo(vectorCall.getAddress()) > 0 &&
					instruction.getMnemonicString().equals("rts")) {
				sawReturn = true;
			}
		}
		return sawReturn;
	}

	private void applyDeviceDispatchWrapperSignatures(Program program, FileDataTypeManager fdm,
			Map<Address, AmigaAbiModel.DeviceDispatch> wrappers) throws InvalidInputException, DuplicateNameException {
		for (Map.Entry<Address, AmigaAbiModel.DeviceDispatch> entry : wrappers.entrySet()) {
			Function wrapper = program.getFunctionManager().getFunctionAt(entry.getKey());
			if (wrapper == null || !canApplyAnalysisSignature(wrapper)) {
				continue;
			}
			List<ParameterImpl> parameters = new ArrayList<>();
			for (AmigaAbiModel.Parameter parameter : entry.getValue().parameters) {
				parameters.add(new ParameterImpl(parameter.name, getAmigaDataType(parameter.type, fdm), program,
						SourceType.ANALYSIS));
			}
			DataType returnType = getAmigaDataType(entry.getValue().returnType, fdm);
			if (!isVoidDataType(returnType) && entry.getValue().returnRegister.isBlank()) {
				continue;
			}
			ReturnParameterImpl returnValue = isVoidDataType(returnType)
					? new ReturnParameterImpl(VoidDataType.dataType, VariableStorage.VOID_STORAGE, program)
					: new ReturnParameterImpl(returnType, program.getRegister(entry.getValue().returnRegister), program);
			wrapper.updateFunction(null, returnValue, FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS, true,
					SourceType.ANALYSIS, parameters.toArray(ParameterImpl[]::new));
		}
	}

	/**
	 * Propagates an already-persisted stack parameter type across direct C calls.
	 * A proposal is accepted only when the pushed value can be traced back to a
	 * caller stack slot and all discovered evidence for that slot agrees.
	 */
	private void propagateTypedStackParameters(Program program, TaskMonitor monitor)
			throws CancelledException, InvalidInputException, DuplicateNameException {
		boolean changed;
		do {
			changed = false;
			Map<Function, Map<Integer, DataType>> proposals = new HashMap<>();
			Map<Function, Set<Integer>> conflicts = new HashMap<>();
			FunctionIterator callers = program.getFunctionManager().getFunctions(true);
			while (callers.hasNext()) {
				monitor.checkCancelled();
				Function caller = callers.next();
				InstructionIterator instructions = program.getListing().getInstructions(caller.getBody(), true);
				while (instructions.hasNext()) {
					Instruction call = instructions.next();
					Function callee = getDirectCallee(program, call);
					if (callee == null || callee.getSignatureSource() == SourceType.DEFAULT) {
						continue;
					}
					for (Parameter parameter : callee.getParameters()) {
						if (!parameter.getVariableStorage().isStackStorage() || !isConcreteDataType(parameter.getDataType())) {
							continue;
						}
						Instruction push = findPushedStackArgument(caller, call, parameter.getStackOffset());
						Integer callerStackOffset = push == null ? null : findCallerStackOffset(caller, push);
						if (callerStackOffset != null) {
							recordTypeProposal(proposals, conflicts, caller, callerStackOffset, parameter.getDataType());
						}
					}
				}
			}
			for (Map.Entry<Function, Map<Integer, DataType>> entry : proposals.entrySet()) {
				monitor.checkCancelled();
				changed |= applyInferredStackParameters(program, entry.getKey(), entry.getValue(),
						conflicts.getOrDefault(entry.getKey(), Set.of()));
			}
		} while (changed);
	}

	private static Function getDirectCallee(Program program, Instruction call) {
		Address target = getDirectCallTarget(call);
		return target == null ? null : program.getFunctionManager().getFunctionAt(target);
	}

	private static boolean isConcreteDataType(DataType dataType) {
		return dataType != null && !isVoidDataType(dataType) && !Undefined.isUndefined(dataType);
	}

	/**
	 * Ghidra may return a program-managed instance of the built-in void type;
	 * void is a semantic type property, not an object-identity property.
	 */
	private static boolean isVoidDataType(DataType dataType) {
		return dataType instanceof VoidDataType;
	}

	private static void recordTypeProposal(Map<Function, Map<Integer, DataType>> proposals,
			Set<Integer> conflicts, Function function, int stackOffset, DataType dataType) {
		if (conflicts.contains(stackOffset)) {
			return;
		}
		Map<Integer, DataType> functionProposals = proposals.computeIfAbsent(function, ignored -> new HashMap<>());
		DataType existing = functionProposals.get(stackOffset);
		if (existing == null || existing.isEquivalent(dataType)) {
			functionProposals.put(stackOffset, dataType);
			return;
		}
		functionProposals.remove(stackOffset);
		conflicts.add(stackOffset);
	}

	private static void recordTypeProposal(Map<Function, Map<Integer, DataType>> proposals,
			Map<Function, Set<Integer>> conflicts, Function function, int stackOffset, DataType dataType) {
		recordTypeProposal(proposals, conflicts.computeIfAbsent(function, ignored -> new HashSet<>()), function,
				stackOffset, dataType);
	}

	private static void recordTypeProposal(Map<Address, DataType> proposals, Set<Address> conflicts,
			Address storage, DataType dataType) {
		if (conflicts.contains(storage)) {
			return;
		}
		DataType existing = proposals.get(storage);
		if (existing == null || existing.isEquivalent(dataType)) {
			proposals.put(storage, dataType);
			return;
		}
		proposals.remove(storage);
		conflicts.add(storage);
	}

	private static Instruction findPushedStackArgument(Function caller, Instruction call, int parameterStackOffset) {
		if (parameterStackOffset < 4) {
			return null;
		}
		int currentStackOffset = 4;
		for (Instruction instruction = call.getPrevious(); instruction != null &&
				caller.getBody().contains(instruction.getAddress()); instruction = instruction.getPrevious()) {
			int pushedSize = getStackPushSize(instruction);
			if (pushedSize == 0) {
				return null;
			}
			if (parameterStackOffset >= currentStackOffset && parameterStackOffset < currentStackOffset + pushedSize) {
				return instruction;
			}
			currentStackOffset += pushedSize;
		}
		return null;
	}

	private static int getStackPushSize(Instruction instruction) {
		String text = instruction.toString().replace(" ", "").toUpperCase(Locale.ROOT);
		if (!text.endsWith(",-(SP)")) {
			return 0;
		}
		if (text.startsWith("MOVEM.L")) {
			int start = text.indexOf('{');
			int end = text.indexOf('}');
			return start >= 0 && end > start ? 4 * countMovemRegisters(text.substring(start + 1, end)) : 0;
		}
		return getOperandSize(instruction);
	}

	private static int countMovemRegisters(String registers) {
		int count = 0;
		for (String register : registers.split(",")) {
			register = register.strip();
			if (register.isEmpty()) {
				continue;
			}
			int range = register.indexOf('-');
			if (range > 0 && register.length() > range + 1) {
				count += Math.abs(register.charAt(range - 1) - register.charAt(register.length() - 1)) + 1;
			}
			else {
				count++;
			}
		}
		return count;
	}

	private static Integer findCallerStackOffset(Function caller, Instruction push) {
		Object[] source = push.getOpObjects(0);
		for (Object object : source) {
			if (object instanceof Register register) {
				Instruction definition = findPreviousRegisterWrite(caller, push, register.getName());
				return definition == null ? null : getExternalStackOffset(caller, definition);
			}
		}
		return getExternalStackOffset(caller, push);
	}

	private static Integer getExternalStackOffset(Function function, Instruction instruction) {
		Integer displacement = getStackSourceDisplacement(instruction);
		if (displacement == null) {
			return null;
		}
		int externalOffset = displacement - getStackReservationBefore(function, instruction);
		return externalOffset >= 4 ? externalOffset : null;
	}

	private static Integer getStackSourceDisplacement(Instruction instruction) {
		boolean hasStackPointer = false;
		Integer displacement = null;
		for (Object object : instruction.getOpObjects(0)) {
			if (object instanceof Register register && "SP".equals(register.getName())) {
				hasStackPointer = true;
			}
			else if (object instanceof Scalar scalar) {
				displacement = (int) scalar.getSignedValue();
			}
		}
		return hasStackPointer ? displacement : null;
	}

	private static int getStackReservationBefore(Function function, Instruction target) {
		int reservation = 0;
		InstructionIterator instructions = function.getProgram().getListing().getInstructions(function.getBody(), true);
		while (instructions.hasNext()) {
			Instruction instruction = instructions.next();
			if (instruction.getAddress().equals(target.getAddress())) {
				break;
			}
			reservation += getStackReservationChange(instruction);
		}
		return reservation;
	}

	private static int getStackReservationChange(Instruction instruction) {
		int pushed = getStackPushSize(instruction);
		if (pushed != 0) {
			return pushed;
		}
		String text = instruction.toString().replace(" ", "").toUpperCase(Locale.ROOT);
		if (text.startsWith("MOVEM.L(SP)+,")) {
			int start = text.indexOf('{');
			int end = text.indexOf('}');
			return start >= 0 && end > start ? -4 * countMovemRegisters(text.substring(start + 1, end)) : 0;
		}
		if ((text.startsWith("ADDQ") || text.startsWith("SUBQ")) && text.endsWith(",SP")) {
			for (Object object : instruction.getOpObjects(0)) {
				if (object instanceof Scalar scalar) {
					return text.startsWith("ADDQ") ? -(int) scalar.getUnsignedValue() : (int) scalar.getUnsignedValue();
				}
			}
		}
		return 0;
	}

	private static int getOperandSize(Instruction instruction) {
		String mnemonic = instruction.getMnemonicString().toLowerCase(Locale.ROOT);
		if (mnemonic.endsWith(".l") || mnemonic.equals("pea")) {
			return 4;
		}
		return 2;
	}

	private static boolean applyInferredStackParameters(Program program, Function function,
			Map<Integer, DataType> proposals, Set<Integer> conflicts)
			throws InvalidInputException, DuplicateNameException {
		if (!canApplyAnalysisSignature(function) || proposals.isEmpty()) {
			return false;
		}
		Map<Integer, Integer> slots = discoverStackParameterSlots(function);
		if (slots.isEmpty() || !slots.keySet().containsAll(proposals.keySet())) {
			return false;
		}
		List<ParameterImpl> parameters = new ArrayList<>();
		for (Map.Entry<Integer, Integer> slot : slots.entrySet()) {
			DataType type = conflicts.contains(slot.getKey()) ? Undefined.getUndefinedDataType(slot.getValue()) :
					proposals.getOrDefault(slot.getKey(), Undefined.getUndefinedDataType(slot.getValue()));
			type = retainMoreSpecificPointerType(function, slot.getKey(), type);
			parameters.add(new ParameterImpl("param_" + (parameters.size() + 1), type, program, SourceType.ANALYSIS));
		}
		if (hasEquivalentStackSignature(function, parameters)) {
			return false;
		}
		function.updateFunction(null, function.getReturn(), FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS, true,
				SourceType.ANALYSIS, parameters.toArray(ParameterImpl[]::new));
		return true;
	}

	private static DataType retainMoreSpecificPointerType(Function function, int stackOffset, DataType proposedType) {
		for (Parameter parameter : function.getParameters()) {
			if (parameter.getVariableStorage().isStackStorage() && parameter.getStackOffset() == stackOffset &&
					isPointerSpecialization(parameter.getDataType(), proposedType)) {
				return parameter.getDataType();
			}
		}
		return proposedType;
	}

	private static boolean isPointerSpecialization(DataType candidateType, DataType baseType) {
		if (!(candidateType instanceof Pointer candidatePointer) || !(baseType instanceof Pointer basePointer) ||
				!(candidatePointer.getDataType() instanceof Structure candidate) ||
				!(basePointer.getDataType() instanceof Structure base)) {
			return false;
		}
		return candidate.isEquivalent(base) || embedsBaseAtZero(candidate, base);
	}

	private static void specializePrivateDeviceDispatchWrappers(Program program,
			Map<Address, AmigaAbiModel.DeviceDispatch> wrappers, TaskMonitor monitor)
			throws CancelledException, InvalidInputException, DuplicateNameException {
		for (Address entry : wrappers.keySet()) {
			monitor.checkCancelled();
			Function wrapper = program.getFunctionManager().getFunctionAt(entry);
			if (wrapper == null || wrapper.getParameterCount() != 1 || !canApplyAnalysisSignature(wrapper) ||
					isAddressTaken(program, entry)) {
				continue;
			}
			DataType agreedType = null;
			ReferenceIterator references = program.getReferenceManager().getReferencesTo(entry);
			while (references.hasNext()) {
				Reference reference = references.next();
				if (!reference.getReferenceType().isCall()) {
					continue;
				}
				Instruction call = program.getListing().getInstructionAt(reference.getFromAddress());
				Function caller = call == null ? null : program.getFunctionManager().getFunctionContaining(call.getAddress());
				Instruction push = caller == null ? null : findPushedStackArgument(caller, call, 4);
				Integer offset = push == null ? null : findCallerStackOffset(caller, push);
				DataType type = offset == null ? null : getStackParameterType(caller, offset);
				if (type == null || !isPointerSpecialization(type, wrapper.getParameter(0).getDataType())) {
					agreedType = null;
					break;
				}
				if (agreedType != null && !agreedType.isEquivalent(type)) {
					agreedType = null;
					break;
				}
				agreedType = type;
			}
			if (agreedType != null) {
				replaceAnalysisParameterType(program, wrapper, 0, agreedType);
			}
		}
	}

	private static DataType getStackParameterType(Function function, int stackOffset) {
		for (Parameter parameter : function.getParameters()) {
			if (parameter.getVariableStorage().isStackStorage() && parameter.getStackOffset() == stackOffset) {
				return parameter.getDataType();
			}
		}
		return null;
	}

	private static boolean isAddressTaken(Program program, Address entry) {
		ReferenceIterator references = program.getReferenceManager().getReferencesTo(entry);
		while (references.hasNext()) {
			if (!references.next().getReferenceType().isCall()) {
				return true;
			}
		}
		return false;
	}

	private static Map<Integer, Integer> discoverStackParameterSlots(Function function) {
		Map<Integer, Integer> slots = new java.util.TreeMap<>();
		InstructionIterator instructions = function.getProgram().getListing().getInstructions(function.getBody(), true);
		while (instructions.hasNext()) {
			Instruction instruction = instructions.next();
			Integer offset = getExternalStackOffset(function, instruction);
			if (offset != null) {
				slots.merge(offset, getOperandSize(instruction), Math::max);
			}
		}
		return slots;
	}

	private static boolean hasEquivalentStackSignature(Function function, List<ParameterImpl> parameters) {
		if (function.getParameterCount() != parameters.size()) {
			return false;
		}
		for (int index = 0; index < parameters.size(); index++) {
			if (!function.getParameter(index).getDataType().isEquivalent(parameters.get(index).getDataType())) {
				return false;
			}
		}
		return true;
	}

	/**
	 * Refines a pointer-to-base parameter only when its own field accesses prove a
	 * unique structure in the NDK archive.  This models the usual Amiga idiom in
	 * which an API accepts an IORequest while device code operates on a concrete
	 * request structure that embeds IORequest at offset zero.
	 */
	private static boolean specializePointerParametersByStructureAccess(Program program, FileDataTypeManager fdm,
			TaskMonitor monitor) throws CancelledException, InvalidInputException, DuplicateNameException {
		boolean changed = false;
		FunctionIterator functions = program.getFunctionManager().getFunctions(true);
		while (functions.hasNext()) {
			monitor.checkCancelled();
			Function function = functions.next();
			if (!canApplyAnalysisSignature(function)) {
				continue;
			}
			for (Parameter parameter : function.getParameters()) {
				DataType refinedType = findUniqueStructureSpecialization(function, parameter, fdm);
				if (refinedType != null && !parameter.getDataType().isEquivalent(refinedType)) {
					changed |= replaceAnalysisParameterType(program, function, parameter.getOrdinal(), refinedType);
				}
			}
		}
		return changed;
	}

	private static DataType findUniqueStructureSpecialization(Function function, Parameter parameter,
			FileDataTypeManager fdm) {
		if (!(parameter.getDataType() instanceof Pointer pointer) ||
				!(pointer.getDataType() instanceof Structure baseStructure) ||
				!parameter.getVariableStorage().isStackStorage()) {
			return null;
		}
		List<StructureAccess> accesses = findStructureAccesses(function, parameter.getStackOffset());
		if (accesses.size() < 2) {
			return null;
		}
		Set<DataType> candidates = new HashSet<>();
		var dataTypes = fdm.getAllDataTypes();
		while (dataTypes.hasNext()) {
			DataType candidate = dataTypes.next();
			if (!(candidate instanceof Structure structure) || !embedsBaseAtZero(structure, baseStructure) ||
					!matchesStructureAccesses(structure, accesses)) {
				continue;
			}
			candidates.add(candidate);
			if (candidates.size() > 1) {
				return null;
			}
		}
		return candidates.size() == 1 ? new PointerDataType(candidates.iterator().next()) : null;
	}

	private static boolean embedsBaseAtZero(Structure candidate, Structure baseStructure) {
		DataTypeComponent component = candidate.getComponentAt(0);
		return component != null && component.getOffset() == 0 && component.getDataType().isEquivalent(baseStructure);
	}

	private static boolean matchesStructureAccesses(Structure structure, List<StructureAccess> accesses) {
		for (StructureAccess access : accesses) {
			DataTypeComponent component = structure.getComponentAt(access.offset());
			if (component == null || component.getLength() != access.size()) {
				return false;
			}
		}
		return true;
	}

	private static List<StructureAccess> findStructureAccesses(Function function, int stackOffset) {
		List<StructureAccess> accesses = new ArrayList<>();
		Set<String> liveRegisters = new HashSet<>();
		InstructionIterator instructions = function.getProgram().getListing().getInstructions(function.getBody(), true);
		while (instructions.hasNext()) {
			Instruction instruction = instructions.next();
			Integer sourceOffset = getExternalStackOffset(function, instruction);
			if (sourceOffset != null && sourceOffset == stackOffset) {
				for (Object result : instruction.getResultObjects()) {
					if (result instanceof Register register) {
						liveRegisters.add(register.getName());
					}
				}
			}
			for (String register : Set.copyOf(liveRegisters)) {
				Integer offset = getRegisterDisplacement(instruction, register);
				if (offset != null && offset >= 0 && getOperandSize(instruction) > 0) {
					accesses.add(new StructureAccess(offset, getOperandSize(instruction)));
				}
				if (writesRegister(instruction, register) && (sourceOffset == null || sourceOffset != stackOffset)) {
					liveRegisters.remove(register);
				}
			}
		}
		return accesses.stream().distinct().toList();
	}

	private static Integer getRegisterDisplacement(Instruction instruction, String registerName) {
		for (int index = 0; index < instruction.getNumOperands(); index++) {
			boolean hasRegister = false;
			Integer displacement = null;
			for (Object object : instruction.getOpObjects(index)) {
				if (object instanceof Register register && registerName.equals(register.getName())) {
					hasRegister = true;
				}
				else if (object instanceof Scalar scalar) {
					displacement = (int) scalar.getSignedValue();
				}
			}
			if (hasRegister && displacement != null) {
				return displacement;
			}
		}
		return null;
	}

	private static boolean replaceAnalysisParameterType(Program program, Function function, int ordinal, DataType type)
			throws InvalidInputException, DuplicateNameException {
		if (!canApplyAnalysisSignature(function)) {
			return false;
		}
		List<ParameterImpl> parameters = new ArrayList<>();
		for (Parameter parameter : function.getParameters()) {
			DataType parameterType = parameter.getOrdinal() == ordinal ? type : parameter.getDataType();
			parameters.add(new ParameterImpl(parameter.getName(), parameterType, program, SourceType.ANALYSIS));
		}
		function.updateFunction(null, function.getReturn(), FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS, true,
				SourceType.ANALYSIS, parameters.toArray(ParameterImpl[]::new));
		return true;
	}

	private record StructureAccess(int offset, int size) {
	}

	/**
	 * OpenDevice's ABI deliberately accepts the common IORequest prefix.  A
	 * statically known device name supplies the stronger, header-defined request
	 * subtype for its request argument.  The mapping is data-driven and is
	 * applied only to direct, statically addressed argument storage.
	 */
	private static void propagateOpenDeviceRequestTypes(Program program, FileDataTypeManager fdm,
			Map<Address, ForwardingApiWrapper> forwardingWrappers,
			List<AmigaAbiModel.DeviceRequestType> requestTypes, TaskMonitor monitor)
			throws CancelledException, CodeUnitInsertionException {
		Map<String, DataType> typesByDevice = new HashMap<>();
		for (AmigaAbiModel.DeviceRequestType requestType : requestTypes) {
			typesByDevice.put(requestType.deviceName, new PointerDataType(getAmigaDataType(requestType.requestType, fdm)));
		}
		Map<Address, DataType> proposals = new HashMap<>();
		Set<Address> conflicts = new HashSet<>();
		FunctionIterator callers = program.getFunctionManager().getFunctions(true);
		while (callers.hasNext()) {
			monitor.checkCancelled();
			Function caller = callers.next();
			InstructionIterator instructions = program.getListing().getInstructions(caller.getBody(), true);
			while (instructions.hasNext()) {
				Instruction call = instructions.next();
				Function callee = getDirectCallee(program, call);
				if (callee == null || !isOpenDeviceCall(callee, forwardingWrappers)) {
					continue;
				}
				Parameter deviceName = findParameter(callee, "devName");
				Parameter request = findParameter(callee, "ioRequest");
				if (deviceName == null || request == null || !deviceName.getVariableStorage().isStackStorage() ||
						!request.getVariableStorage().isStackStorage()) {
					continue;
				}
				Instruction namePush = findPushedStackArgument(caller, call, deviceName.getStackOffset());
				Instruction requestPush = findPushedStackArgument(caller, call, request.getStackOffset());
				String name = namePush == null ? null : getReferencedString(program, namePush);
				Address storage = requestPush == null ? null : getMemoryReference(requestPush, true);
				DataType type = name == null ? null : typesByDevice.get(name.toLowerCase(Locale.ROOT));
				if (storage != null && type != null) {
					recordTypeProposal(proposals, conflicts, storage, type);
				}
			}
		}
		for (Map.Entry<Address, DataType> proposal : proposals.entrySet()) {
			if (!conflicts.contains(proposal.getKey())) {
				applyAnalysisPointerStorageType(program, proposal.getKey(), proposal.getValue());
			}
		}
	}

	private static boolean isOpenDeviceCall(Function callee, Map<Address, ForwardingApiWrapper> wrappers) {
		ForwardingApiWrapper forwarding = wrappers.get(callee.getEntryPoint());
		return (forwarding != null && "OpenDevice".equals(forwarding.definition().getName(false))) ||
				"exec_library_OpenDevice".equals(callee.getName());
	}

	private static Parameter findParameter(Function function, String name) {
		for (Parameter parameter : function.getParameters()) {
			if (name.equals(parameter.getName())) {
				return parameter;
			}
		}
		return null;
	}

	private static String getReferencedString(Program program, Instruction instruction) {
		for (Reference reference : instruction.getReferencesFrom()) {
			if (!reference.isMemoryReference()) {
				continue;
			}
			Data data = program.getListing().getDefinedDataAt(reference.getToAddress());
			if (data != null && data.hasStringValue()) {
				return StringDataInstance.getStringDataInstance(data).getStringValue();
			}
		}
		return null;
	}

	/**
	 * Persists an inferred pointer type at a statically addressed argument
	 * storage location.  This covers globals and A4-relative slots, which are
	 * intentionally outside stack-to-stack forwarding.
	 */
	private static void propagateTypedPointerArgumentsToStorage(Program program, TaskMonitor monitor)
			throws CancelledException, CodeUnitInsertionException {
		Map<Address, DataType> proposals = new HashMap<>();
		Set<Address> conflicts = new HashSet<>();
		FunctionIterator callers = program.getFunctionManager().getFunctions(true);
		while (callers.hasNext()) {
			monitor.checkCancelled();
			Function caller = callers.next();
			InstructionIterator instructions = program.getListing().getInstructions(caller.getBody(), true);
			while (instructions.hasNext()) {
				Instruction call = instructions.next();
				Function callee = getDirectCallee(program, call);
				if (callee == null || callee.getSignatureSource() == SourceType.DEFAULT) {
					continue;
				}
				for (Parameter parameter : callee.getParameters()) {
					if (!parameter.getVariableStorage().isStackStorage() || !(parameter.getDataType() instanceof Pointer)) {
						continue;
					}
					Instruction push = findPushedStackArgument(caller, call, parameter.getStackOffset());
					Address storage = push == null ? null : getMemoryReference(push, true);
					if (storage != null) {
						recordTypeProposal(proposals, conflicts, storage, parameter.getDataType());
					}
				}
			}
		}
		for (Map.Entry<Address, DataType> entry : proposals.entrySet()) {
			if (!conflicts.contains(entry.getKey())) {
				applyAnalysisPointerStorageType(program, entry.getKey(), entry.getValue());
			}
		}
	}

	private static void applyAnalysisPointerStorageType(Program program, Address storage, DataType type)
			throws CodeUnitInsertionException {
		Data existing = program.getListing().getDefinedDataAt(storage);
		Symbol symbol = program.getSymbolTable().getPrimarySymbol(storage);
		if (symbol != null && symbol.getSource() == SourceType.USER_DEFINED) {
			return;
		}
		if (existing != null && !Undefined.isUndefined(existing.getDataType()) &&
				!(existing.getDataType() instanceof Pointer)) {
			return;
		}
		if (existing != null && existing.getDataType().isEquivalent(type)) {
			return;
		}
		DataUtilities.createData(program, storage, type, -1, false, ClearDataMode.CLEAR_ALL_CONFLICT_DATA);
	}

	private Map<Address, FdFunction> findOpenApiWrappers(Program program, Map<Address, String> bases) {
		Map<Address, FdFunction> wrappers = new HashMap<>();
		FunctionIterator functions = program.getFunctionManager().getFunctions(true);
		while (functions.hasNext()) {
			Function function = functions.next();
			InstructionIterator instructions = program.getListing().getInstructions(function.getBody(), true);
			while (instructions.hasNext()) {
				Instruction instruction = instructions.next();
				Integer bias = getA6VectorBias(instruction);
				if (bias == null || !FdParser.EXEC_LIB.equals(findA6ApiBase(program, instruction, bases))) {
					continue;
				}
				FdFunction functionDefinition = getFunctionByLibraryBias(FdParser.EXEC_LIB, bias);
				if (isForwardingOpenApiWrapper(program, function, instruction, functionDefinition)) {
					wrappers.put(function.getEntryPoint(), functionDefinition);
				}
			}
		}
		return wrappers;
	}

	/**
	 * Finds compiler wrappers which expose exactly one statically-proven Amiga
	 * vector call.  Unlike opener wrappers, these do not establish library-base
	 * provenance; they only preserve the called API's ordinary C-facing
	 * signature for the rest of the program.
	 */
	private Map<Address, ForwardingApiWrapper> findForwardingApiWrappers(Program program,
			Map<Address, String> bases) {
		Map<Address, ForwardingApiWrapper> wrappers = new HashMap<>();
		FunctionIterator functions = program.getFunctionManager().getFunctions(true);
		while (functions.hasNext()) {
			Function wrapper = functions.next();
			InstructionIterator instructions = program.getListing().getInstructions(wrapper.getBody(), true);
			while (instructions.hasNext()) {
				Instruction vectorCall = instructions.next();
				Integer bias = getA6VectorBias(vectorCall);
				String library = bias == null ? null : findA6ApiBase(program, vectorCall, bases);
				FdFunction definition = library == null ? null : getFunctionByLibraryBias(library, bias);
				if (definition != null && !isApiNameOpener(definition) &&
						isForwardingApiWrapper(program, wrapper, vectorCall, library, definition)) {
					wrappers.put(wrapper.getEntryPoint(), new ForwardingApiWrapper(library, definition));
				}
			}
		}
		return wrappers;
	}

	private boolean isForwardingOpenApiWrapper(Program program, Function wrapper, Instruction openerCall,
			FdFunction opener) {
		if (!isApiBaseOpener(opener) || !forwardsOpenApiArguments(program, wrapper, openerCall, opener)) {
			return false;
		}
		String returnRegister = getApiOpenerReturnRegister(program, opener);
		if (returnRegister == null) {
			return false;
		}
		boolean sawReturn = false;
		InstructionIterator instructions = program.getListing().getInstructions(wrapper.getBody(), true);
		while (instructions.hasNext()) {
			Instruction instruction = instructions.next();
			if (instruction.getAddress().equals(openerCall.getAddress())) {
				continue;
			}
			if (instruction.getFlowType().isCall() || instruction.getFlowType().isJump()) {
				return false;
			}
			if (instruction.getAddress().compareTo(openerCall.getAddress()) > 0 &&
					writesRegister(instruction, returnRegister)) {
				return false;
			}
			if (instruction.getAddress().compareTo(openerCall.getAddress()) > 0 &&
					instruction.getMnemonicString().equals("rts")) {
				sawReturn = true;
			}
		}
		return sawReturn;
	}

	private boolean isForwardingApiWrapper(Program program, Function wrapper, Instruction vectorCall,
			String library, FdFunction definition) {
		if (!forwardsApiArguments(program, wrapper, vectorCall, definition)) {
			return false;
		}
		Function apiFunction = getApiFunction(program, library, definition);
		if (apiFunction == null) {
			return false;
		}
		String returnRegister = getApiReturnRegister(program, library, definition);
		if (!isVoidDataType(apiFunction.getReturnType()) && returnRegister == null) {
			return false;
		}
		boolean sawReturn = false;
		InstructionIterator instructions = program.getListing().getInstructions(wrapper.getBody(), true);
		while (instructions.hasNext()) {
			Instruction instruction = instructions.next();
			if (instruction.getAddress().equals(vectorCall.getAddress())) {
				continue;
			}
			if (instruction.getFlowType().isCall() || instruction.getFlowType().isJump()) {
				return false;
			}
			if (returnRegister != null && instruction.getAddress().compareTo(vectorCall.getAddress()) > 0 &&
					writesRegister(instruction, returnRegister)) {
				return false;
			}
			if (instruction.getAddress().compareTo(vectorCall.getAddress()) > 0 &&
					instruction.getMnemonicString().equals("rts")) {
				sawReturn = true;
			}
		}
		return sawReturn;
	}

	private static boolean forwardsOpenApiArguments(Program program, Function wrapper, Instruction openerCall,
			FdFunction opener) {
		for (FdFunction.Arg argument : opener.getArgs()) {
			Register register = program.getRegister(argument.reg);
			if (register == null || !findStackArgumentSource(openerCall, wrapper, register)) {
				return false;
			}
		}
		return true;
	}

	private static boolean forwardsApiArguments(Program program, Function wrapper, Instruction vectorCall,
			FdFunction definition) {
		for (FdFunction.Arg argument : definition.getArgs()) {
			Register register = program.getRegister(argument.reg);
			if (register == null || !findStackArgumentSource(vectorCall, wrapper, register)) {
				return false;
			}
		}
		return true;
	}

	/**
	 * A compiler forwarding wrapper preserves the ABI-facing result and arguments
	 * of the Exec opener it calls.  Its arguments are stack values which it moves
	 * into the opener's register ABI, so let the program's calling convention
	 * assign stack storage rather than copying the vector function's registers.
	 */
	private void applyOpenApiWrapperSignatures(Program program, Map<Address, FdFunction> wrappers)
			throws InvalidInputException, DuplicateNameException {
		for (Map.Entry<Address, FdFunction> entry : wrappers.entrySet()) {
			Function wrapper = program.getFunctionManager().getFunctionAt(entry.getKey());
			MemoryBlock execBlock = program.getMemory().getBlock(FdParser.EXEC_LIB);
			if (wrapper == null || execBlock == null || !canApplyAnalysisSignature(wrapper)) {
				continue;
			}
			Address openerAddress = execBlock.getStart().add(Math.abs(entry.getValue().getBias()));
			Function opener = program.getFunctionManager().getFunctionAt(openerAddress);
			if (opener == null || isVoidDataType(opener.getReturnType())) {
				continue;
			}
			ReturnParameterImpl returnValue = new ReturnParameterImpl(opener.getReturnType(),
				opener.getReturn().getVariableStorage(), program);
			List<ParameterImpl> parameters = getOpenApiWrapperParameters(program, wrapper, opener,
				entry.getValue());
			wrapper.updateFunction(null, returnValue, FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS, true,
				SourceType.ANALYSIS, parameters.toArray(ParameterImpl[]::new));
		}
	}

	private void applyForwardingApiWrapperSignatures(Program program,
			Map<Address, ForwardingApiWrapper> wrappers) throws InvalidInputException, DuplicateNameException {
		for (Map.Entry<Address, ForwardingApiWrapper> entry : wrappers.entrySet()) {
			Function wrapper = program.getFunctionManager().getFunctionAt(entry.getKey());
			ForwardingApiWrapper forwarding = entry.getValue();
			Function apiFunction = getApiFunction(program, forwarding.library(), forwarding.definition());
			if (wrapper == null || apiFunction == null || !canApplyAnalysisSignature(wrapper)) {
				continue;
			}
			Instruction vectorCall = findApiVectorCall(program, wrapper, forwarding.definition());
			List<ParameterImpl> parameters = getForwardingApiWrapperParameters(program, wrapper, apiFunction,
					vectorCall);
			if (parameters == null) {
				continue;
			}
			ReturnParameterImpl returnValue = isVoidDataType(apiFunction.getReturnType())
					? new ReturnParameterImpl(VoidDataType.dataType, VariableStorage.VOID_STORAGE, program)
					: new ReturnParameterImpl(apiFunction.getReturnType(),
							apiFunction.getReturn().getVariableStorage(), program);
			wrapper.updateFunction(null, returnValue, FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS, true,
					SourceType.ANALYSIS, parameters.toArray(ParameterImpl[]::new));
		}
	}

	private static boolean canApplyAnalysisSignature(Function function) {
		SourceType source = function.getSignatureSource();
		return source == SourceType.DEFAULT || source == SourceType.ANALYSIS;
	}

	private List<ParameterImpl> getOpenApiWrapperParameters(Program program, Function wrapper,
			Function opener, FdFunction definition) throws InvalidInputException {
		Instruction vectorCall = findOpenApiVectorCall(program, wrapper, definition);
		if (vectorCall == null) {
			return List.of();
		}
		List<ParameterImpl> parameters = new ArrayList<>();
		for (var openerParameter : opener.getParameters()) {
			Register register = openerParameter.getRegister();
			if (register == null || !findStackArgumentSource(vectorCall, wrapper, register)) {
				return List.of();
			}
			parameters.add(new ParameterImpl(openerParameter.getName(), openerParameter.getDataType(), program,
				SourceType.ANALYSIS));
		}
		return parameters;
	}

	private List<ParameterImpl> getForwardingApiWrapperParameters(Program program, Function wrapper,
			Function apiFunction, Instruction vectorCall) throws InvalidInputException {
		if (vectorCall == null) {
			return null;
		}
		List<ParameterImpl> parameters = new ArrayList<>();
		for (var apiParameter : apiFunction.getParameters()) {
			Register register = apiParameter.getRegister();
			if (register == null || !findStackArgumentSource(vectorCall, wrapper, register)) {
				return null;
			}
			parameters.add(new ParameterImpl(apiParameter.getName(), apiParameter.getDataType(), program,
					SourceType.ANALYSIS));
		}
		return parameters;
	}

	private Instruction findOpenApiVectorCall(Program program, Function wrapper, FdFunction definition) {
		return findApiVectorCall(program, wrapper, definition);
	}

	private Instruction findApiVectorCall(Program program, Function wrapper, FdFunction definition) {
		InstructionIterator instructions = program.getListing().getInstructions(wrapper.getBody(), true);
		while (instructions.hasNext()) {
			Instruction instruction = instructions.next();
			Integer bias = getA6VectorBias(instruction);
			if (bias != null && bias == definition.getBias()) {
				return instruction;
			}
		}
		return null;
	}

	private static boolean findStackArgumentSource(Instruction vectorCall, Function wrapper, Register register) {
		for (Instruction instruction = vectorCall.getPrevious(); instruction != null &&
				wrapper.getBody().contains(instruction.getAddress()); instruction = instruction.getPrevious()) {
			if (!writesRegister(instruction, register.getName())) {
				continue;
			}
			return hasSourceRegister(instruction, "SP");
		}
		return false;
	}

	/**
	 * A library-base global is a typed role rather than a constant value: a
	 * close path may clear it after an opener has assigned it. Infer that role
	 * from all non-zero defining writes, retaining it only if they agree.
	 */
	private void inferApiBaseSlots(Program program, Map<Address, String> bases,
			Set<Address> ambiguousStorages, Map<Address, FdFunction> openWrappers) {
		boolean changed;
		do {
			changed = false;
			InstructionIterator instructions = program.getListing().getInstructions(true);
			while (instructions.hasNext()) {
				Instruction instruction = instructions.next();
				if (!writesRegister(instruction, "A6")) {
					continue;
				}
				Address storage = getMemoryReference(instruction, true);
				if (storage == null || bases.containsKey(storage) || ambiguousStorages.contains(storage)) {
					continue;
				}
				String library = inferApiBaseSlot(program, storage, bases, openWrappers);
				if (library != null) {
					changed |= recordApiBaseStorage(bases, ambiguousStorages, storage, library);
				}
			}
		} while (changed);
	}

	private String inferApiBaseSlot(Program program, Address storage, Map<Address, String> bases,
			Map<Address, FdFunction> openWrappers) {
		String library = null;
		ReferenceIterator references = program.getReferenceManager().getReferencesTo(storage);
		while (references.hasNext()) {
			Reference reference = references.next();
			if (!reference.getReferenceType().isWrite()) {
				continue;
			}
			Instruction write = program.getListing().getInstructionAt(reference.getFromAddress());
			if (write == null || clearsStorage(write)) {
				continue;
			}
			String writtenLibrary = getOpeningLibraryStoredBy(program, write, bases, openWrappers);
			if (writtenLibrary == null || (library != null && !library.equals(writtenLibrary))) {
				return null;
			}
			library = writtenLibrary;
		}
		return library;
	}

	private static boolean clearsStorage(Instruction instruction) {
		return instruction.getMnemonicString().startsWith("clr");
	}

	private String getOpeningLibraryStoredBy(Program program, Instruction store, Map<Address, String> bases,
			Map<Address, FdFunction> openWrappers) {
		for (Object input : store.getOpObjects(0)) {
			if (!(input instanceof Register register)) {
				continue;
			}
			for (Instruction instruction = store.getPrevious(); instruction != null;
					instruction = instruction.getPrevious()) {
				Address target = getDirectCallTarget(instruction);
				FdFunction opener = target == null ? null : openWrappers.get(target);
				if (opener == null) {
					opener = getDirectOpenApiCall(program, instruction, bases);
				}
				if (opener != null && isApiBaseOpener(opener)) {
					String returnRegister = getApiOpenerReturnRegister(program, opener);
					ApiNameArgument argument = getApiNameArgument(program, instruction, opener);
					return register.getName().equals(returnRegister) && argument != null
							? argument.apiKey() : null;
				}
				if (writesRegister(instruction, register.getName())) {
					return null;
				}
				if (instruction.getFlowType().isJump() || instruction.getFlowType().isTerminal()) {
					return null;
				}
			}
		}
		return null;
	}

	private FdFunction getDirectOpenApiCall(Program program, Instruction instruction, Map<Address, String> bases) {
		Integer bias = getA6VectorBias(instruction);
		if (bias == null || !FdParser.EXEC_LIB.equals(findA6ApiBase(program, instruction, bases))) {
			return null;
		}
		FdFunction function = getFunctionByLibraryBias(FdParser.EXEC_LIB, bias);
		return isApiNameOpener(function) ? function : null;
	}

	private static boolean isApiNameOpener(FdFunction function) {
		if (function == null) {
			return false;
		}
		String name = function.getName(false);
		return name.equals("OpenLibrary") || name.equals("OldOpenLibrary") || name.equals("OpenDevice") ||
				name.equals("OpenResource");
	}

	private static boolean isApiBaseOpener(FdFunction function) {
		return function != null && !function.getName(false).equals("OpenDevice") && isApiNameOpener(function);
	}

	private FdFunction getFunctionByLibraryBias(String library, int bias) {
		FdLibFunctions table = funcsList.getFunctionTableByLib(library);
		return table == null ? null : table.getFunctionByBias(bias);
	}

	private Function getApiFunction(Program program, String library, FdFunction definition) {
		MemoryBlock libraryBlock = program.getMemory().getBlock(library);
		return libraryBlock == null ? null : program.getFunctionManager()
				.getFunctionAt(libraryBlock.getStart().add(Math.abs(definition.getBias())));
	}

	private String getApiReturnRegister(Program program, String library, FdFunction definition) {
		Function function = getApiFunction(program, library, definition);
		if (function == null || isVoidDataType(function.getReturnType())) {
			return null;
		}
		return getSingleReturnRegister(function);
	}

	private static String getSingleReturnRegister(Function function) {
		List<Register> registers = function.getReturn().getVariableStorage().getRegisters();
		return registers != null && registers.size() == 1 ? registers.get(0).getName() : null;
	}

	/**
	 * Follows the specific ABI name register to its defining string in the same
	 * straight-line function path. Register-to-register copies are supported;
	 * calls, branches, and unrecognised computations are deliberately rejected.
	 */
	private static ApiNameArgument getApiNameArgument(Program program, Instruction call, FdFunction opener) {
		String register = getApiNameArgumentRegister(opener);
		Function function = register == null ? null :
				program.getFunctionManager().getFunctionContaining(call.getAddress());
		if (function == null) {
			return null;
		}
		CodeBlock block;
		try {
			block = new BasicBlockModel(program).getFirstCodeBlockContaining(call.getAddress(), TaskMonitor.DUMMY);
		}
		catch (CancelledException exception) {
			return null;
		}
		if (block == null) {
			return null;
		}
		for (Instruction instruction = call.getPrevious(); instruction != null &&
				block.contains(instruction.getAddress());
				instruction = instruction.getPrevious()) {
			if (instruction.getFlowType().isCall() || instruction.getFlowType().isJump() ||
					instruction.getFlowType().isTerminal()) {
				return null;
			}
			if (!writesRegister(instruction, register)) {
				continue;
			}
			for (Reference reference : instruction.getReferencesFrom()) {
				if (!reference.isMemoryReference()) {
					continue;
				}
				Data data = program.getListing().getDefinedDataAt(reference.getToAddress());
				if (data != null && data.hasStringValue()) {
					String apiKey = toApiBaseName(StringDataInstance.getStringDataInstance(data).getStringValue());
					return apiKey == null ? null : new ApiNameArgument(apiKey, data.getAddress());
				}
			}
			String sourceRegister = getSingleInputRegister(instruction);
			if (sourceRegister == null || sourceRegister.equals(register)) {
				return null;
			}
			register = sourceRegister;
		}
		return null;
	}

	private static String getApiNameArgumentRegister(FdFunction opener) {
		if (!isApiNameOpener(opener) || opener.getArgs().isEmpty()) {
			return null;
		}
		String register = opener.getArgs().get(0).reg;
		return register == null ? null : register.toUpperCase(Locale.ROOT);
	}

	private static String getSingleInputRegister(Instruction instruction) {
		String source = null;
		for (Object object : instruction.getInputObjects()) {
			if (!(object instanceof Register register) || register.getName().equals("CCR")) {
				continue;
			}
			if (source != null) {
				return null;
			}
			source = register.getName();
		}
		return source;
	}

	private static Address getReturnedRegisterStorage(Program program, Instruction call, String registerName) {
		if (registerName == null) {
			return null;
		}
		Function function = program.getFunctionManager().getFunctionContaining(call.getAddress());
		for (Instruction instruction = call.getNext(); instruction != null &&
				(function == null || function.getBody().contains(instruction.getAddress()));
				instruction = instruction.getNext()) {
			if (hasSourceRegister(instruction, registerName)) {
				Address storage = getMemoryReference(instruction, false);
				if (storage != null) {
					return storage;
				}
			}
			if (writesRegister(instruction, registerName)) {
				break;
			}
		}
		return null;
	}

	private static Address getDirectCallTarget(Instruction instruction) {
		for (Reference reference : instruction.getReferencesFrom()) {
			if (reference.getReferenceType().isCall() && !reference.getReferenceType().isComputed()) {
				return reference.getToAddress();
			}
		}
		return null;
	}

	private static Address getMemoryReference(Instruction instruction, boolean read) {
		for (Reference reference : instruction.getReferencesFrom()) {
			if (reference.isMemoryReference() && (read ? reference.getReferenceType().isRead() : reference.getReferenceType().isWrite())) {
				return reference.getToAddress();
			}
		}
		return null;
	}

	private static boolean hasSourceRegister(Instruction instruction, String registerName) {
		for (Object object : instruction.getInputObjects()) {
			if (object instanceof Register register && registerName.equals(register.getName())) {
				return true;
			}
		}
		return false;
	}

	private static Integer getA6VectorBias(Instruction instruction) {
		String mnemonic = instruction.getMnemonicString();
		if (!(mnemonic.equals("jsr") || mnemonic.equals("jmp"))) {
			return null;
		}
		Scalar displacement = null;
		boolean hasA6 = false;
		for (Object object : instruction.getOpObjects(0)) {
			if (object instanceof Register register && register.getName().equals("A6")) {
				hasA6 = true;
			}
			if (object instanceof Scalar scalar) {
				displacement = scalar;
			}
		}
		if (!hasA6 || displacement == null || displacement.getSignedValue() >= 0) {
			return null;
		}
		return (int) displacement.getSignedValue();
	}

	private static String findA6ApiBase(Program program, Instruction call, Map<Address, String> bases) {
		Address storage = findA6Storage(program, call);
		return storage == null ? null : bases.get(storage);
	}

	private static Address findA6Storage(Program program, Instruction call) {
		Function function = program.getFunctionManager().getFunctionContaining(call.getAddress());
		if (function == null) {
			return null;
		}
		for (Instruction instruction = call.getPrevious(); instruction != null && function.getBody().contains(instruction.getAddress());
				instruction = instruction.getPrevious()) {
			if (!writesRegister(instruction, "A6")) {
				continue;
			}
			return getMemoryReference(instruction, true);
		}
		return null;
	}

	private static boolean writesRegister(Instruction instruction, String registerName) {
		for (Object object : instruction.getResultObjects()) {
			if (object instanceof Register register && registerName.equals(register.getName())) {
				return true;
			}
		}
		return false;
	}

	private void addResolvedApiCall(Program program, Instruction instruction, String library, int bias) {
		if (!activeLibraries.contains(library)) {
			return;
		}
		FdFunction functionDefinition = getFunctionByLibraryBias(library, bias);
		MemoryBlock libraryBlock = program.getMemory().getBlock(library);
		if (functionDefinition == null || libraryBlock == null) {
			return;
		}
		Address target = libraryBlock.getStart().add(Math.abs(functionDefinition.getBias()));
		Reference primary = instruction.getPrimaryReference(0);
		if (primary != null && primary.getSource() != SourceType.ANALYSIS) {
			return;
		}
		instruction.addOperandReference(0, target, RefType.CALL_OVERRIDE_UNCONDITIONAL, SourceType.ANALYSIS);
		for (Reference reference : instruction.getOperandReferences(0)) {
			if (target.equals(reference.getToAddress())) {
				instruction.setPrimaryMemoryReference(reference);
				break;
			}
		}
	}

	/**
	 * Resolves vector calls from library bases whose origin is proven on every
	 * incoming basic-block path.  The state deliberately contains only a
	 * register-to-library association: a merge retains an association only when
	 * all predecessors agree, so ambiguous control flow cannot produce a call
	 * override.
	 */
	boolean resolveApiCallsInFunction(Program program, Function function,
			Map<Address, String> apiBaseStorages, Set<Address> ambiguousStorages,
			Map<Address, FdFunction> openWrappers, TaskMonitor monitor) throws CancelledException {
		BasicBlockModel blockModel = new BasicBlockModel(program);
		List<CodeBlock> blocks = new ArrayList<>();
		CodeBlockIterator iterator = blockModel.getCodeBlocksContaining(function.getBody(), monitor);
		while (iterator.hasNext()) {
			CodeBlock block = iterator.next();
			if (function.getBody().contains(block.getFirstStartAddress())) {
				blocks.add(block);
			}
		}
		Map<Address, ApiBaseState> outStates = new HashMap<>();
		boolean storageChanged = false;
		boolean stateChanged;
			do {
				stateChanged = false;
				for (CodeBlock block : blocks) {
					monitor.checkCancelled();
					ApiBaseState state = incomingState(block, function, outStates, monitor);
					if (state == null) {
						continue;
					}
					InstructionIterator instructions = program.getListing().getInstructions(block, true);
				while (instructions.hasNext()) {
					storageChanged |= transferApiBaseState(program, instructions.next(), state,
							apiBaseStorages, ambiguousStorages, openWrappers, false);
				}
				ApiBaseState previous = outStates.put(block.getFirstStartAddress(), state);
				stateChanged |= !state.equals(previous);
			}
			} while (stateChanged);

		// The fixed point above may visit a block with provisional predecessor
		// state.  Install call overrides only after all states are stable, so an
		// override can never survive a later conflicting merge.
		for (CodeBlock block : blocks) {
			monitor.checkCancelled();
			ApiBaseState state = incomingState(block, function, outStates, monitor);
			if (state == null) {
				continue;
			}
			InstructionIterator instructions = program.getListing().getInstructions(block, true);
			while (instructions.hasNext()) {
				transferApiBaseState(program, instructions.next(), state, apiBaseStorages,
						ambiguousStorages, openWrappers, true);
			}
		}
		return storageChanged;
	}

	private static ApiBaseState incomingState(CodeBlock block, Function function,
			Map<Address, ApiBaseState> outStates, TaskMonitor monitor) throws CancelledException {
		ApiBaseState incoming = null;
		boolean hasInFunctionSource = false;
		CodeBlockReferenceIterator sources = block.getSources(monitor);
		while (sources.hasNext()) {
			CodeBlock source = sources.next().getSourceBlock();
			if (!function.getBody().contains(source.getFirstStartAddress())) {
				continue;
			}
			hasInFunctionSource = true;
			ApiBaseState sourceState = outStates.get(source.getFirstStartAddress());
			if (sourceState == null) {
				continue;
			}
			incoming = incoming == null ? sourceState.copy() : incoming.intersection(sourceState);
		}
		if (incoming != null) {
			return incoming;
		}
		return hasInFunctionSource ? null : new ApiBaseState();
	}

	private boolean transferApiBaseState(Program program, Instruction instruction, ApiBaseState state,
			Map<Address, String> apiBaseStorages, Set<Address> ambiguousStorages,
			Map<Address, FdFunction> openWrappers, boolean installOverrides) {
		Integer bias = getA6VectorBias(instruction);
		String vectorLibrary = bias == null ? null : state.get("A6");
		if (installOverrides && vectorLibrary != null) {
			addResolvedApiCall(program, instruction, vectorLibrary, bias);
		}

		Address sourceStorage = getMemoryReference(instruction, true);
		String sourceLibrary = sourceStorage == null ? null : apiBaseStorages.get(sourceStorage);
		if (sourceLibrary == null) {
			sourceLibrary = getSourceRegisterBase(instruction, state);
		}
		for (Object result : instruction.getResultObjects()) {
			if (result instanceof Register register) {
				state.put(register.getName(), sourceLibrary);
			}
		}

		boolean storageChanged = false;
		Address destinationStorage = getMemoryReference(instruction, false);
		if (destinationStorage != null && sourceLibrary != null) {
			storageChanged = recordApiBaseStorage(apiBaseStorages, ambiguousStorages,
				destinationStorage, sourceLibrary);
		}

		Address directTarget = getDirectCallTarget(instruction);
		FdFunction opener = directTarget == null ? null : openWrappers.get(directTarget);
		if (opener == null && isOpenApiVectorCall(vectorLibrary, bias)) {
			opener = getFunctionByLibraryBias(FdParser.EXEC_LIB, bias);
		}
		if (opener != null && isApiBaseOpener(opener)) {
			ApiNameArgument argument = getApiNameArgument(program, instruction, opener);
			String library = argument == null ? null : argument.apiKey();
			String returnRegister = getApiOpenerReturnRegister(program, opener);
			if (returnRegister != null) {
				state.put(returnRegister, library != null && activeLibraries.contains(library) ? library : null);
			}
		}
		else if (bias != null || directTarget != null) {
			state.put("D0", null);
		}
		return storageChanged;
	}

	private String getApiOpenerReturnRegister(Program program, FdFunction opener) {
		MemoryBlock execBlock = program.getMemory().getBlock(FdParser.EXEC_LIB);
		if (execBlock == null) {
			return null;
		}
		Function function = program.getFunctionManager()
				.getFunctionAt(execBlock.getStart().add(Math.abs(opener.getBias())));
		if (function == null) {
			return null;
		}
		return getSingleReturnRegister(function);
	}

	private static String getSourceRegisterBase(Instruction instruction, ApiBaseState state) {
		for (Object object : instruction.getInputObjects()) {
			if (object instanceof Register register) {
				String library = state.get(register.getName());
				if (library != null) {
					return library;
				}
			}
		}
		return null;
	}

	private boolean isOpenApiVectorCall(String library, Integer bias) {
		if (!FdParser.EXEC_LIB.equals(library) || bias == null) {
			return false;
		}
		return isApiBaseOpener(getFunctionByLibraryBias(FdParser.EXEC_LIB, bias));
	}

	private record ForwardingApiWrapper(String library, FdFunction definition) {
	}

	static final class ApiBaseState {
		private final Map<String, String> registerBases;

		ApiBaseState() {
			registerBases = new HashMap<>();
		}

		private ApiBaseState(Map<String, String> registerBases) {
			this.registerBases = new HashMap<>(registerBases);
		}

		String get(String registerName) {
			return registerBases.get(registerName);
		}

		void put(String registerName, String library) {
			if (library == null) {
				registerBases.remove(registerName);
			}
			else {
				registerBases.put(registerName, library);
			}
		}

		ApiBaseState copy() {
			return new ApiBaseState(registerBases);
		}

		ApiBaseState intersection(ApiBaseState other) {
			ApiBaseState result = new ApiBaseState();
			for (Map.Entry<String, String> entry : registerBases.entrySet()) {
				if (entry.getValue().equals(other.registerBases.get(entry.getKey()))) {
					result.put(entry.getKey(), entry.getValue());
				}
			}
			return result;
		}

		@Override
		public boolean equals(Object object) {
			return object instanceof ApiBaseState other && registerBases.equals(other.registerBases);
		}

		@Override
		public int hashCode() {
			return registerBases.hashCode();
		}
	}
	
}
