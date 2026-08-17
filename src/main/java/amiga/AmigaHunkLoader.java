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

import ghidra.app.plugin.core.reloc.InstructionStasher;
import ghidra.app.util.Option;
import ghidra.app.util.OptionException;
import ghidra.app.util.PseudoInstruction;
import ghidra.app.util.bin.BinaryReader;
import ghidra.app.util.bin.ByteProvider;
import ghidra.app.util.importer.MessageLog;
import ghidra.app.util.opinion.AbstractLibrarySupportLoader;
import ghidra.app.util.opinion.LoadSpec;
import ghidra.app.util.opinion.Loader;
import ghidra.framework.Application;
import ghidra.framework.model.DomainObject;
import ghidra.framework.options.Options;
import ghidra.program.flatapi.FlatProgramAPI;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.DWordDataType;
import ghidra.program.model.data.DataUtilities;
import ghidra.program.model.data.DataUtilities.ClearDataMode;
import ghidra.program.model.lang.Language;
import ghidra.program.model.lang.LanguageCompilerSpecPair;
import ghidra.program.model.lang.LanguageNotFoundException;
import ghidra.program.model.lang.Register;
import ghidra.program.model.lang.RegisterValue;
import ghidra.program.model.listing.ContextChangeException;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Program;
import ghidra.program.model.listing.ProgramContext;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryAccessException;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.reloc.Relocation.Status;
import ghidra.program.model.symbol.RefType;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolTable;
import ghidra.program.model.util.CodeUnitInsertionException;
import ghidra.util.task.TaskMonitor;
import hunk.*;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class AmigaHunkLoader extends AbstractLibrarySupportLoader {
	/** Executable-format identifier persisted on programs imported by this loader. */
	public static final String EXECUTABLE_FORMAT = "Amiga Hunk Executable";
	public static final int DEF_IMAGE_BASE = 0x21F000;
	static final LanguageCompilerSpecPair DEFAULT_68000_LANGUAGE =
			new LanguageCompilerSpecPair("68000:BE:32:default", "default");
	static final LanguageCompilerSpecPair MANX_68000_LANGUAGE =
			new LanguageCompilerSpecPair("68000:BE:32:MANX", "manx");

	static final String OPTION_NAME = "ImageBase";
	static final String NO_RETURN_DISCOVERY_ANALYZER = "Non-Returning Functions - Discovered";
	public static Address imageBase = null;

	static final String defsSegmName = "DEFS";
	static final String refsSegmName = "REFS";
	static final int defsSegmImageBaseOffset = 0x10000;
	static int refsLastIndex = 0;
	static int defsLastIndex = 0;

	@Override
	public String getName() {
		return EXECUTABLE_FORMAT;
	}
	
	public static int getImageBase(int offset) {
		return (int) (((imageBase != null) ? imageBase.getOffset() : DEF_IMAGE_BASE) + offset);
	}

	@Override
	public Collection<LoadSpec> findSupportedLoadSpecs(ByteProvider provider) {
		List<LoadSpec> loadSpecs = new ArrayList<>();
		try {
			if (HunkBlockFile.isHunkBlockFile(new BinaryReader(provider, false))) {
				boolean isManxOverlay = isManxOverlayExecutable(provider);
				loadSpecs.add(new LoadSpec(this, 0,
						isManxOverlay ? MANX_68000_LANGUAGE : DEFAULT_68000_LANGUAGE, true));
				// Hunk records carry no compiler identifier. The non-preferred
				// alternative lets analysts select the proven ABI for ordinary
				// (non-overlay) MANX output, or the generic ABI for an unusual
				// overlay executable.
				loadSpecs.add(new LoadSpec(this, 0,
						isManxOverlay ? DEFAULT_68000_LANGUAGE : MANX_68000_LANGUAGE, false));
			}
		} catch(Exception e) {
		}

		return loadSpecs;
	}

	/**
	 * The MANX ABI is selected only when the executable contains the structured
	 * flat MANX overlay metadata.  It is a file-format property, not a game or
	 * function fingerprint.  A later validation step still decides whether its
	 * trampoline references can safely be mapped.
	 */
	static boolean isManxOverlayExecutable(ByteProvider provider) throws Exception {
		BinaryReader reader = new BinaryReader(provider, false);
		HunkBlockType type = HunkBlockFile.peekType(reader);
		if (type != HunkBlockType.TYPE_LOADSEG) {
			return false;
		}
		HunkBlockFile hunkBlocks = new HunkBlockFile(reader, true);
		HunkLoadSegFile hunkFile = BinFmtHunk.loadSegFile(hunkBlocks, new MessageLog());
		return hunkFile != null && hunkFile.getManxOverlayTable() != null;
	}

	@Override
	protected void load(Program program, ImporterSettings importerSettings)  {
		refsLastIndex = 0;
		defsLastIndex = 0;
		disableUnsafeNoReturnDiscovery(program);
		
		FlatProgramAPI fpa = new FlatProgramAPI(program);
		Memory mem = program.getMemory();

		MessageLog log = importerSettings.log();
		ByteProvider provider = importerSettings.provider();
		TaskMonitor monitor = importerSettings.monitor();

		BinaryReader reader = new BinaryReader(provider, false);

		// executable
		HunkBlockType type = HunkBlockFile.peekType(reader);
        HunkBlockFile hbf = null;
        try {
            hbf = new HunkBlockFile(reader, type == HunkBlockType.TYPE_LOADSEG);
        } catch (HunkParseError e) {
			e.printStackTrace();
			log.appendMsg("Import stopped while parsing Hunk records: " + e.getMessage());
			log.appendException(e);
			return;
        }
        switch (type) {
		case TYPE_LOADSEG: 
		case TYPE_UNIT:
			try {
				loadExecutable(imageBase, type == HunkBlockType.TYPE_LOADSEG, hbf, fpa, monitor, mem, log);
			} catch (Throwable e) {
				e.printStackTrace();
				log.appendException(e);
			}
		break;
		case TYPE_LIB:
		break;
		default:
		break;
		}
	}

	private static void loadExecutable(Address imageBase, boolean isExecutable, HunkBlockFile hbf, FlatProgramAPI fpa, TaskMonitor monitor, Memory mem, MessageLog log) throws Throwable {
		HunkLoadSegFile hunkFile = BinFmtHunk.loadSegFile(hbf, log);
		if (hunkFile == null) {
			return;
		}

		BinImage bi = BinFmtHunk.createImage(hunkFile, log);
		
		if (bi == null) {
			return;
		}
		
		int _imageBase = getImageBase(0);

		Segment[] segments = bi.getSegments();
		int[] addrs = getNodeAddresses(hunkFile, segments, _imageBase);
		Relocate rel = new Relocate(bi);
		List<byte[]> datas;
		try {
			datas = rel.relocate(addrs);
		} catch (HunkParseError e1) {
			log.appendMsg("Import stopped while applying Hunk relocations: " + e1.getMessage());
			log.appendException(e1);
			return;
		}
		
		Address[] segmentAddresses = new Address[segments.length];
		int lastSectAddress = mapNodes(hunkFile, segments, datas, addrs, fpa, log, segmentAddresses);
		boolean manxA4ContextApplied = applyValidatedManxA4Context(hunkFile, segments, segmentAddresses, fpa, log);
		if (!manxA4ContextApplied) {
			applyValidatedSasCA4Context(hunkFile, segments, segmentAddresses, fpa, log);
		}
		lastSectAddress = mapOverlayMetadata(hunkFile, segments, fpa, log, segmentAddresses, lastSectAddress);

		for (Segment seg : segments) {
			relocateSegment(seg, segmentAddresses[seg.getId()], datas, addrs, segmentAddresses, mem, fpa, log);
		}
		
		for (Segment seg : segments) {
			applySegmentDefs(seg, segmentAddresses[seg.getId()], fpa, fpa.getCurrentProgram().getSymbolTable(), log, lastSectAddress);
		}
		
		HunkLoadSegFile.Node root = findRootNode(hunkFile);
		Segment startSegment = root == null ? null : findSegment(root, root.getFirstHunk(), segments);
		if (startSegment == null) {
			throw new HunkParseError("Root HUNK_HEADER has no physical entry segment");
		}
		Address startAddr = segmentAddresses[startSegment.getId()];
		
		var fdm = fpa.openDataTypeArchive(Application.getModuleDataFile("amiga_ndk39.gdt").getFile(false), true);
		AmigaUtils.createExecBaseSegment(fpa, fdm, log);
		AmigaUtils.createCustomSegment(fpa, fdm, log);
		AmigaUtils.addTypes(fpa.getCurrentProgram(), log);
		AmigaUtils.analyzeResident(mem, fpa, fdm, startAddr, log);
		
		if(isExecutable)
			AmigaUtils.setFunction(fpa, startAddr, "start", log);
		
		addSymbols(segments, fpa.getCurrentProgram().getSymbolTable(), segmentAddresses);
	}

	/**
	 * Ghidra's generic discovered-no-return analyzer treats a call followed by
	 * presently undefined bytes as evidence that the callee cannot return. That
	 * inference is unsound for Hunk executables: address-taken code and MANX A4
	 * forwarding calls are discovered after initial disassembly. A false result
	 * truncates callers by replacing their call fall-through with a terminator.
	 *
	 * <p>Disable only that heuristic before auto-analysis. Known no-return
	 * definitions and explicit user annotations remain available.</p>
	 */
	static void disableUnsafeNoReturnDiscovery(Program program) {
		Options analysisOptions = program.getOptions(Program.ANALYSIS_PROPERTIES);
		analysisOptions.setBoolean(NO_RETURN_DISCOVERY_ANALYZER, false);
	}

	/**
	 * Makes the otherwise file-only HUNK_OVERLAY payload visible to the analyst.
	 * The block is a static analysis representation, not a claim that AmigaDOS
	 * assigns this address at run time; LoadSeg supplies that pointer dynamically.
	 */
	private static int mapOverlayMetadata(HunkLoadSegFile hunkFile, Segment[] segments, FlatProgramAPI fpa,
			MessageLog log, Address[] segmentAddresses, int afterRoot) throws HunkParseError {
		byte[] tableData = hunkFile.getOverlayTableData();
		if (tableData == null) {
			return afterRoot;
		}

		int tableAddress = (afterRoot + 3) & ~3;
		MemoryBlock block = AmigaUtils.createSegment(new ByteArrayInputStream(tableData), fpa,
				"HUNK_OVERLAY_TABLE", tableAddress, tableData.length, false, false, log);
		if (block == null) {
			throw new HunkParseError("Failed to create HUNK_OVERLAY_TABLE block");
		}
		String tableComment = "Static copy of the HUNK_OVERLAY payload. AmigaDOS supplies its runtime pointer in the root OverlayHeader.";
		block.setComment(tableComment);

		SymbolTable symbols = fpa.getCurrentProgram().getSymbolTable();
		try {
			symbols.createLabel(block.getStart(), "HunkOverlayTable", SourceType.IMPORTED);
		} catch (Exception e) {
			log.appendException(e);
		}

		Map<Integer, HunkLoadSegFile.Node> nodesByHeaderOffset = new HashMap<>();
		for (HunkLoadSegFile.Node node : hunkFile.getNodes()) {
			if (node.isOverlay()) {
				nodesByHeaderOffset.put(node.getHeaderFileOffset(), node);
			}
		}

		HunkOverlayTable overlayTable = hunkFile.getOverlayTable();
		if (overlayTable != null) {
			int resolvedSymbols = mapHierarchicalOverlaySymbols(overlayTable, nodesByHeaderOffset, block.getStart(), segments, segmentAddresses, symbols, log);
			log.appendMsg(String.format("Mapped hierarchical HUNK_OVERLAY table: depth %d, %d externally reachable symbols.",
					overlayTable.getTreeDepth(), resolvedSymbols));
			if (resolvedSymbols != overlayTable.getSymbols().size()) {
				appendOverlayWarning(block, log, "Some hierarchical overlay targets failed validation and were not linked. " +
						"Inspect the table before relying on cross-overlay navigation.");
			}
			return tableAddress + tableData.length;
		}

		HunkManxOverlayTable manxTable = hunkFile.getManxOverlayTable();
		if (manxTable != null) {
			if (mapManxOverlaySymbols(manxTable, hunkFile.getNodes(), nodesByHeaderOffset, segments, segmentAddresses, symbols, fpa, log)) {
				log.appendMsg(String.format("Mapped MANX HUNK_OVERLAY table: %d independently loadable nodes.", manxTable.getNodes().size()));
			} else {
				appendOverlayWarning(block, log, "A MANX overlay table was recognised but did not validate. " +
						"Overlay nodes loaded, but no MANX cross-overlay targets were linked.");
			}
			return tableAddress + tableData.length;
		}

		appendOverlayWarning(block, log, "Overlay manager is unrecognised. Overlay nodes loaded, but cross-overlay calls were not resolved. " +
				"Inspect this HUNK_OVERLAY_TABLE before relying on cross-overlay navigation.");
		return tableAddress + tableData.length;
	}

	private static void appendOverlayWarning(MemoryBlock block, MessageLog log, String warning) {
		block.setComment(block.getComment() + "\n\nWARNING: " + warning);
		log.appendMsg("WARNING: " + warning);
	}

	private static int mapHierarchicalOverlaySymbols(HunkOverlayTable overlayTable,
			Map<Integer, HunkLoadSegFile.Node> nodesByHeaderOffset, Address tableStart,
			Segment[] segments, Address[] segmentAddresses, SymbolTable symbols, MessageLog log) {
		int symbolIndex = 0;
		int resolvedSymbols = 0;
		for (HunkOverlayTable.Symbol entry : overlayTable.getSymbols()) {
			Address entryAddress = tableStart.add((overlayTable.getTreeDepth() + symbolIndex * 8L) * 4);
			try {
				symbols.createLabel(entryAddress, String.format("HunkOverlaySymbol_%03d", symbolIndex), SourceType.IMPORTED);
			} catch (Exception e) {
				log.appendException(e);
			}

			HunkLoadSegFile.Node node = nodesByHeaderOffset.get(entry.getFilePosition());
			Segment targetSegment = node == null ? null : findSegment(node, entry.getSymbolSegment(), segments);
			if (node == null || node.getFirstHunk() != entry.getFirstSegment() || targetSegment == null ||
					entry.getSymbolOffset() >= targetSegment.getSize()) {
				log.appendMsg(String.format("HUNK_OVERLAY symbol %d does not match an overlay node; no target label created.", symbolIndex));
				symbolIndex++;
				continue;
			}

			Address target = segmentAddresses[targetSegment.getId()].add(entry.getSymbolOffset());
			try {
				symbols.createLabel(target, String.format("OverlayTarget_L%d_O%d_%03d", entry.getLevel(), entry.getOrdinate(), symbolIndex), SourceType.IMPORTED);
				resolvedSymbols++;
			} catch (Exception e) {
				log.appendException(e);
			}
			symbolIndex++;
		}
		return resolvedSymbols;
	}

	private static boolean mapManxOverlaySymbols(HunkManxOverlayTable table, HunkLoadSegFile.Node[] allNodes,
			Map<Integer, HunkLoadSegFile.Node> nodesByHeaderOffset, Segment[] segments, Address[] segmentAddresses,
			SymbolTable symbols, FlatProgramAPI fpa, MessageLog log) {
		HunkLoadSegFile.Node root = null;
		for (HunkLoadSegFile.Node node : allNodes) {
			if (!node.isOverlay()) {
				root = node;
				break;
			}
		}
		if (root == null || root.getFirstHunk() == root.getLastHunk()) {
			log.appendMsg("MANX HUNK_OVERLAY has no second root segment for its trampoline table.");
			return false;
		}

		Segment trampolineSegment = findSegment(root, root.getFirstHunk() + 1, segments);
		if (trampolineSegment == null) {
			log.appendMsg("MANX HUNK_OVERLAY has no physical trampoline segment.");
			return false;
		}
		Address trampolineBase = segmentAddresses[trampolineSegment.getId()];
		ReferenceManager references = fpa.getCurrentProgram().getReferenceManager();
		Integer nodeIndexBias = determineManxNodeIndexBias(table, nodesByHeaderOffset, segments,
				trampolineBase, fpa.getCurrentProgram().getMemory(), log);
		if (nodeIndexBias == null) {
			log.appendMsg("MANX HUNK_OVERLAY failed validation; metadata retained without target references.");
			return false;
		}
		for (int nodeIndex = 0; nodeIndex < table.getNodes().size(); nodeIndex++) {
			HunkManxOverlayTable.Node entry = table.getNodes().get(nodeIndex);
			HunkLoadSegFile.Node node = nodesByHeaderOffset.get(entry.getFilePosition());
			if (node == null) {
				log.appendMsg(String.format("MANX overlay node %d does not identify a HUNK_HEADER.", nodeIndex));
				continue;
			}

			try {
				symbols.createLabel(trampolineBase.add(entry.getTrampolineOffset()),
						String.format("ManxOverlayTrampolines_%02d", nodeIndex), SourceType.IMPORTED);
			} catch (Exception e) {
				log.appendException(e);
			}

			int trampolineNumber = 0;
			for (HunkManxOverlayTable.SegmentDescriptor descriptor : entry.getSegments()) {
				Segment targetSegment = findSegment(node, descriptor.getSegment(), segments);
				if (targetSegment == null) {
					log.appendMsg(String.format("MANX overlay node %d references segment %d outside its HUNK_HEADER range.", nodeIndex, descriptor.getSegment()));
					trampolineNumber += descriptor.getTrampolineCount();
					continue;
				}
				for (int i = 0; i < descriptor.getTrampolineCount(); i++, trampolineNumber++) {
					Address trampoline = trampolineBase.add(entry.getTrampolineOffset() + trampolineNumber * 8L);
					try {
						byte[] bytes = new byte[8];
						fpa.getCurrentProgram().getMemory().getBytes(trampoline, bytes);
						int opcode = Short.toUnsignedInt(ByteBuffer.wrap(bytes, 0, 2).order(ByteOrder.BIG_ENDIAN).getShort());
						int encodedNode = Byte.toUnsignedInt(bytes[4]);
						int symbolOffset = (Byte.toUnsignedInt(bytes[5]) << 16) |
								(Byte.toUnsignedInt(bytes[6]) << 8) | Byte.toUnsignedInt(bytes[7]);
						// MANX documentation specifies zero-based node IDs, but historical
						// linkers also emitted one-based IDs. The table record is authoritative;
							// accept either documented encoding after validating the trampoline.
						if (opcode != 0x6100 || encodedNode != nodeIndex + nodeIndexBias ||
								symbolOffset >= targetSegment.getSize()) {
							log.appendMsg(String.format("MANX trampoline %d for node %d failed format validation.", trampolineNumber, nodeIndex));
							continue;
						}
						Address target = segmentAddresses[targetSegment.getId()].add(symbolOffset);
						String targetName = String.format("ManxOverlayTarget_%02d_%03d", nodeIndex, trampolineNumber);
						Function targetFunction = fpa.getFunctionAt(target);
						if (targetFunction == null) {
							// A validated MANX trampoline denotes an externally callable routine.
							// Create an explicit function because a COMPUTED_CALL alone is not a
							// reliable auto-analysis seed, particularly across overlay spaces.
							AmigaUtils.setFunction(fpa, target, targetName, log);
						} else {
							symbols.createLabel(target, targetName, SourceType.IMPORTED);
						}
						// This is a metadata-resolved dynamic call. COMPUTED_CALL preserves the
						// manager trampoline's actual control flow while exposing the target.
						references.addMemoryReference(trampoline, target, RefType.COMPUTED_CALL, SourceType.IMPORTED, 0);
					} catch (Exception e) {
						log.appendException(e);
					}
				}
			}
		}
		return true;
	}

	/**
	 * MANX startup code establishes its small-data base through a short helper:
	 * {@code lea absolute.l,A4; rts}.  Do not infer a base merely from arbitrary
	 * A4 use: require the root entry's direct startup chain to call that helper.
	 * The base is intentionally not required to be mapped.  Aztec/Manx places it
	 * above the resident small-data area so negative 16-bit displacements address
	 * the globals below it.
	 */
	private static boolean applyValidatedManxA4Context(HunkLoadSegFile hunkFile, Segment[] segments,
			Address[] segmentAddresses, FlatProgramAPI fpa, MessageLog log) {
		Address a4Base = findManxA4Base(hunkFile, segments, segmentAddresses, fpa.getCurrentProgram());
		if (a4Base == null) {
			return false;
		}
		if (!applyA4Context(fpa.getCurrentProgram(), a4Base, "MANX", log)) {
			return false;
		}
		int stubs = mapManxA4CallStubs(hunkFile, segments, segmentAddresses, a4Base, fpa, log);
		log.appendMsg(String.format("Validated MANX startup A4 base at %s; applied context to executable root and overlay blocks.", a4Base));
		log.appendMsg(String.format("Mapped %d validated MANX A4 call stubs.", stubs));
		return true;
	}

	/**
	 * SAS/C 6.50's c.o startup owns a linker-defined A4-relative database named
	 * LinkerDB. Unlike MANX, it does not use A4 forwarding stubs. The exact CRT
	 * prologue is sufficient to establish the context in every executable block,
	 * including hierarchical-overlay nodes.
	 */
	private static void applyValidatedSasCA4Context(HunkLoadSegFile hunkFile, Segment[] segments,
			Address[] segmentAddresses, FlatProgramAPI fpa, MessageLog log) {
		HunkLoadSegFile.Node root = findRootNode(hunkFile);
		if (root == null) {
			return;
		}
		Address linkerDb = findSasCLinkerDb(hunkFile, segments, segmentAddresses, fpa.getCurrentProgram());
		if (linkerDb == null || !applyA4Context(fpa.getCurrentProgram(), linkerDb, "SAS/C", log)) {
			return;
		}
		try {
			AmigaUtils.applyAnalysisGlobalLabel(fpa.getCurrentProgram(), linkerDb, "SasCLinkerDB");
			log.appendMsg(String.format("Validated SAS/C c.o startup LinkerDB at %s; applied A4 context to executable root and overlay blocks.", linkerDb));
		}
		catch (Exception exception) {
			log.appendException(exception);
		}
	}

	/** Applies a compiler-validated A4 base to every executable Hunk block. */
	static boolean applyA4Context(Program program, Address a4Base, String profileName, MessageLog log) {
		Register a4 = program.getRegister("A4");
		if (a4 == null) {
			log.appendMsg(profileName + " A4 startup was recognized, but the selected language has no A4 register.");
			return false;
		}
		ProgramContext context = program.getProgramContext();
		RegisterValue value = new RegisterValue(a4, a4Base.getOffsetAsBigInteger());
		try {
			for (MemoryBlock block : program.getMemory().getBlocks()) {
				if (block.isExecute()) {
					context.setRegisterValue(block.getStart(), block.getEnd(), value);
				}
			}
			return true;
		}
		catch (ContextChangeException exception) {
			log.appendException(exception);
			return false;
		}
	}

	/**
	 * MANX emits calls through A4-relative jump stubs, often placing the stubs
	 * in a DATA hunk. A context value alone cannot make Ghidra discover code in
	 * that non-executable block, so only map the exact JSR(d16,A4) -> JMP(abs.l)
	 * form when the jump destination is resident root code.
	 */
	private static int mapManxA4CallStubs(HunkLoadSegFile hunkFile, Segment[] segments,
			Address[] segmentAddresses, Address a4Base, FlatProgramAPI fpa, MessageLog log) {
		HunkLoadSegFile.Node root = findRootNode(hunkFile);
		if (root == null) {
			return 0;
		}
		Memory memory = fpa.getCurrentProgram().getMemory();
		ReferenceManager references = fpa.getCurrentProgram().getReferenceManager();
		SymbolTable symbols = fpa.getCurrentProgram().getSymbolTable();
		int mapped = 0;
		for (Segment segment : segments) {
			if (segment.getType() != SegmentType.SEGMENT_TYPE_CODE) {
				continue;
			}
			Address callSite = segmentAddresses[segment.getId()];
			for (int offset = 0; offset + 4 <= segment.getSize(); offset += 2) {
				try {
					Address instruction = callSite.add(offset);
					if (Short.toUnsignedInt(memory.getShort(instruction)) != 0x4eac) { // jsr (d16,A4)
						continue;
					}
					Address stub = a4Base.add(memory.getShort(instruction.add(2)));
					if (Short.toUnsignedInt(memory.getShort(stub)) != 0x4ef9) { // jmp absolute.l
						continue;
					}
					Address destination = stub.getNewAddress(Integer.toUnsignedLong(memory.getInt(stub.add(2))));
					if (!isRootExecutableAddress(destination, segments, root, segmentAddresses)) {
						continue;
					}

					String name = String.format("ManxA4CallStub_%06X", stub.getOffset());
					if (fpa.getFunctionAt(stub) == null) {
						AmigaUtils.setFunction(fpa, stub, name, log);
					} else {
						symbols.createLabel(stub, name, SourceType.IMPORTED);
					}
					references.addMemoryReference(instruction, stub, RefType.COMPUTED_CALL, SourceType.IMPORTED, 0);
					references.addMemoryReference(stub, destination, RefType.UNCONDITIONAL_JUMP, SourceType.IMPORTED, 0);
					mapped++;
				} catch (Exception e) {
					log.appendException(e);
				}
			}
		}
		return mapped;
	}

	private static Address findManxA4Base(HunkLoadSegFile hunkFile, Segment[] segments, Address[] segmentAddresses,
			Program program) {
		HunkLoadSegFile.Node root = findRootNode(hunkFile);
		if (root == null) {
			return null;
		}
		for (Segment segment : getSegmentsForNode(root, segments)) {
			if (segment.getLogicalSlot() == root.getFirstHunk()) {
				return findManxA4Base(segmentAddresses[segment.getId()], program);
			}
		}
		return null;
	}

	/**
	 * Returns the MANX small-data anchor only when the root startup transfer
	 * chain calls the canonical A4 initializer.  Kept package-visible for the
	 * loader test because this is the compiler-identification boundary; it does
	 * not rely on an overlay table or an image-specific address.
	 */
	static Address findManxA4Base(Address rootEntry, Program program) {
		Address startup = rootEntry;
		for (int depth = 0; depth < 4; depth++) {
			Address a4Base = findCalledManxA4Initializer(startup, program);
			if (a4Base != null) {
				return a4Base;
			}
			startup = getDirectStartupTransfer(startup, program);
			if (startup == null) {
				break;
			}
		}
		return null;
	}

	/**
	 * Recognises the SAS/C 6.50 {@code c.o} startup prologue and returns its
	 * linker-defined A4 global-data base.  The signature is deliberately exact:
	 * register save, command-line preservation, A4 setup, and the conventional
	 * absolute ExecBase fetch must all occur in order.  This keeps SAS/C context
	 * recovery independent from both MANX overlays and generic A4 use.
	 */
	static Address findSasCLinkerDb(Address rootEntry, Program program) {
		try {
			Memory memory = program.getMemory();
			Address address = rootEntry;
			if (Short.toUnsignedInt(memory.getShort(address)) != 0x48e7 ||
					Short.toUnsignedInt(memory.getShort(address.add(2))) != 0x7efe) {
				return null;
			}
			address = address.add(4);
			if (Short.toUnsignedInt(memory.getShort(address)) != 0x2448 ||
					Short.toUnsignedInt(memory.getShort(address.add(2))) != 0x2400) {
				return null;
			}
			address = address.add(4);
			if (Short.toUnsignedInt(memory.getShort(address)) != 0x49f9) { // lea abs.l,A4
				return null;
			}
			Address linkerDb = address.getNewAddress(Integer.toUnsignedLong(memory.getInt(address.add(2))));
			address = address.add(6);
			if (Short.toUnsignedInt(memory.getShort(address)) != 0x2c78 ||
					Short.toUnsignedInt(memory.getShort(address.add(2))) != 4 ||
					!memory.contains(linkerDb)) {
				return null;
			}
			return linkerDb;
		}
		catch (MemoryAccessException exception) {
			return null;
		}
	}

	/**
	 * Finds the unique SAS/C CRT startup in the resident node. SLink places its
	 * overlay manager before c.o in the root node, so the executable entry hunk
	 * is not necessarily the CRT hunk. Overlay nodes are intentionally excluded:
	 * only the resident startup is allowed to establish the process-wide A4 base.
	 */
	static Address findSasCLinkerDb(HunkLoadSegFile hunkFile, Segment[] segments, Address[] segmentAddresses,
			Program program) {
		HunkLoadSegFile.Node root = findRootNode(hunkFile);
		if (root == null) {
			return null;
		}
		Address linkerDb = null;
		for (Segment segment : getSegmentsForNode(root, segments)) {
			if (segment.getType() != SegmentType.SEGMENT_TYPE_CODE ||
					program.getMemory().getBlock(segmentAddresses[segment.getId()]) == null) {
				continue;
			}
			Address candidate = findSasCLinkerDb(segmentAddresses[segment.getId()], program);
			if (candidate == null) {
				continue;
			}
			if (linkerDb != null && !linkerDb.equals(candidate)) {
				return null;
			}
			linkerDb = candidate;
		}
		return linkerDb;
	}

	private static Address findCalledManxA4Initializer(Address startup, Program program) {
		for (PseudoInstruction instruction : M68kControlFlow.decodeLinear(program, startup, 0x80)) {
			Address target = getDirectCallTarget(instruction);
			if (target == null) {
				continue;
			}
			Address base = getManxA4InitializerBase(target, program);
			if (base != null) {
				return base;
			}
		}
		return null;
	}

	private static Address getDirectStartupTransfer(Address startup, Program program) {
		PseudoInstruction instruction = M68kControlFlow.decodeOne(program, startup);
		return instruction != null && isMnemonic(instruction, "BRA", "JMP") ? getSingleFlowTarget(instruction) : null;
	}

	private static Address getDirectCallTarget(PseudoInstruction instruction) {
		return isMnemonic(instruction, "BSR", "JSR") ? getSingleFlowTarget(instruction) : null;
	}

	private static Address getManxA4InitializerBase(Address target, Program program) {
		PseudoInstruction initializer = M68kControlFlow.decodeOne(program, target);
		if (!isMnemonic(initializer, "LEA") || initializer.getNumOperands() != 2 || initializer.getRegister(1) == null ||
				!"A4".equalsIgnoreCase(initializer.getRegister(1).getName()) || initializer.getLength() != 6) {
			return null;
		}
		PseudoInstruction terminator = M68kControlFlow.decodeOne(program, initializer.getMaxAddress().add(1));
		if (!isMnemonic(terminator, "RTS")) {
			return null;
		}
		try {
			// PseudoInstruction does not expose an absolute 68000 LEA operand as
			// an Address, but its decoded six-byte form proves the following
			// four-byte extension is the absolute-long small-data anchor.
			return target.getNewAddress(Integer.toUnsignedLong(initializer.getInt(2)));
		}
		catch (MemoryAccessException exception) {
			return null;
		}
	}

	private static boolean isMnemonic(PseudoInstruction instruction, String... mnemonics) {
		if (instruction == null) {
			return false;
		}
		String actual = instruction.getMnemonicString();
		int sizeSuffix = actual.indexOf('.');
		if (sizeSuffix >= 0) {
			actual = actual.substring(0, sizeSuffix);
		}
		for (String mnemonic : mnemonics) {
			if (mnemonic.equalsIgnoreCase(actual)) {
				return true;
			}
		}
		return false;
	}

	private static Address getSingleFlowTarget(PseudoInstruction instruction) {
		Address[] flows = instruction.getFlows();
		return flows.length == 1 ? flows[0] : null;
	}

	/** The 68000 branch displacement is relative to the address after the opcode word. */
	static long getBranchTargetDelta(int opcode, short extension) {
		int displacement = opcode & 0xff;
		return 2L + (displacement == 0 ? extension : (byte) displacement);
	}

	private static boolean isRootExecutableAddress(Address address, Segment[] segments, HunkLoadSegFile.Node root,
			Address[] segmentAddresses) {
		for (Segment segment : getSegmentsForNode(root, segments)) {
			if (segment.getType() != SegmentType.SEGMENT_TYPE_CODE) {
				continue;
			}
			Address start = segmentAddresses[segment.getId()];
			if (address.getAddressSpace().equals(start.getAddressSpace()) && address.compareTo(start) >= 0 &&
					address.subtract(start) < segment.getSize()) {
				return true;
			}
		}
		return false;
	}

	private static HunkLoadSegFile.Node findRootNode(HunkLoadSegFile hunkFile) {
		for (HunkLoadSegFile.Node node : hunkFile.getNodes()) {
			if (!node.isOverlay()) {
				return node;
			}
		}
		return null;
	}

	private static Integer determineManxNodeIndexBias(HunkManxOverlayTable table,
			Map<Integer, HunkLoadSegFile.Node> nodesByHeaderOffset, Segment[] segments,
			Address trampolineBase, Memory memory, MessageLog log) {
		Integer bias = null;
		for (int nodeIndex = 0; nodeIndex < table.getNodes().size(); nodeIndex++) {
			HunkManxOverlayTable.Node entry = table.getNodes().get(nodeIndex);
			HunkLoadSegFile.Node node = nodesByHeaderOffset.get(entry.getFilePosition());
			if (node == null) {
				return null;
			}
			long trampolineNumber = 0;
			for (HunkManxOverlayTable.SegmentDescriptor descriptor : entry.getSegments()) {
				Segment targetSegment = findSegment(node, descriptor.getSegment(), segments);
				if (targetSegment == null) {
					return null;
				}
				for (int i = 0; i < descriptor.getTrampolineCount(); i++, trampolineNumber++) {
					try {
						Address trampoline = trampolineBase.add(entry.getTrampolineOffset() + trampolineNumber * 8);
						byte[] bytes = new byte[8];
						memory.getBytes(trampoline, bytes);
						int opcode = Short.toUnsignedInt(ByteBuffer.wrap(bytes, 0, 2).order(ByteOrder.BIG_ENDIAN).getShort());
						int encodedNode = Byte.toUnsignedInt(bytes[4]);
						int symbolOffset = (Byte.toUnsignedInt(bytes[5]) << 16) |
								(Byte.toUnsignedInt(bytes[6]) << 8) | Byte.toUnsignedInt(bytes[7]);
						int candidateBias = encodedNode - nodeIndex;
						if (opcode != 0x6100 || (candidateBias != 0 && candidateBias != 1) ||
								symbolOffset >= targetSegment.getSize()) {
							return null;
						}
						if (bias == null) {
							bias = candidateBias;
						} else if (bias != candidateBias) {
							return null;
						}
					} catch (MemoryAccessException e) {
						log.appendException(e);
						return null;
					}
				}
			}
		}
		return bias == null ? 0 : bias;
	}

	private static int[] getNodeAddresses(HunkLoadSegFile hunkFile, Segment[] segments, int imageBase) throws HunkParseError {
		int[] addresses = new int[segments.length];
		for (HunkLoadSegFile.Node node : hunkFile.getNodes()) {
			int address = imageBase;
			for (Segment segment : getSegmentsForNode(node, segments)) {
				addresses[segment.getId()] = address;
				address += segment.getSize();
			}
		}
		return addresses;
	}

	private static int mapNodes(HunkLoadSegFile hunkFile, Segment[] segments, List<byte[]> datas, int[] addresses, FlatProgramAPI fpa, MessageLog log, Address[] segmentAddresses) throws HunkParseError {
		int rootEnd = 0;
		int overlayNumber = 0;

		for (HunkLoadSegFile.Node node : hunkFile.getNodes()) {
			if (node.isOverlay()) {
				mapOverlayNode(node, ++overlayNumber, segments, datas, addresses, fpa, log, segmentAddresses);
				continue;
			}

			for (Segment segment : getSegmentsForNode(node, segments)) {
				int address = addresses[segment.getId()];
				byte[] data = datas.get(segment.getId());
				MemoryBlock block = AmigaUtils.createSegment(new ByteArrayInputStream(data), fpa, segment.getName(), address, segment.getSize(), segment.getType() == SegmentType.SEGMENT_TYPE_DATA, segment.getType() == SegmentType.SEGMENT_TYPE_CODE, log);
				if (block == null) {
					throw new HunkParseError("Failed to create root hunk memory block");
				}
				segmentAddresses[segment.getId()] = block.getStart();
				rootEnd = Math.max(rootEnd, address + segment.getSize());
			}
		}
		return rootEnd;
	}

	private static void mapOverlayNode(HunkLoadSegFile.Node node, int overlayNumber, Segment[] segments, List<byte[]> datas, int[] addresses, FlatProgramAPI fpa, MessageLog log, Address[] segmentAddresses) throws HunkParseError {
		ByteArrayOutputStream bytes = new ByteArrayOutputStream();
		boolean hasCode = false;
		boolean hasData = false;

		Segment[] nodeSegments = getSegmentsForNode(node, segments);
		if (nodeSegments.length == 0) {
			throw new HunkParseError("Overlay node has no physical segments");
		}
		for (Segment segment : nodeSegments) {
			bytes.writeBytes(datas.get(segment.getId()));
			hasCode |= segment.getType() == SegmentType.SEGMENT_TYPE_CODE;
			hasData |= segment.getType() == SegmentType.SEGMENT_TYPE_DATA;
		}

		MemoryBlock block = AmigaUtils.createSegment(new ByteArrayInputStream(bytes.toByteArray()), fpa,
				String.format("OVERLAY_%02d", overlayNumber), addresses[nodeSegments[0].getId()], bytes.size(), hasData,
				hasCode, log, true);
		if (block == null) {
			throw new HunkParseError("Failed to create overlay hunk memory block");
		}

		for (Segment segment : nodeSegments) {
			segmentAddresses[segment.getId()] = block.getStart().add(
					addresses[segment.getId()] - addresses[nodeSegments[0].getId()]);
		}
	}

	private static Segment[] getSegmentsForNode(HunkLoadSegFile.Node node, Segment[] segments) {
		return java.util.Arrays.stream(segments).filter(segment -> segment.getNode() == node).toArray(Segment[]::new);
	}

	private static Segment findSegment(HunkLoadSegFile.Node node, int logicalSlot, Segment[] segments) {
		for (Segment segment : segments) {
			if (segment.getNode() == node && segment.getLogicalSlot() == logicalSlot) {
				return segment;
			}
		}
		return null;
	}

	private static void addSymbols(Segment segs[], SymbolTable st, Address[] addrs) throws Throwable {
		for (Segment seg : segs) {
			hunk.Symbol[] symbols = seg.getSymbols(seg);
			if(symbols.length > 0) {
				for(hunk.Symbol symbol : symbols) {
					String name = symbol.getName();
					int offset = symbol.getOffset();
					st.createLabel(addrs[seg.getId()].add(offset), name, SourceType.IMPORTED);
				}
			}
		}
	}

	private static void relocateSegment(Segment seg, Address segAddress, final List<byte[]> datas,
			int[] runtimeSegmentAddresses, Address[] mappedSegmentAddresses, Memory mem,
			FlatProgramAPI fpa, MessageLog log) {
		Segment[] toSegs = seg.getRelocationsToSegments();
		ReferenceManager referenceManager = fpa.getCurrentProgram().getReferenceManager();

		for (Segment toSeg : toSegs) {
			Reloc[] reloc = seg.getRelocations(toSeg);

			for (Reloc r : reloc) {
				int dataOffset = r.getOffset();

				ByteBuffer buf = ByteBuffer.wrap(datas.get(seg.getId()));
				int newAddr = 0;
				
				try {
					switch (r.getWidth()) {
					case 4:
						newAddr = buf.getInt(dataOffset) + r.getAddend();
						break;
					case 2:
						newAddr = buf.getShort(dataOffset) + r.getAddend();
						break;
					case 1:
						newAddr = buf.get(dataOffset) + r.getAddend();
						break;
					}
					patchReference(mem, segAddress.add(dataOffset), newAddr, r.getWidth());
					if (r.getWidth() == 4 && r.getKind() == Reloc.Kind.ABSOLUTE) {
						Address relocationAddress = segAddress.add(dataOffset);
						long targetOffset = Integer.toUnsignedLong(newAddr) - Integer.toUnsignedLong(runtimeSegmentAddresses[toSeg.getId()]);
						// Preserve the Hunk target base and signed field addend as Ghidra's
						// native offset-reference pair.  This remains valid when the addend
						// addresses LoadSeg state immediately before the mapped segment.
						referenceManager.addOffsetMemReference(relocationAddress,
								mappedSegmentAddresses[toSeg.getId()], true, targetOffset,
								RefType.DATA, SourceType.IMPORTED, 0);
						fpa.getCurrentProgram().getRelocationTable().add(relocationAddress, Status.APPLIED,
								r.getWidth(), new long[] { Integer.toUnsignedLong(newAddr), toSeg.getId(),
										targetOffset }, null, null);
					}
				} catch (MemoryAccessException | CodeUnitInsertionException e) {
					log.appendException(e);
					return;
				}
			}
		}
	}
	
	private static void applySegmentDefs(Segment seg, Address segAddress, FlatProgramAPI fpa, SymbolTable st, MessageLog log, int lastSectAddress) throws Throwable {
		if (seg.getSegmentInfo().getDefinitions() == null) {
			return;
		}
		
		Memory mem = fpa.getCurrentProgram().getMemory();
		
		for (final XDefinition entry : seg.getSegmentInfo().getDefinitions()) {
			Address defAddr = fpa.toAddr(entry.getOffset());
			
			if (!entry.isAbsolute()) {
				defAddr = segAddress.add(entry.getOffset());
			}
			
			if (mem.contains(defAddr)) {
				st.createLabel(defAddr, entry.getName(), SourceType.USER_DEFINED);
				
				if (entry.getName().equals("___startup")) {
					AmigaUtils.setFunction(fpa, defAddr, entry.getName(), log);
				}
			} else {
				addDefinition(mem, fpa, st, entry.getName(), entry.getOffset());
			}
		}
		
		if (seg.getSegmentInfo().getReferences() == null) {
			return;
		}
		
		for (final XReference entry : seg.getSegmentInfo().getReferences()) {
			for (Integer offset : entry.getOffsets()) {
				Address fromAddr = segAddress.add(offset);
				int newAddr = 0;
				
				switch (entry.getType()) {
				case R_ABS: {
					newAddr = addReference(mem, fpa, st, entry.getName(), lastSectAddress);
					patchReference(mem, fromAddr, newAddr, entry.getWidth());
				} break;
				case R_SD: {
					newAddr = addReference(mem, fpa, st, entry.getName(), lastSectAddress);
					patchReference(mem, fromAddr, (int) (newAddr - lastSectAddress), entry.getWidth());
				} break;
				case R_PC: {
					newAddr = addReference(mem, fpa, st, entry.getName(), lastSectAddress);
					patchReference(mem, fromAddr, (int) (newAddr - fromAddr.getOffset()), entry.getWidth());
				} break;
				}
				
			}
		}
	}
	
	private static void patchReference(Memory mem, Address fromAddr, int toAddr, int width) throws MemoryAccessException, CodeUnitInsertionException {
		InstructionStasher instructionStasher = new InstructionStasher(mem.getProgram(), fromAddr);
		switch (width) {
		case 4:
			mem.setBytes(fromAddr, intToBytes(toAddr));
			break;
		case 2:
			mem.setBytes(fromAddr, shortToBytes((short) toAddr));
			break;
		case 1:
			mem.setBytes(fromAddr, new byte[] {(byte) toAddr});
			break;
		}
		instructionStasher.restore();
	}

	private static int addReference(Memory mem, FlatProgramAPI fpa, SymbolTable st, String name, int lastSectAddress) throws Throwable {
		List<Symbol> syms = st.getGlobalSymbols(name);
		
		if (syms.size() > 0) {
			return (int) syms.get(0).getAddress().getOffset();
		}
		
		MemoryBlock block = mem.getBlock(refsSegmName);
		
		if (block == null) {
			int transId = mem.getProgram().startTransaction(String.format("Create %s block", refsSegmName));
			block = mem.createUninitializedBlock(refsSegmName, fpa.toAddr(lastSectAddress), 4, false);
			mem.getProgram().endTransaction(transId, true);
		}
		
		Address newAddress = block.getStart().add(refsLastIndex * 4);
		expandBlockByDword(mem, block, newAddress, false);
		
		st.createLabel(newAddress, name, SourceType.IMPORTED);
		refsLastIndex++;
		
		return (int) newAddress.getOffset();
	}
	
	private static int addDefinition(Memory mem, FlatProgramAPI fpa, SymbolTable st, String name, int value) throws Throwable {
		List<Symbol> syms = st.getGlobalSymbols(name);
		
		if (syms.size() > 0) {
			return (int) syms.get(0).getAddress().getOffset();
		}
		
		MemoryBlock block = mem.getBlock(defsSegmName);

		if (block == null) {
			int transId = mem.getProgram().startTransaction(String.format("Create %s block", defsSegmName));
			block = mem.createInitializedBlock(defsSegmName, fpa.toAddr(getImageBase(defsSegmImageBaseOffset)), 4, (byte) 0x00, TaskMonitor.DUMMY, false);
			mem.getProgram().endTransaction(transId, true);
		}
		
		Address newAddress = block.getStart().add(defsLastIndex * 4);
		expandBlockByDword(mem, block, newAddress, true);
		
		st.createLabel(newAddress, name, SourceType.USER_DEFINED);
		mem.setInt(newAddress, value);
		DataUtilities.createData(mem.getProgram(), newAddress, DWordDataType.dataType, -1, true, ClearDataMode.CLEAR_ALL_UNDEFINED_CONFLICT_DATA);
		defsLastIndex++;
		
		return (int) newAddress.getOffset();
	}
	
	private static void expandBlockByDword(Memory mem, MemoryBlock block, Address appendAddress, boolean initialized) throws Throwable {
		if (block.getStart().equals(appendAddress)) {
			return;
		}
		
		int transId = mem.getProgram().startTransaction(String.format("Expand %s block", block.getName()));
		MemoryBlock tmp = mem.createUninitializedBlock(block.getName() + ".exp", appendAddress, 4, false);
		mem.getProgram().endTransaction(transId, true);
		
		if (initialized) {
			tmp = mem.convertToInitialized(tmp, (byte)0x00);
		}
		
		mem.join(block, tmp);
	}

	private static byte[] intToBytes(int x) {
		ByteBuffer buffer = ByteBuffer.allocate(Integer.BYTES);
		buffer.order(ByteOrder.BIG_ENDIAN);
		buffer.putInt(x);
		return buffer.array();
	}
	
	private static byte[] shortToBytes(short x) {
		ByteBuffer buffer = ByteBuffer.allocate(Short.BYTES);
		buffer.order(ByteOrder.BIG_ENDIAN);
		buffer.putShort(x);
		return buffer.array();
	}

	@Override
	public List<Option> getDefaultOptions(ByteProvider provider, LoadSpec loadSpec, DomainObject domainObject, boolean loadIntoProgram, boolean mirrorFsLayout) {
		List<Option> list = new ArrayList<Option>();

		LanguageCompilerSpecPair pair = loadSpec.getLanguageCompilerSpec();
		try {
			Language importerLanguage = getLanguageService().getLanguage(pair.languageID);
			imageBase = importerLanguage.getAddressFactory().getDefaultAddressSpace().getAddress(DEF_IMAGE_BASE);
			list.add(new Option(OPTION_NAME, imageBase, Address.class, Loader.COMMAND_LINE_ARG_PREFIX + "-baseAddr"));
		} catch (LanguageNotFoundException e) {

		}

		return list;
	}

	@Override
	public String validateOptions(ByteProvider provider, LoadSpec loadSpec, List<Option> options, Program program) {
		imageBase = null;

		for (Option option : options) {
			String optName = option.getName();
			try {
				if (optName.equals(OPTION_NAME)) {
					imageBase = (Address) option.getValue();

					long val = imageBase.getOffset();
					if (val >= 0x1000L && val <= 0x700000L) {
						break;
					}
				}
			} catch (Exception e) {
				if (e instanceof OptionException) {
					return e.getMessage();
				}
				return "Invalid value for " + optName + " - " + option.getValue();
			}
		}
		if (imageBase == null || (imageBase.getOffset() < 0x1000L) || (imageBase.getOffset() >= 0x80000000L)) {
			return "Invalid image base";
		}

		return null;
	}
}
