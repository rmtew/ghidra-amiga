package amiga;

import ghidra.app.util.PseudoDisassembler;
import ghidra.app.util.PseudoDisassemblerContext;
import ghidra.app.util.PseudoFlowProcessor;
import ghidra.app.util.PseudoInstruction;
import ghidra.program.model.address.Address;
import ghidra.program.model.lang.RegisterValue;
import ghidra.program.model.listing.Program;
import ghidra.util.exception.CancelledException;
import ghidra.util.task.TaskMonitor;

import java.util.ArrayList;
import java.util.List;

/** Small, listing-independent 68000 control-flow checks shared by analyzers. */
final class M68kControlFlow {

	private static final int MAX_PSEUDO_INSTRUCTIONS = 512;

	private M68kControlFlow() {
	}

	static boolean hasReachableReturn(Program program, Address target, TaskMonitor monitor)
			throws CancelledException {
		monitor.checkCancelled();
		PseudoDisassembler disassembler = new PseudoDisassembler(program);
		disassembler.setMaxInstructions(MAX_PSEUDO_INSTRUCTIONS);
		PseudoDisassemblerContext context = initialContext(program, target);
		try {
			PseudoInstruction entry = disassembler.disassemble(target, context, true);
			ReturnFindingFlowProcessor flowProcessor = new ReturnFindingFlowProcessor();
			disassembler.followSubFlows(entry.getAddress(), context, MAX_PSEUDO_INSTRUCTIONS, flowProcessor);
			monitor.checkCancelled();
			return flowProcessor.foundReturn;
		}
		catch (Exception exception) {
			return false;
		}
	}

	/**
	 * Decodes a bounded linear region without adding instructions to the program
	 * listing. Callers can therefore inspect genuine instruction boundaries during
	 * loading, before auto-analysis owns the listing.
	 */
	static List<PseudoInstruction> decodeLinear(Program program, Address start, int maximumBytes) {
		List<PseudoInstruction> instructions = new ArrayList<>();
		PseudoDisassembler disassembler = new PseudoDisassembler(program);
		disassembler.setMaxInstructions(MAX_PSEUDO_INSTRUCTIONS);
		PseudoDisassemblerContext context = initialContext(program, start);
		Address end;
		try {
			end = start.add(maximumBytes);
		}
		catch (Exception exception) {
			return instructions;
		}

		Address current = start;
		while (current.compareTo(end) < 0 && instructions.size() < MAX_PSEUDO_INSTRUCTIONS) {
			try {
				PseudoInstruction instruction = disassembler.disassemble(current, context, true);
				if (instruction == null || instruction.getMaxAddress().compareTo(current) < 0) {
					break;
				}
				instructions.add(instruction);
				current = instruction.getMaxAddress().add(1);
			}
			catch (Exception exception) {
				break;
			}
		}
		return instructions;
	}

	static PseudoInstruction decodeOne(Program program, Address address) {
		List<PseudoInstruction> instructions = decodeLinear(program, address, 2);
		return instructions.isEmpty() ? null : instructions.get(0);
	}

	private static PseudoDisassemblerContext initialContext(Program program, Address address) {
		PseudoDisassemblerContext context = new PseudoDisassemblerContext(program.getProgramContext());
		RegisterValue programContext = program.getProgramContext().getDisassemblyContext(address);
		if (programContext != null) {
			context.setValue(programContext.getRegister(), address, programContext.getUnsignedValueIgnoreMask());
		}
		return context;
	}

	static boolean isReturnMnemonic(String mnemonic) {
		return "RTS".equalsIgnoreCase(mnemonic) || "RTE".equalsIgnoreCase(mnemonic);
	}

	private static class ReturnFindingFlowProcessor implements PseudoFlowProcessor {
		private boolean foundReturn;

		@Override
		public boolean process(PseudoInstruction instruction) {
			if (instruction != null && isReturnMnemonic(instruction.getMnemonicString())) {
				foundReturn = true;
			}
			return !foundReturn;
		}

		@Override
		public boolean followFlows(PseudoInstruction instruction) {
			return !foundReturn;
		}
	}
}
