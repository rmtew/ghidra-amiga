package amiga;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

import ghidra.app.services.AbstractAnalyzer;
import ghidra.app.services.AnalysisPriority;
import ghidra.app.services.AnalyzerType;
import ghidra.app.util.importer.MessageLog;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.data.CharDataType;
import ghidra.program.model.data.LongDataType;
import ghidra.program.model.data.PointerDataType;
import ghidra.program.model.data.VoidDataType;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.listing.ParameterImpl;
import ghidra.program.model.listing.Program;
import ghidra.program.model.listing.ReturnParameterImpl;
import ghidra.program.model.listing.VariableStorage;
import ghidra.program.model.listing.Function.FunctionUpdateType;
import ghidra.program.model.symbol.RefType;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.symbol.SymbolTable;
import ghidra.program.model.scalar.Scalar;
import ghidra.util.exception.CancelledException;
import ghidra.util.exception.DuplicateNameException;
import ghidra.util.exception.InvalidInputException;
import ghidra.util.task.TaskMonitor;

/**
 * Identifies a small, deliberately conservative subset of MANX C's linked
 * arithmetic runtime. The recognisers use instruction and control-flow
 * properties, not image addresses or application-specific byte signatures.
 */
public class ManxRuntimeAnalyzer extends AbstractAnalyzer {

	private static final String MANX_COMPILER_SPEC_ID = "manx";
	private static final String A4_STUB_PREFIX = "ManxA4CallStub_";
	private static final long MANX_STACK_MARKER = 0x4d414e58L;
	private static final Set<String> MANX_STARTUP_APIS = Set.of(
			"AllocMem", "Alert", "FindTask", "WaitPort", "GetMsg", "CurrentDir", "Input", "Output", "Open");

	enum RuntimeHelper {
		MULTIPLY_LONG32_LOW("ManxMultiplyLong32Low",
				"MANX runtime: calculates the low 32 bits of the product in D0 and D1."),
		UNSIGNED_LONG_DIVISION_CORE("ManxUnsignedLongDivisionCore",
				"MANX runtime: unsigned 32-bit division core; returns quotient in D0 and remainder in D1."),
		UNSIGNED_DIVIDE_LONG32("ManxUnsignedDivideLong32",
				"MANX runtime: unsigned 32-bit division wrapper that preserves the divisor in D1."),
		DIVIDE_LONG32("ManxDivideLong32",
				"MANX runtime: signed 32-bit division wrapper around the unsigned division core."),
		MODULO_LONG32("ManxModuloLong32",
				"MANX runtime: signed 32-bit remainder wrapper around the unsigned division core."),
		STRING_LENGTH("ManxStrlen", "MANX runtime: returns the byte length of a NUL-terminated string.");

		final String functionName;
		final String comment;

		RuntimeHelper(String functionName, String comment) {
			this.functionName = functionName;
			this.comment = comment;
		}
	}

	public ManxRuntimeAnalyzer() {
		super("MANX Runtime", "Recognises validated MANX CRT startup, runtime helpers, and A4 forwarding stubs",
				AnalyzerType.INSTRUCTION_ANALYZER);
		// The recogniser relies on the Amiga library-call analyzer having already
		// resolved the standard Exec and DOS calls used by the CRT.
		setPriority(AnalysisPriority.DATA_ANALYSIS.after().after());
		setSupportsOneTimeAnalysis();
	}

	@Override
	public boolean getDefaultEnablement(Program program) {
		return canAnalyze(program);
	}

	@Override
	public boolean canAnalyze(Program program) {
		return MANX_COMPILER_SPEC_ID.equals(program.getCompilerSpec().getCompilerSpecID().getIdAsString());
	}

	@Override
	public boolean added(Program program, AddressSetView set, TaskMonitor monitor, MessageLog log)
			throws CancelledException {
		Listing listing = program.getListing();
		Map<Function, FunctionShape> shapes = new HashMap<>();
		Iterator<Function> functions = program.getFunctionManager().getFunctionsOverlapping(set);
		while (functions.hasNext()) {
			monitor.checkCancelled();
			Function function = functions.next();
			shapes.put(function, FunctionShape.from(program, listing, function));
		}

		Map<Function, RuntimeHelper> classifications = new HashMap<>();
		for (Map.Entry<Function, FunctionShape> entry : shapes.entrySet()) {
			RuntimeHelper helper = classifyLeafHelper(entry.getValue());
			if (helper != null) {
				classifications.put(entry.getKey(), helper);
			}
		}
		if (classifications.containsValue(RuntimeHelper.UNSIGNED_LONG_DIVISION_CORE)) {
			FunctionIterator allFunctions = program.getFunctionManager().getFunctions(true);
			while (allFunctions.hasNext()) {
				monitor.checkCancelled();
				Function function = allFunctions.next();
				shapes.putIfAbsent(function, FunctionShape.from(program, listing, function));
			}
			for (Map.Entry<Function, FunctionShape> entry : shapes.entrySet()) {
				RuntimeHelper helper = classifyLeafHelper(entry.getValue());
				if (helper != null) {
					classifications.put(entry.getKey(), helper);
				}
			}
		}
		for (Map.Entry<Function, FunctionShape> entry : shapes.entrySet()) {
			RuntimeHelper helper = classifySignedWrapper(entry.getValue(), classifications.keySet());
			if (helper != null) {
				classifications.put(entry.getKey(), helper);
			}
		}

		for (Map.Entry<Function, RuntimeHelper> entry : classifications.entrySet()) {
			monitor.checkCancelled();
			nameRuntimeHelper(entry.getKey(), entry.getValue(), log);
		}
		propagateNamesToA4Stubs(program, classifications.keySet(), log, monitor);
		identifyManxStartup(program, shapes.values(), log, monitor);
		return true;
	}

	/**
	 * Recognises the standard Aztec C 5 Amiga CRT entry point, {@code _main}.
	 *
	 * <p>The implementation is deliberately a conjunction of independent
	 * properties from the shipped Manx CRT source: its MANX stack marker and
	 * its CLI/Workbench setup calls. The marker alone is not enough, because
	 * applications can legitimately store the same value in unrelated data.</p>
	 */
	private static void identifyManxStartup(Program program, Collection<FunctionShape> shapes, MessageLog log,
			TaskMonitor monitor) throws CancelledException {
		for (FunctionShape shape : shapes) {
			monitor.checkCancelled();
			if (!isManxStartup(shape) || !hasDefaultName(shape.function)) {
				continue;
			}
			nameManxStartup(program, shape.function, log);
		}
	}

	static boolean isManxStartup(FunctionShape shape) {
		return shape.hasManxStackMarker && shape.directCalleeNames.containsAll(MANX_STARTUP_APIS);
	}

	private static void nameManxStartup(Program program, Function function, MessageLog log) {
		try {
			ParameterImpl[] parameters = {
				new ParameterImpl("commandLineLength", LongDataType.dataType, program, SourceType.ANALYSIS),
				new ParameterImpl("commandLine", new PointerDataType(CharDataType.dataType), program,
						SourceType.ANALYSIS)
			};
			function.updateFunction(function.getCallingConventionName(),
					new ReturnParameterImpl(VoidDataType.dataType, VariableStorage.VOID_STORAGE, program),
					FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS, true, SourceType.ANALYSIS, parameters);
			function.setName(AmigaUtils.uniqueAnalysisSymbolName(program, function.getEntryPoint(), "_main"),
					SourceType.ANALYSIS);
			if (function.getComment() == null || function.getComment().isBlank()) {
				function.setComment("MANX Aztec C CRT startup: initializes CLI or Workbench launch state, " +
						"then invokes the program main path.");
			}
			log.appendMsg(String.format("Recognised MANX Aztec C CRT _main at %s.", function.getEntryPoint()));
		}
		catch (DuplicateNameException | InvalidInputException e) {
			log.appendException(e);
		}
	}

	static RuntimeHelper classifyMnemonicSequence(List<String> mnemonics, boolean callsDivisionCore,
			boolean movesD1ToD0) {
		int multiplyUnsigned = count(mnemonics, "mulu");
		int divideUnsigned = count(mnemonics, "divu");
		int swaps = count(mnemonics, "swap");
		int clearWords = count(mnemonics, "clr");
		int addLongs = count(mnemonics, "add");
		int negateLongs = count(mnemonics, "neg");
		int tests = count(mnemonics, "tst");
		int exclusiveOrs = count(mnemonics, "eori");
		int dbf = count(mnemonics, "dbf");
		int addExtended = count(mnemonics, "addx");

		if (multiplyUnsigned == 3 && swaps >= 3 && clearWords >= 2 && addLongs >= 2 &&
				mnemonics.size() >= 12 && mnemonics.size() <= 24) {
			return RuntimeHelper.MULTIPLY_LONG32_LOW;
		}
		if (divideUnsigned == 2 && dbf == 1 && addExtended == 1 && swaps >= 5 &&
				mnemonics.size() >= 28) {
			return RuntimeHelper.UNSIGNED_LONG_DIVISION_CORE;
		}
		if (callsDivisionCore && mnemonics.equals(List.of("move", "bsr", "move", "tst", "rts"))) {
			return RuntimeHelper.UNSIGNED_DIVIDE_LONG32;
		}
		if (callsDivisionCore && negateLongs >= 3 && tests >= 2 && exclusiveOrs >= 1) {
			return RuntimeHelper.DIVIDE_LONG32;
		}
		if (callsDivisionCore && negateLongs >= 2 && tests >= 2 && exclusiveOrs == 0 && movesD1ToD0) {
			return RuntimeHelper.MODULO_LONG32;
		}
		if (!callsDivisionCore && mnemonics.equals(List.of("movea", "move", "tst", "bne", "subq", "suba",
				"move", "rts"))) {
			return RuntimeHelper.STRING_LENGTH;
		}
		return null;
	}

	private static RuntimeHelper classifyLeafHelper(FunctionShape shape) {
		if (!shape.directCallees.isEmpty()) {
			return null;
		}
		return classifyMnemonicSequence(shape.mnemonics, false, false);
	}

	private static RuntimeHelper classifySignedWrapper(FunctionShape shape, Collection<Function> divisionCores) {
		boolean callsDivisionCore = false;
		for (Function target : shape.directCallees) {
			if (divisionCores.contains(target)) {
				callsDivisionCore = true;
				break;
			}
		}
		return classifyMnemonicSequence(shape.mnemonics, callsDivisionCore, shape.movesD1ToD0);
	}

	private static int count(List<String> mnemonics, String mnemonic) {
		int count = 0;
		for (String value : mnemonics) {
			if (mnemonic.equals(value)) {
				count++;
			}
		}
		return count;
	}

	static boolean isMoveLongD1ToD0(Instruction instruction) {
		if (!"move.l".equalsIgnoreCase(instruction.getMnemonicString()) || instruction.getNumOperands() != 2) {
			return false;
		}
		return isSingleRegisterNamed(instruction.getOpObjects(0), "D1") &&
				isSingleRegisterNamed(instruction.getOpObjects(1), "D0");
	}

	private static boolean isSingleRegisterNamed(Object[] objects, String registerName) {
		return objects.length == 1 && objects[0] instanceof Register register &&
				registerName.equalsIgnoreCase(register.getName());
	}

	static String normalizeMnemonic(String mnemonic) {
		int sizeSuffix = mnemonic.indexOf('.');
		return (sizeSuffix < 0 ? mnemonic : mnemonic.substring(0, sizeSuffix)).toLowerCase(Locale.ROOT);
	}

	private static void nameRuntimeHelper(Function function, RuntimeHelper helper, MessageLog log) {
		if (!hasDefaultName(function)) {
			return;
		}
		try {
			function.setName(AmigaUtils.uniqueAnalysisSymbolName(function.getProgram(), function.getEntryPoint(),
					helper.functionName), SourceType.ANALYSIS);
			if (function.getComment() == null || function.getComment().isBlank()) {
				function.setComment(helper.comment);
			}
			log.appendMsg(String.format("Recognised %s at %s.", helper.functionName, function.getEntryPoint()));
		} catch (DuplicateNameException | InvalidInputException e) {
			log.appendException(e);
		}
	}

	private static boolean hasDefaultName(Function function) {
		return function.getSymbol().getSource() == SourceType.DEFAULT;
	}

	private static void propagateNamesToA4Stubs(Program program, Collection<Function> runtimeFunctions,
			MessageLog log, TaskMonitor monitor) throws CancelledException {
		SymbolTable symbols = program.getSymbolTable();
		for (Function target : runtimeFunctions) {
			monitor.checkCancelled();
			if (hasDefaultName(target)) {
				continue;
			}
			ReferenceIterator references = program.getReferenceManager().getReferencesTo(target.getEntryPoint());
			while (references.hasNext()) {
				Reference reference = references.next();
				if (reference.getReferenceType() != RefType.UNCONDITIONAL_JUMP) {
					continue;
				}
				Function stub = program.getFunctionManager().getFunctionAt(reference.getFromAddress());
				if (stub == null || !stub.getName().startsWith(A4_STUB_PREFIX)) {
					continue;
				}
				String name = nextThunkName(symbols, target.getName());
				try {
					stub.setName(name, SourceType.ANALYSIS);
					if (stub.getComment() == null || stub.getComment().isBlank()) {
						stub.setComment("MANX A4 forwarding stub to " + target.getName() + ".");
					}
					log.appendMsg(String.format("Named MANX forwarding stub %s as %s.", reference.getFromAddress(), name));
				} catch (DuplicateNameException | InvalidInputException e) {
					log.appendException(e);
				}
			}
		}
	}

	private static String nextThunkName(SymbolTable symbols, String targetName) {
		for (int index = 1;; index++) {
			String candidate = targetName + "_Thunk" + index;
			if (symbols.getGlobalSymbols(candidate).isEmpty()) {
				return candidate;
			}
		}
	}

	static final class FunctionShape {
		final Function function;
		final List<String> mnemonics = new ArrayList<>();
		final Set<Function> directCallees = new HashSet<>();
		final Set<String> directCalleeNames = new HashSet<>();
		boolean hasManxStackMarker;
		boolean movesD1ToD0;

		private FunctionShape(Function function) {
			this.function = function;
		}

		static FunctionShape from(Program program, Listing listing, Function function) {
			FunctionShape shape = new FunctionShape(function);
			InstructionIterator instructions = listing.getInstructions(function.getBody(), true);
			while (instructions.hasNext()) {
				Instruction instruction = instructions.next();
				String mnemonic = normalizeMnemonic(instruction.getMnemonicString());
				shape.mnemonics.add(mnemonic);
				shape.movesD1ToD0 |= isMoveLongD1ToD0(instruction);
				for (int operand = 0; operand < instruction.getNumOperands(); operand++) {
					for (Object object : instruction.getOpObjects(operand)) {
						if (object instanceof Scalar scalar) {
							shape.hasManxStackMarker |= scalar.getUnsignedValue() == MANX_STACK_MARKER;
						}
					}
				}
				if (!instruction.getFlowType().isCall()) {
					continue;
				}
				// Amiga resident-library calls are normally register-indirect. The
				// library analyzer attaches a CALL_OVERRIDE reference to the API
				// model, so there may be no native flow target to inspect here.
				for (Reference reference : program.getReferenceManager().getReferencesFrom(instruction.getAddress())) {
					if (reference.getReferenceType().isCall()) {
						shape.addDirectCallee(program.getFunctionManager().getFunctionAt(reference.getToAddress()));
					}
				}
				for (Address flow : instruction.getFlows()) {
					shape.addDirectCallee(program.getFunctionManager().getFunctionAt(flow));
				}
			}
			return shape;
		}

		private void addDirectCallee(Function target) {
			// Calls within the current function are ordinary control flow, not
			// calls to another runtime helper.
			if (target != null && target != function) {
				directCallees.add(target);
				directCalleeNames.add(apiName(target));
			}
		}

		private static String apiName(Function function) {
			String name = function.getName();
			int separator = name.lastIndexOf('_');
			return separator < 0 ? name : name.substring(separator + 1);
		}
	}
}
