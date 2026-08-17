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
		PseudoDisassemblerContext context = new PseudoDisassemblerContext(program.getProgramContext());
		RegisterValue programContext = program.getProgramContext().getDisassemblyContext(target);
		if (programContext != null) {
			context.setValue(programContext.getRegister(), target, programContext.getUnsignedValueIgnoreMask());
		}
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
