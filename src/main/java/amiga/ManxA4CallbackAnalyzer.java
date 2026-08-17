package amiga;

import java.util.Locale;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.app.services.AbstractAnalyzer;
import ghidra.app.services.AnalysisPriority;
import ghidra.app.services.AnalyzerType;
import ghidra.app.util.importer.MessageLog;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.listing.Program;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryAccessException;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.SourceType;
import ghidra.util.exception.CancelledException;
import ghidra.util.exception.DuplicateNameException;
import ghidra.util.exception.InvalidInputException;
import ghidra.util.task.TaskMonitor;

/**
 * Discovers address-taken MANX callbacks installed into A4-relative globals.
 *
 * <p>The MANX startup code can initialise callback slots with the pair
 * {@code lea target(PC),Ax; move.l Ax,(d16,A4)}. Ghidra correctly records the
 * data reference, but cannot safely infer that the referenced bytes are a
 * function. This analyser promotes only targets in executable memory that
 * begin with a 68000 LINK function prologue; strings and arbitrary data
 * referenced by the same initializer remain data.</p>
 */
public class ManxA4CallbackAnalyzer extends AbstractAnalyzer {

	private static final String MANX_COMPILER_SPEC_ID = "manx";
	private static final Pattern PC_LEA = Pattern.compile("^LEA.*,PC\\),A([0-7])$");
	private static final Pattern A4_STORE = Pattern.compile("^MOVE\\.LA([0-7]),\\((-?0X[0-9A-F]+),A4\\)$");

	public ManxA4CallbackAnalyzer() {
		super("MANX A4 Callbacks", "Discovers address-taken functions stored in MANX A4 callback slots",
				AnalyzerType.INSTRUCTION_ANALYZER);
		setPriority(AnalysisPriority.FUNCTION_ANALYSIS.after());
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
		InstructionIterator instructions = listing.getInstructions(set, true);
		while (instructions.hasNext()) {
			monitor.checkCancelled();
			Instruction lea = instructions.next();
			Instruction store = lea.getNext();
			int slot = callbackSlot(lea.toString(), store == null ? null : store.toString());
			if (slot == Integer.MIN_VALUE) {
				continue;
			}
			Address target = getCodeTarget(lea, program);
			if (target == null || !hasLinkPrologue(program.getMemory(), target)) {
				continue;
			}
			createCallbackFunction(program, target, slot, lea.getAddress(), monitor, log);
		}
		return true;
	}

	static int callbackSlot(String leaText, String storeText) {
		if (leaText == null || storeText == null) {
			return Integer.MIN_VALUE;
		}
		Matcher lea = PC_LEA.matcher(normalize(leaText));
		Matcher store = A4_STORE.matcher(normalize(storeText));
		if (!lea.matches() || !store.matches() || !lea.group(1).equals(store.group(1))) {
			return Integer.MIN_VALUE;
		}
		return Integer.decode(store.group(2));
	}

	static boolean hasLinkPrologue(byte firstByte, byte secondByte) {
		return (firstByte & 0xff) == 0x4e && (secondByte & 0xf8) == 0x50;
	}

	private static String normalize(String text) {
		return text.replace(" ", "").toUpperCase(Locale.ROOT);
	}

	private static Address getCodeTarget(Instruction instruction, Program program) {
		for (Reference reference : instruction.getReferencesFrom()) {
			Address target = reference.getToAddress();
			MemoryBlock block = program.getMemory().getBlock(target);
			if (block != null && block.isExecute()) {
				return target;
			}
		}
		return null;
	}

	private static boolean hasLinkPrologue(Memory memory, Address target) {
		try {
			return hasLinkPrologue(memory.getByte(target), memory.getByte(target.add(1)));
		} catch (MemoryAccessException e) {
			return false;
		}
	}

	private static void createCallbackFunction(Program program, Address target, int slot, Address initializer,
			TaskMonitor monitor, MessageLog log) throws CancelledException {
		Function function = program.getFunctionManager().getFunctionAt(target);
		if (function == null) {
			DisassembleCommand disassemble = new DisassembleCommand(target, null, true);
			if (!disassemble.applyTo(program, monitor)) {
				return;
			}
			CreateFunctionCmd createFunction = new CreateFunctionCmd(target);
			if (!createFunction.applyTo(program, monitor)) {
				return;
			}
			function = createFunction.getFunction();
		}
		if (function == null || !hasDefaultName(function)) {
			return;
		}
		String name = String.format("ManxA4Callback_%04X", slot & 0xffff);
		try {
			function.setName(name, SourceType.ANALYSIS);
			if (function.getComment() == null || function.getComment().isBlank()) {
				function.setComment(String.format(
						"Address-taken MANX callback installed in A4 slot 0x%04x by %s; no static caller is implied.",
						slot & 0xffff, initializer));
			}
			log.appendMsg(String.format("Recognised MANX A4 callback %s at %s.", name, target));
		} catch (DuplicateNameException | InvalidInputException e) {
			log.appendException(e);
		}
	}

	private static boolean hasDefaultName(Function function) {
		return function.getSymbol().getSource() == SourceType.DEFAULT || function.getName().startsWith("FUN_");
	}
}
