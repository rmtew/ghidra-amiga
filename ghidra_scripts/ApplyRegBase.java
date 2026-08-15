//This script applies a selected A4 register value to the function at the cursor.
//@author Dr. MefistO
//@category Amiga
//@keybinding
//@menupath
//@toolbar

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.lang.Register;
import ghidra.program.model.lang.RegisterValue;
import ghidra.program.model.listing.ContextChangeException;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.ProgramContext;

public class ApplyRegBase extends GhidraScript {
	private static final String REG = "A4";

	@Override
	protected void run() throws Exception {
		Register reg = this.currentProgram.getRegister(REG);
		if (reg == null) {
			printerr("The current language does not define register " + REG + ".");
			return;
		}

		Function function = this.currentProgram.getFunctionManager().getFunctionContaining(currentAddress);
		if (function == null) {
			printerr("Place the cursor in the function whose A4 context is known.");
			return;
		}

		Address addr = this.askAddress(String.format("%s register base", REG), String.format("Specify %s register base address:", REG));
		doAnalysis(function, reg, addr);
		analyzeChanges(currentProgram);
	}

	private void doAnalysis(Function func, Register reg, Address addr) {
		if(func == null) {
			println("No function to analyze.");
			return;
		}

		try {
			monitor.setMessage(String.format("Analyzing %s reg usage in %s", REG, func.getName()));

			ProgramContext ctx = this.getCurrentProgram().getProgramContext();
			ctx.setRegisterValue(func.getBody().getMinAddress(), func.getBody().getMaxAddress(), new RegisterValue(reg, addr.getOffsetAsBigInteger()));
		} catch(ContextChangeException e1) {
			printerr("Unable to set " + REG + " context: " + e1.getMessage());
		}
	}
}
