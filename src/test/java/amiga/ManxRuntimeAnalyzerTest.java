package amiga;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.io.File;

import org.junit.BeforeClass;
import org.junit.Test;

import ghidra.GhidraApplicationLayout;
import ghidra.app.util.importer.MessageLog;
import ghidra.framework.Application;
import ghidra.framework.ApplicationConfiguration;
import ghidra.program.database.ProgramBuilder;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.Program;
import ghidra.program.model.symbol.SourceType;
import ghidra.util.task.TaskMonitor;

public class ManxRuntimeAnalyzerTest {

	@BeforeClass
	public static void initializeGhidra() throws Exception {
		if (!Application.isInitialized()) {
			Application.initializeApplication(
					new GhidraApplicationLayout(new File(System.getProperty("ghidra.install.dir"))),
					new ApplicationConfiguration());
		}
	}

	@Test
	public void identifiesTheModuloReturnMoveFromOperandsRatherThanDisplayText() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("manx-operands", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 8);
			builder.setBytes("0", "20011001"); // move.l D1,D0; move.b D1,D0
			builder.disassemble("0", 4);
			Instruction moveLong = builder.getProgram().getListing().getInstructionAt(builder.addr("0"));
			Instruction moveByte = builder.getProgram().getListing().getInstructionAt(builder.addr("2"));
			assertTrue(ManxRuntimeAnalyzer.isMoveLongD1ToD0(moveLong));
			assertFalse(ManxRuntimeAnalyzer.isMoveLongD1ToD0(moveByte));
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void recognisesTheAztecCMainStartupFromIndependentRuntimeSignals() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("manx-startup", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x400);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);

			String[] apiNames = { "exec_library_AllocMem", "exec_library_Alert", "exec_library_FindTask",
					"exec_library_WaitPort", "exec_library_GetMsg", "dos_library_CurrentDir", "dos_library_Input",
					"dos_library_Output", "dos_library_Open" };
			Function[] apiFunctions = new Function[apiNames.length];
			StringBuilder startup = new StringBuilder("203c4d414e58"); // move.l #MANX,D0
			for (int index = 0; index < apiNames.length; index++) {
				int target = 0x200 + index * 0x10;
				startup.append(String.format("4eb90000%04x", target)); // jsr absolute.l target
				builder.setBytes(String.format("0x%x", target), "4e75");
				builder.disassemble(String.format("0x%x", target), 2);
				apiFunctions[index] = builder.createFunction(String.format("0x%x", target));
			}
			startup.append("4e75");
			builder.setBytes("0x100", startup.toString());
			builder.disassemble("0x100", startup.length() / 2);
			Function crtMain = builder.createFunction("0x100");
			builder.setBytes("0x180", startup.toString());
			builder.disassemble("0x180", startup.length() / 2);
			Function analystNamedStartup = builder.createFunction("0x180");

			Program program = builder.getProgram();
			ManxRuntimeAnalyzer analyzer = new ManxRuntimeAnalyzer();
			boolean[] added = new boolean[1];
			MessageLog log = new MessageLog();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				try {
					for (int index = 0; index < apiNames.length; index++) {
						apiFunctions[index].setName(apiNames[index], SourceType.ANALYSIS);
					}
					analystNamedStartup.setName("FUN_analyst_review", SourceType.USER_DEFINED);
					added[0] = analyzer.added(program, new AddressSet(crtMain.getBody()), TaskMonitor.DUMMY, log);
				}
				catch (Exception e) {
					throw new AssertionError(e);
				}
			});

			assertTrue(log.toString(), added[0]);
			Function recognised = program.getFunctionManager().getFunctionAt(builder.addr("0x100"));
			assertEquals("_main", recognised.getName());
			assertEquals(2, recognised.getParameterCount());
			assertEquals("commandLineLength", recognised.getParameter(0).getName());
			assertEquals("commandLine", recognised.getParameter(1).getName());
			assertEquals("void", recognised.getReturnType().getDisplayName());
			assertEquals("FUN_analyst_review",
					program.getFunctionManager().getFunctionAt(builder.addr("0x180")).getName());
		}
		finally {
			builder.dispose();
		}
	}
}
