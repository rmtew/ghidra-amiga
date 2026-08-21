package amiga;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import java.io.File;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;

import org.junit.BeforeClass;
import org.junit.Test;

import ghidra.GhidraApplicationLayout;
import ghidra.app.util.importer.MessageLog;
import ghidra.framework.Application;
import ghidra.framework.ApplicationConfiguration;
import ghidra.framework.options.ToolOptions;
import ghidra.program.database.ProgramBuilder;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.data.DataUtilities;
import ghidra.program.model.data.DataUtilities.ClearDataMode;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.Program;
import ghidra.program.model.lang.OperandType;
import ghidra.program.model.lang.Register;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.RefType;
import ghidra.program.model.symbol.SourceType;
import ghidra.util.task.TaskMonitor;

public class AmigaHunkAnalyzerTest {

	@BeforeClass
	public static void initializeGhidra() throws Exception {
		if (!Application.isInitialized()) {
			Application.initializeApplication(
					new GhidraApplicationLayout(new File(System.getProperty("ghidra.install.dir"))),
					new ApplicationConfiguration());
		}
	}

	@Test
	public void retainsOnlyAgreedApiBasesAtControlFlowMerge() {
		AmigaHunkAnalyzer.ApiBaseState left = new AmigaHunkAnalyzer.ApiBaseState();
		left.put("A6", "graphics_library");
		left.put("A5", "dos_library");

		AmigaHunkAnalyzer.ApiBaseState right = new AmigaHunkAnalyzer.ApiBaseState();
		right.put("A6", "graphics_library");
		right.put("A5", "intuition_library");

		AmigaHunkAnalyzer.ApiBaseState merged = left.intersection(right);
		assertEquals("graphics_library", merged.get("A6"));
		assertNull(merged.get("A5"));
	}

	@Test
	public void unknownPathRemovesApiBaseAtControlFlowMerge() {
		AmigaHunkAnalyzer.ApiBaseState known = new AmigaHunkAnalyzer.ApiBaseState();
		known.put("A6", "dos_library");

		AmigaHunkAnalyzer.ApiBaseState merged = known.intersection(new AmigaHunkAnalyzer.ApiBaseState());
		assertNull(merged.get("A6"));
	}

	@Test
	public void resolvesAnApiVectorThroughAnOpenerAssignedGlobal() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("api-base", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x200);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			builder.setBytes("0x100",
					"2c78000443fa003870004eaefdd8298000802c6c00804eaeffb24e75");
			builder.createString("0x140", "icon.library");
			builder.disassemble("0x100", 0x1c);
			builder.createMemoryReadReference("0x100", "0x4");
			builder.createMemoryReference("0x104", "0x140", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReference("0x10e", "0x80", RefType.WRITE, SourceType.ANALYSIS, 1);
			builder.createMemoryReadReference("0x112", "0x80");
			Function function = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			boolean[] added = new boolean[1];
			MessageLog log = new MessageLog();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				try {
					// Simulate analysis made by an older extension version.
					program.getSymbolTable().createLabel(builder.addr("0x4"), "g_ExecLibraryBase", SourceType.ANALYSIS);
					assertTrue(analyzer.canAnalyze(program));
					added[0] = analyzer.added(program, new AddressSet(function.getBody()), TaskMonitor.DUMMY, log);
					int functionsAfterFirstAnalysis = program.getFunctionManager().getFunctionCount();
					int referencesAfterFirstAnalysis = getAnalysisReferenceCount(
							program.getListing().getInstructionAt(builder.addr("0x116")));
					assertTrue(analyzer.added(program, new AddressSet(function.getBody()), TaskMonitor.DUMMY,
							new MessageLog()));
					assertEquals(functionsAfterFirstAnalysis, program.getFunctionManager().getFunctionCount());
					assertEquals(referencesAfterFirstAnalysis, getAnalysisReferenceCount(
							program.getListing().getInstructionAt(builder.addr("0x116"))));
				}
				catch (Exception e) {
					throw new AssertionError(e);
				}
			});
			assertTrue(log.toString(), added[0]);

			boolean resolved = false;
			StringBuilder references = new StringBuilder();
			for (Reference reference : program.getListing().getInstructionAt(builder.addr("0x116")).getReferencesFrom()) {
				references.append(reference).append(';');
				if ("icon_library_GetDiskObject".equals(
						program.getFunctionManager().getFunctionAt(reference.getToAddress()).getName())) {
					resolved = true;
				}
			}
			assertTrue("the icon vector should resolve through the opener-assigned base at " +
					program.getListing().getInstructionAt(builder.addr("0x116")) + ": " + references, resolved);
			assertEquals("SysBase", program.getSymbolTable().getPrimarySymbol(builder.addr("0x4")).getName());
			assertEquals("g_IconLibraryBase", program.getSymbolTable().getPrimarySymbol(builder.addr("0x80")).getName());
			assertTrue(program.getSymbolTable().getGlobalSymbols("g_ExecLibraryBase").isEmpty());
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void discoversAnApiNameCopiedIntoTheOpenLibraryArgumentRegister() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("api-name-copy", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x200);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			// SysBase -> A6; address of icon.library -> A0 -> A1; OpenLibrary(A1, 0).
			builder.setBytes("0x100", "2c78000441fa0038224870004eaefdd84e75");
			builder.createString("0x140", "icon.library");
			builder.disassemble("0x100", 0x12);
			builder.createMemoryReadReference("0x100", "0x4");
			builder.createMemoryReference("0x104", "0x140", RefType.DATA, SourceType.ANALYSIS, 0);
			Function function = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			boolean[] added = new boolean[1];
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				HashMap<ghidra.program.model.address.Address, String> knownBases = new HashMap<>();
				knownBases.put(builder.addr("0x4"), "exec_library");
				var evidence = analyzer.findReferencedApiLibraries(program,
						new String[] { "icon_library" }, knownBases);
				assertEquals(1, evidence.size());
				AmigaHunkAnalyzer.ApiDiscoveryEvidence discovery = evidence.iterator().next();
				assertEquals("icon_library", discovery.apiKey());
				assertEquals(builder.addr("0x140"), discovery.stringAddress());
				assertEquals(builder.addr("0x10c"), discovery.openerAddress());
				assertEquals("OpenLibrary", discovery.openerName());
				added[0] = analyzer.added(program, new AddressSet(function.getBody()), TaskMonitor.DUMMY,
						new MessageLog());
			});
			assertTrue(added[0]);
			assertTrue("a copied name argument should select icon.library",
					program.getMemory().getBlock("icon_library") != null);
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void doesNotDiscoverAnApiNameFromAnotherControlFlowPredecessor() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("api-name-control-flow", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x200);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			// The branch reaches OpenLibrary without defining A1. The adjacent
			// fall-through code does define icon.library in A1, but is another block.
			builder.setBytes("0x100", "2c780004600a43fa003670004e714e714eaefdd84e75");
			builder.createString("0x140", "icon.library");
			builder.disassemble("0x100", 0x16);
			builder.createMemoryReadReference("0x100", "0x4");
			builder.createMemoryReference("0x106", "0x140", RefType.DATA, SourceType.ANALYSIS, 0);
			Function function = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			boolean[] added = new boolean[1];
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				added[0] = analyzer.added(program, new AddressSet(function.getBody()), TaskMonitor.DUMMY,
						new MessageLog());
			});
			assertTrue(added[0]);
			assertNull("a name assignment in another predecessor block is not proof",
					program.getMemory().getBlock("icon_library"));
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void doesNotDiscoverAStringThatIsNotTheOpenLibraryArgument() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("api-name-not-argument", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x200);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			// The code references icon.library, then overwrites A1 before OpenLibrary.
			builder.setBytes("0x100", "2c78000443fa0038227c0000018070004eaefdd84e75");
			builder.createString("0x140", "icon.library");
			builder.disassemble("0x100", 0x18);
			builder.createMemoryReadReference("0x100", "0x4");
			builder.createMemoryReference("0x104", "0x140", RefType.DATA, SourceType.ANALYSIS, 0);
			Function function = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			boolean[] added = new boolean[1];
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				added[0] = analyzer.added(program, new AddressSet(function.getBody()), TaskMonitor.DUMMY,
						new MessageLog());
			});
			assertTrue(added[0]);
			assertNull("a merely referenced API name must not select a table",
					program.getMemory().getBlock("icon_library"));
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void doesNotDiscoverDisplayOnlyUnreferencedOrUnknownApiNames() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("api-name-negative-evidence", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x300);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			// Display-only reference to icon.library.
			builder.setBytes("0x100", "41fa007c4e75");
			// OpenLibrary("unknown.library", 0) must remain harmless and unselected.
			builder.setBytes("0x120", "2c78000443fa006870004eaefdd84e75");
			builder.createString("0x180", "icon.library");
			builder.createString("0x190", "unknown.library");
			builder.createString("0x1b0", "workbench.library");
			builder.disassemble("0x100", 6);
			builder.disassemble("0x120", 0x12);
			builder.createMemoryReference("0x100", "0x180", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReadReference("0x120", "0x4");
			builder.createMemoryReference("0x124", "0x190", RefType.DATA, SourceType.ANALYSIS, 0);
			Function display = builder.createFunction("0x100");
			Function unknownOpen = builder.createFunction("0x120");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			boolean[] added = new boolean[1];
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				AddressSet functions = new AddressSet(display.getBody());
				functions.add(unknownOpen.getBody());
				added[0] = analyzer.added(program, functions, TaskMonitor.DUMMY, new MessageLog());
			});
			assertTrue(added[0]);
			assertNull("a display-only reference must not select icon.library",
					program.getMemory().getBlock("icon_library"));
			assertNull("an unreferenced name must not select workbench.library",
					program.getMemory().getBlock("workbench_library"));
			assertNull("an unknown opener name must not create a synthetic table",
					program.getMemory().getBlock("unknown_library"));
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void discoversAKnownDevicePassedToOpenDevice() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("api-device", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x200);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			// OpenDevice("console.device", 0, ioRequest, 0): the name ABI register is A0.
			builder.setBytes("0x100", "2c78000441fa00387000227c0000018072004eaefe444e75");
			builder.createString("0x140", "console.device");
			builder.disassemble("0x100", 0x18);
			builder.createMemoryReadReference("0x100", "0x4");
			builder.createMemoryReference("0x104", "0x140", RefType.DATA, SourceType.ANALYSIS, 0);
			Function function = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			boolean[] added = new boolean[1];
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				added[0] = analyzer.added(program, new AddressSet(function.getBody()), TaskMonitor.DUMMY,
						new MessageLog());
			});
			assertTrue(added[0]);
			assertTrue("OpenDevice should select its known device table",
					program.getMemory().getBlock("console_device") != null);
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void resolvesDeviceVectorAfterASuccessfulOpenDevice() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("device-success", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x300);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			builder.setBytes("0x100", "2c78000441fa007843f900000080700072004eaefe444a80660c" +
					"2c79000000944eaeffd64e754e75");
			builder.createString("0x180", "timer.device");
			builder.disassemble("0x100", 0x2a);
			builder.createMemoryReadReference("0x100", "0x4");
			builder.createMemoryReference("0x104", "0x180", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReference("0x108", "0x80", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReadReference("0x11a", "0x94");
			Function function = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				assertTrue(analyzer.added(program, new AddressSet(function.getBody()), TaskMonitor.DUMMY,
						new MessageLog()));
			});
			assertTrue("OpenDevice should select timer.device", program.getMemory().getBlock("timer_device") != null);
			assertVectorTarget(program, builder.addr("0x120"), "timer_device_AddTime");
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void resolvesDeviceVectorWhenBeqBranchesToOpenDeviceSuccess() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("device-success-beq", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x300);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			// tst D0; beq success; return-on-failure; success: load io_Device and call AddTime.
			builder.setBytes("0x100", "2c78000441fa00787000227c0000008072004eaefe444a806702" +
					"4e752c79000000944eaeffd64e75");
			builder.createString("0x180", "timer.device");
			builder.disassemble("0x100", 0x2c);
			builder.createMemoryReadReference("0x100", "0x4");
			builder.createMemoryReference("0x104", "0x180", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReference("0x10a", "0x80", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReadReference("0x11c", "0x94");
			Function function = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				assertTrue(analyzer.added(program, new AddressSet(function.getBody()), TaskMonitor.DUMMY,
						new MessageLog()));
			});
			assertVectorTarget(program, builder.addr("0x122"), "timer_device_AddTime");
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void doesNotResolveDeviceVectorAfterAnUncheckedOpenDevice() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("device-unchecked", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x300);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			builder.setBytes("0x100", "2c78000441fa007843f900000080700072004eaefe44" +
					"2c79000000944eaeffd64e75");
			builder.createString("0x180", "timer.device");
			builder.disassemble("0x100", 0x24);
			builder.createMemoryReadReference("0x100", "0x4");
			builder.createMemoryReference("0x104", "0x180", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReference("0x108", "0x80", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReadReference("0x116", "0x94");
			Function function = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				assertTrue(analyzer.added(program, new AddressSet(function.getBody()), TaskMonitor.DUMMY,
						new MessageLog()));
			});
			assertNoVectorTarget(program, builder.addr("0x11c"));
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void doesNotResolveDeviceVectorAtAConflictingSuccessMerge() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("device-conflict", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x300);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			// A separate branch can reach the apparent success block without OpenDevice succeeding.
			builder.setBytes("0x100", "601c4e752c78000441fa007843f900000080700072004eaefe44" +
					"4a80660c2c79000000944eaeffd64e754e75");
			builder.createString("0x184", "timer.device");
			builder.disassemble("0x100", 0x2c);
			builder.createMemoryReadReference("0x104", "0x4");
			builder.createMemoryReference("0x108", "0x184", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReference("0x10c", "0x80", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReadReference("0x11e", "0x94");
			Function function = builder.createFunction("0x104");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				assertTrue(analyzer.added(program, new AddressSet(function.getBody()), TaskMonitor.DUMMY,
						new MessageLog()));
			});
			assertNoVectorTarget(program, builder.addr("0x124"));
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void keepsSeparateOpenDeviceRequestsIndependent() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("device-independent", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x300);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			builder.setBytes("0x100", "2c78000441fa007843f900000080700072004eaefe444a806630" +
					"2c79000000944eaeffd62c78000441fa007443f9000000a0700072004eaefe44" +
					"4a80660c2c79000000b44eaeffd64e754e75");
			builder.createString("0x180", "timer.device");
			builder.createString("0x1a0", "console.device");
			builder.disassemble("0x100", 0x4c);
			builder.createMemoryReadReference("0x100", "0x4");
			builder.createMemoryReference("0x104", "0x180", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReference("0x108", "0x80", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReadReference("0x11a", "0x94");
			builder.createMemoryReadReference("0x124", "0x4");
			builder.createMemoryReference("0x128", "0x1a0", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReference("0x12c", "0xa0", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReadReference("0x13e", "0xb4");
			Function function = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				assertTrue(analyzer.added(program, new AddressSet(function.getBody()), TaskMonitor.DUMMY,
						new MessageLog()));
			});
			assertVectorTarget(program, builder.addr("0x120"), "timer_device_AddTime");
			assertVectorTarget(program, builder.addr("0x144"), "console_device_CDInputHandler");
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void discoversAKnownResourcePassedToOpenResource() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("api-resource", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x200);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			// OpenResource("card.resource"): the name ABI register is A1.
			builder.setBytes("0x100", "2c78000443fa00384eaefe0e4e75");
			builder.createString("0x140", "card.resource");
			builder.disassemble("0x100", 0x10);
			builder.createMemoryReadReference("0x100", "0x4");
			builder.createMemoryReference("0x104", "0x140", RefType.DATA, SourceType.ANALYSIS, 0);
			Function function = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			boolean[] added = new boolean[1];
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				added[0] = analyzer.added(program, new AddressSet(function.getBody()), TaskMonitor.DUMMY,
						new MessageLog());
			});
			assertTrue(added[0]);
			assertTrue("OpenResource should select its known resource table",
					program.getMemory().getBlock("card_resource") != null);
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void doesNotDiscoverApisWhenAutomaticDiscoveryIsDisabled() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("api-discovery-disabled", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x200);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			builder.setBytes("0x100", "2c78000443fa003870004eaefdd84e75");
			builder.createString("0x140", "icon.library");
			builder.disassemble("0x100", 0x12);
			builder.createMemoryReadReference("0x100", "0x4");
			builder.createMemoryReference("0x104", "0x140", RefType.DATA, SourceType.ANALYSIS, 0);
			Function function = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			boolean[] added = new boolean[1];
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				ToolOptions options = new ToolOptions("test");
				analyzer.registerOptions(options, program);
				options.setBoolean("Automatically discover referenced APIs", false);
				analyzer.optionsChanged(options, program);
				added[0] = analyzer.added(program, new AddressSet(function.getBody()), TaskMonitor.DUMMY,
						new MessageLog());
			});
			assertTrue(added[0]);
			assertNull(program.getMemory().getBlock("icon_library"));
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void givesConflictingAnalysisSymbolsStableAddressQualifiedNames() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("analysis-labels", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x100);
			Program program = builder.getProgram();
			builder.withTransaction(() -> {
				try {
					AmigaUtils.applyAnalysisGlobalLabel(program, builder.addr("0x20"), "g_Base");
					AmigaUtils.applyAnalysisGlobalLabel(program, builder.addr("0x24"), "g_Base");
					AmigaUtils.applyAnalysisGlobalLabel(program, builder.addr("0x24"), "g_Base");
				}
				catch (Exception e) {
					throw new AssertionError(e);
				}
			});
			assertEquals("g_Base", program.getSymbolTable().getPrimarySymbol(builder.addr("0x20")).getName());
			assertEquals("g_Base_at_00000024",
					program.getSymbolTable().getPrimarySymbol(builder.addr("0x24")).getName());
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void resolvesAnApiVectorThroughAnOpenLibraryReturnRegister() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("api-return-register", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x200);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			builder.setBytes("0x100", "2c78000443fa003870004eaefdd82c404eaeffbe4e75");
			builder.createString("0x140", "dos.library");
			builder.disassemble("0x100", 0x16);
			builder.createMemoryReadReference("0x100", "0x4");
			builder.createMemoryReference("0x104", "0x140", RefType.DATA, SourceType.ANALYSIS, 0);
			Function function = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			boolean[] added = new boolean[1];
			MessageLog log = new MessageLog();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				added[0] = analyzer.added(program, new AddressSet(function.getBody()), TaskMonitor.DUMMY, log);
			});
			assertTrue(log.toString(), added[0]);

			boolean resolved = false;
			for (Reference reference : program.getListing().getInstructionAt(builder.addr("0x110")).getReferencesFrom()) {
				Function target = program.getFunctionManager().getFunctionAt(reference.getToAddress());
				resolved |= target != null && "dos_library_Seek".equals(target.getName());
			}
			assertTrue("a library return copied from D0 to A6 should resolve its vector", resolved);
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void resolvesAnApiVectorWhenAStoredLibraryBaseIsReloadedThroughD0() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("api-base-d0-reload", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x200);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			// OpenLibrary("icon.library") -> D0; store D0; reload it through D0; move D0,A6; call GetDiskObject.
			builder.setBytes("0x100", "2c78000443fa003870004eaefdd8298000802039000000802c404eaeffb24e75");
			builder.createString("0x140", "icon.library");
			builder.disassemble("0x100", 0x20);
			builder.createMemoryReadReference("0x100", "0x4");
			builder.createMemoryReference("0x104", "0x140", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReference("0x10e", "0x80", RefType.WRITE, SourceType.ANALYSIS, 1);
			builder.createMemoryReadReference("0x112", "0x80");
			Function function = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			boolean[] added = new boolean[1];
			MessageLog log = new MessageLog();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				try {
					assertTrue(analyzer.canAnalyze(program));
					added[0] = analyzer.added(program, new AddressSet(function.getBody()), TaskMonitor.DUMMY, log);
				}
				catch (Exception e) {
					throw new AssertionError(e);
				}
			});
			assertTrue(log.toString(), added[0]);
			Instruction moveToA6 = program.getListing().getInstructionAt(builder.addr("0x118"));
			assertEquals("Ghidra does not mark the source register of MOVEA as READ", 0,
					moveToA6.getOperandType(0) & OperandType.READ);
			assertTrue("the existing analyzer obtains that source from input objects",
					Arrays.stream(moveToA6.getInputObjects()).anyMatch(object -> object instanceof Register register &&
							"D0".equals(register.getName())));
			assertVectorTarget(program, builder.addr("0x11a"), "icon_library_GetDiskObject");
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void preservesAnOpenLibraryBaseThroughAControlFlowLoop() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("api-return-loop", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x200);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			builder.setBytes("0x100",
					"48e7300243fa005870002c7800044eaefdd82c40240067362214241376ff4eaeffbe4a816bf2" +
					"7200242c000826144eaeff6a4a8167e2206c0008d1c8d1c84a906704205060f420804cdf400c" +
					"4e752e3c0700000c2c7800044eaeff944e75");
			builder.createString("0x160", "dos.library");
			builder.disassemble("0x100", 0x60);
			builder.createMemoryReadReference("0x10a", "0x4");
			builder.createMemoryReference("0x104", "0x160", RefType.DATA, SourceType.ANALYSIS, 0);
			Function function = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			boolean[] added = new boolean[1];
			MessageLog log = new MessageLog();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				added[0] = analyzer.added(program, new AddressSet(function.getBody()), TaskMonitor.DUMMY, log);
			});
			assertTrue(log.toString(), added[0]);
			assertVectorTarget(program, builder.addr("0x11e"), "dos_library_Seek");
			assertVectorTarget(program, builder.addr("0x12e"), "dos_library_LoadSeg");
		}
		finally {
			builder.dispose();
		}
	}

	private static void assertVectorTarget(Program program, ghidra.program.model.address.Address address,
			String targetName) {
		for (Reference reference : program.getListing().getInstructionAt(address).getReferencesFrom()) {
			Function target = program.getFunctionManager().getFunctionAt(reference.getToAddress());
			if (target != null && targetName.equals(target.getName())) {
				return;
			}
		}
		throw new AssertionError("expected " + targetName + " override at " + address);
	}

	private static void assertNoVectorTarget(Program program, ghidra.program.model.address.Address address) {
		for (Reference reference : program.getListing().getInstructionAt(address).getReferencesFrom()) {
			if (reference.getReferenceType().isCall() && reference.getSource() == SourceType.ANALYSIS) {
				throw new AssertionError("unexpected analysis call override at " + address);
			}
		}
	}

	private static int getAnalysisReferenceCount(Instruction instruction) {
		int count = 0;
		for (Reference reference : instruction.getReferencesFrom()) {
			if (reference.getSource() == SourceType.ANALYSIS) {
				count++;
			}
		}
		return count;
	}

	@Test
	public void givesAnOpenLibraryForwarderItsStackParameters() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("open-wrapper", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x200);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			builder.setBytes("0x100", "2f0e2c780004226f0008202f000c4eaefdd82c5f4e75");
			builder.disassemble("0x100", 0x16);
			builder.createMemoryReadReference("0x104", "0x4");
			Function wrapper = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			boolean[] added = new boolean[1];
			MessageLog log = new MessageLog();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				added[0] = analyzer.added(program, new AddressSet(wrapper.getBody()), TaskMonitor.DUMMY, log);
			});
			assertTrue(log.toString(), added[0]);
			Function updatedWrapper = program.getFunctionManager().getFunctionAt(builder.addr("0x100"));
			assertEquals("Library *", updatedWrapper.getReturnType().getDisplayName());
			assertEquals(2, updatedWrapper.getParameterCount());
			assertEquals("libName", updatedWrapper.getParameter(0).getName());
			assertEquals(4, updatedWrapper.getParameter(0).getStackOffset());
			assertEquals("version", updatedWrapper.getParameter(1).getName());
			assertEquals(8, updatedWrapper.getParameter(1).getStackOffset());
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void discoversADevicePassedThroughAForwardingOpenDeviceWrapper() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("open-device-wrapper", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x300);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			// Save A6; forward four C stack arguments to OpenDevice's A0/D0/A1/D1 ABI; restore A6.
			builder.setBytes("0x100", "2f0e2c780004206f0008202f000c226f0010222f00144eaefe442c5f4e75");
			// OpenDevice("timer.device", 0, &request, 0) through the wrapper.
			builder.setBytes("0x140", "42a72f39000001c042a74879000001a04eb9000001004fef00104e75");
			builder.createString("0x1a0", "timer.device");
			builder.disassemble("0x100", 0x24);
			builder.disassemble("0x140", 0x20);
			builder.createMemoryReadReference("0x104", "0x4");
			builder.createMemoryReadReference("0x142", "0x1c0");
			builder.createMemoryReference("0x14a", "0x1a0", RefType.DATA, SourceType.ANALYSIS, 0);
			Function wrapper = builder.createFunction("0x100");
			Function caller = builder.createFunction("0x140");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				AddressSet functions = new AddressSet(wrapper.getBody());
				functions.add(caller.getBody());
				assertTrue(analyzer.added(program, functions, TaskMonitor.DUMMY, new MessageLog()));
				int functionCount = program.getFunctionManager().getFunctionCount();
				assertTrue(analyzer.added(program, functions, TaskMonitor.DUMMY, new MessageLog()));
				assertEquals(functionCount, program.getFunctionManager().getFunctionCount());
			});
			Function updatedWrapper = program.getFunctionManager().getFunctionAt(builder.addr("0x100"));
			assertEquals("BYTE", updatedWrapper.getReturnType().getDisplayName());
			assertEquals(4, updatedWrapper.getParameterCount());
			assertEquals("devName", updatedWrapper.getParameter(0).getName());
			assertEquals(4, updatedWrapper.getParameter(0).getStackOffset());
			assertEquals("ioRequest", updatedWrapper.getParameter(2).getName());
			assertEquals(12, updatedWrapper.getParameter(2).getStackOffset());
			assertTrue("a proven OpenDevice wrapper call should select timer.device",
					program.getMemory().getBlock("timer_device") != null);
			assertEquals("timerequest *", program.getListing().getDefinedDataAt(builder.addr("0x1c0"))
					.getDataType().getDisplayName());
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void doesNotTrustAnOpenDeviceWrapperWithAnExtraCall() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("open-device-wrapper-extra-call", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x300);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			// This otherwise forwarding wrapper calls another function after OpenDevice, so it is not a transparent forwarder.
			builder.setBytes("0x100", "2f0e2c780004206f0008202f000c226f0010222f00144eaefe444eb9000001302c5f4e75");
			builder.setBytes("0x140", "42a72f39000001c042a74879000001a04eb9000001004fef00104e75");
			builder.setBytes("0x130", "4e75");
			builder.createString("0x1a0", "timer.device");
			builder.disassemble("0x100", 0x2a);
			builder.disassemble("0x140", 0x20);
			builder.disassemble("0x130", 2);
			builder.createMemoryReadReference("0x104", "0x4");
			builder.createMemoryReference("0x14a", "0x1a0", RefType.DATA, SourceType.ANALYSIS, 0);
			Function wrapper = builder.createFunction("0x100");
			Function caller = builder.createFunction("0x140");
			builder.createFunction("0x130");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				AddressSet functions = new AddressSet(wrapper.getBody());
				functions.add(caller.getBody());
				assertTrue(analyzer.added(program, functions, TaskMonitor.DUMMY, new MessageLog()));
			});
			assertNull("a wrapper with an extra call must not select timer.device",
					program.getMemory().getBlock("timer_device"));
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void typesAnA4RelativeRequestSlotPassedThroughAnOpenDeviceWrapper() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("open-device-wrapper-a4-request", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x300);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			builder.setBytes("0x100", "2f0e2c780004206f0008202f000c226f0010222f00144eaefe442c5f4e75");
			// A4 is an anchored data base; its -4 slot is the request pointer passed to OpenDevice.
			builder.setBytes("0x140", "287c000001c442a72f2cffff42a74879000001a04eb9000001004fef00104e75");
			// The reference points to the C string after a three-byte prefix, as in
			// Treasure's Hunk relocation presentation.  No defined string starts there.
			builder.setBytes("0x1a0", "12345674696d65722e64657669636500");
			builder.disassemble("0x100", 0x24);
			builder.disassemble("0x140", 0x26);
			builder.createMemoryReadReference("0x104", "0x4");
			builder.createMemoryReadReference("0x148", "0x1c0");
			// Treasure's relocated PC-relative reference points past the prefix.
			builder.createMemoryReference("0x14e", "0x1a3", RefType.DATA, SourceType.ANALYSIS, 0);
			Function wrapper = builder.createFunction("0x100");
			Function caller = builder.createFunction("0x140");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertEquals("timer.device", AmigaHunkAnalyzer.getReferencedString(program, builder.addr("0x1a3")));
				assertTrue(analyzer.canAnalyze(program));
				AddressSet functions = new AddressSet(wrapper.getBody());
				functions.add(caller.getBody());
				assertTrue(analyzer.added(program, functions, TaskMonitor.DUMMY, new MessageLog()));
				// Reproduce the weaker type left by an earlier/general analysis pass.
				try {
					DataUtilities.createData(program, builder.addr("0x1c0"), wrapper.getParameter(2).getDataType(), -1,
							false, ClearDataMode.CLEAR_ALL_CONFLICT_DATA);
				}
				catch (Exception exception) {
					throw new AssertionError(exception);
				}
				assertEquals("IORequest *", program.getListing().getDefinedDataAt(builder.addr("0x1c0"))
						.getDataType().getDisplayName());
				assertTrue(analyzer.added(program, functions, TaskMonitor.DUMMY, new MessageLog()));
			});
			assertEquals("timerequest *", program.getListing().getDefinedDataAt(builder.addr("0x1c0"))
					.getDataType().getDisplayName());
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void typesConsoleDeviceRequestAsIoStdReq() throws Exception {
		assertStandardDeviceRequestType("console.device");
	}

	@Test
	public void typesInputDeviceRequestAsIoStdReq() throws Exception {
		assertStandardDeviceRequestType("input.device");
	}

	@Test
	public void typesGameportDeviceRequestAsIoStdReq() throws Exception {
		assertStandardDeviceRequestType("gameport.device");
	}

	@Test
	public void typesAnAnalysisOwnedLocalPassedToDirectOpenDevice() throws Exception {
		assertDirectOpenDeviceLocalType();
	}

	@Test
	public void typesADirectStaticConsoleRequestObject() throws Exception {
		assertDirectStaticRequestObject("console.device");
	}

	@Test
	public void typesADirectStaticInputRequestObject() throws Exception {
		assertDirectStaticRequestObject("input.device");
	}

	@Test
	public void doesNotTypeAnIndirectDirectOpenDeviceRequest() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("direct-open-device-indirect-request", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x300);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			// A1 loads a pointer value from memory; it is not the address of a request object.
			builder.setBytes("0x100", "2c78000441f9000001a070002279000001c072004eaefe444e75");
			builder.createString("0x1a0", "console.device");
			builder.disassemble("0x100", 0x1a);
			builder.createMemoryReadReference("0x100", "0x4");
			builder.createMemoryReference("0x104", "0x1a0", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReadReference("0x10c", "0x1c0");
			Function caller = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				assertTrue(analyzer.added(program, new AddressSet(caller.getBody()), TaskMonitor.DUMMY,
						new MessageLog()));
			});
			assertNull(program.getListing().getDefinedDataAt(builder.addr("0x1c0")));
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void doesNotOverwriteAUserDefinedDirectStaticRequestObject() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("direct-open-device-user-request", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x300);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			builder.setBytes("0x100", "2c78000441f9000001a0700043f9000001c072004eaefe444e75");
			builder.createString("0x1a0", "console.device");
			builder.disassemble("0x100", 0x1a);
			builder.createMemoryReadReference("0x100", "0x4");
			builder.createMemoryReference("0x104", "0x1a0", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReference("0x10c", "0x1c0", RefType.DATA, SourceType.ANALYSIS, 0);
			Function caller = builder.createFunction("0x100");

			Program program = builder.getProgram();
			builder.withTransaction(() -> {
				try {
					// A user-owned field inside the proposed IOStdReq range also protects the object.
					program.getSymbolTable().createLabel(builder.addr("0x1d4"), "userRequestField",
							SourceType.USER_DEFINED);
				}
				catch (Exception exception) {
					throw new AssertionError(exception);
				}
			});
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				assertTrue(analyzer.added(program, new AddressSet(caller.getBody()), TaskMonitor.DUMMY,
						new MessageLog()));
			});
			assertNull(program.getListing().getDefinedDataAt(builder.addr("0x1c0")));
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void doesNotSpecializeAConflictingDirectStaticRequestObject() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("direct-open-device-request-conflict", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x300);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			builder.setBytes("0x100", "2c78000441f9000001a0700043f9000001c072004eaefe444e75");
			builder.setBytes("0x140", "2c78000441f9000001b0700043f9000001c072004eaefe444e75");
			builder.createString("0x1a0", "console.device");
			builder.createString("0x1b0", "timer.device");
			builder.disassemble("0x100", 0x1a);
			builder.disassemble("0x140", 0x1a);
			builder.createMemoryReadReference("0x100", "0x4");
			builder.createMemoryReadReference("0x140", "0x4");
			builder.createMemoryReference("0x104", "0x1a0", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReference("0x10c", "0x1c0", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReference("0x144", "0x1b0", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReference("0x14c", "0x1c0", RefType.DATA, SourceType.ANALYSIS, 0);
			Function first = builder.createFunction("0x100");
			Function second = builder.createFunction("0x140");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				AddressSet functions = new AddressSet(first.getBody());
				functions.add(second.getBody());
				assertTrue(analyzer.added(program, functions, TaskMonitor.DUMMY, new MessageLog()));
			});
			assertNull(program.getListing().getDefinedDataAt(builder.addr("0x1c0")));
		}
		finally {
			builder.dispose();
		}
	}

	private static void assertDirectStaticRequestObject(String deviceName) throws Exception {
		ProgramBuilder builder = new ProgramBuilder("direct-open-device-object-" + deviceName,
				"68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x300);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			// LEA materialises the request object's address directly in A1.
			builder.setBytes("0x100", "2c78000441f9000001a0700043f9000001c072004eaefe444e75");
			builder.createString("0x1a0", deviceName);
			builder.disassemble("0x100", 0x1a);
			builder.createMemoryReadReference("0x100", "0x4");
			builder.createMemoryReference("0x104", "0x1a0", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReference("0x10c", "0x1c0", RefType.DATA, SourceType.ANALYSIS, 0);
			Function caller = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				assertTrue(analyzer.added(program, new AddressSet(caller.getBody()), TaskMonitor.DUMMY,
						new MessageLog()));
				assertTrue(analyzer.added(program, new AddressSet(caller.getBody()), TaskMonitor.DUMMY,
						new MessageLog()));
			});
			assertEquals("IOStdReq", program.getListing().getDefinedDataAt(builder.addr("0x1c0"))
					.getDataType().getDisplayName());
		}
		finally {
			builder.dispose();
		}
	}

	private static void assertDirectOpenDeviceLocalType() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("open-device-local-request", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x300);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			// A2 is an analysis-owned local request pointer.  The post-open write at
			// 0x28 is IOStdReq.io_Data, not IORequest[1].Message.Node.ln_Type.
			builder.setBytes("0x100", "2c7800044eb900000180244041fa00907000224a72004eaefe4442aa00284e75");
			builder.setBytes("0x180", "70004e75");
			builder.createString("0x1a0", "input.device");
			builder.disassemble("0x100", 0x22);
			builder.disassemble("0x180", 4);
			builder.createMemoryReadReference("0x104", "0x4");
			builder.createMemoryReference("0x10c", "0x1a0", RefType.DATA, SourceType.ANALYSIS, 0);
			Function caller = builder.createFunction("0x100");
			builder.createFunction("0x180");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				assertTrue(analyzer.added(program, new AddressSet(caller.getBody()), TaskMonitor.DUMMY,
						new MessageLog()));
				assertTrue(analyzer.added(program, new AddressSet(caller.getBody()), TaskMonitor.DUMMY,
						new MessageLog()));
			});
			ghidra.program.model.listing.Variable request = findLocal(caller, "ioRequest");
			assertTrue(request != null);
			assertEquals("IOStdReq *", request.getDataType().getDisplayName());
		}
		finally {
			builder.dispose();
		}
	}

	private static ghidra.program.model.listing.Variable findLocal(Function function, String name) {
		for (ghidra.program.model.listing.Variable local : function.getLocalVariables()) {
			if (name.equals(local.getName())) {
				return local;
			}
		}
		return null;
	}

	@Test
	public void doesNotOverwriteUserDefinedStandardDeviceRequestStorage() throws Exception {
		assertStandardDeviceRequestType("console.device", true);
	}

	private static void assertStandardDeviceRequestType(String deviceName) throws Exception {
		assertStandardDeviceRequestType(deviceName, false);
	}

	private static void assertStandardDeviceRequestType(String deviceName, boolean userDefinedStorage) throws Exception {
		ProgramBuilder builder = new ProgramBuilder("open-device-wrapper-" + deviceName, "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x300);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			builder.setBytes("0x100", "2f0e2c780004206f0008202f000c226f0010222f00144eaefe442c5f4e75");
			builder.setBytes("0x140", "42a72f39000001c042a74879000001a04eb9000001004fef00104e75");
			builder.createString("0x1a0", deviceName);
			builder.disassemble("0x100", 0x24);
			builder.disassemble("0x140", 0x20);
			builder.createMemoryReadReference("0x104", "0x4");
			builder.createMemoryReadReference("0x142", "0x1c0");
			builder.createMemoryReference("0x14a", "0x1a0", RefType.DATA, SourceType.ANALYSIS, 0);
			Function wrapper = builder.createFunction("0x100");
			Function caller = builder.createFunction("0x140");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				AddressSet functions = new AddressSet(wrapper.getBody());
				functions.add(caller.getBody());
				assertTrue(analyzer.added(program, functions, TaskMonitor.DUMMY, new MessageLog()));
				try {
					DataUtilities.createData(program, builder.addr("0x1c0"), wrapper.getParameter(2).getDataType(), -1,
							false, ClearDataMode.CLEAR_ALL_CONFLICT_DATA);
				}
				catch (Exception exception) {
					throw new AssertionError(exception);
				}
				assertEquals("IORequest *", program.getListing().getDefinedDataAt(builder.addr("0x1c0"))
						.getDataType().getDisplayName());
				if (userDefinedStorage) {
					try {
						program.getSymbolTable().createLabel(builder.addr("0x1c0"), "userRequest",
								SourceType.USER_DEFINED);
					}
					catch (Exception exception) {
						throw new AssertionError(exception);
					}
				}
				assertTrue(analyzer.added(program, functions, TaskMonitor.DUMMY, new MessageLog()));
			});
			assertEquals(deviceName, userDefinedStorage ? "IORequest *" : "IOStdReq *",
					program.getListing().getDefinedDataAt(builder.addr("0x1c0"))
					.getDataType().getDisplayName());
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void doesNotSpecializeARequestSlotWithConflictingDeviceEvidence() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("open-device-request-conflict", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x400);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			builder.setBytes("0x100", "2f0e2c780004206f0008202f000c226f0010222f00144eaefe442c5f4e75");
			// Two proven calls give the same request slot incompatible known device types.
			builder.setBytes("0x140", "42a72f39000001c042a74879000001a04eb9000001004fef00104e75");
			builder.setBytes("0x170", "42a72f39000001c042a74879000001b04eb9000001004fef00104e75");
			builder.createString("0x1a0", "console.device");
			builder.createString("0x1b0", "timer.device");
			builder.disassemble("0x100", 0x24);
			builder.disassemble("0x140", 0x20);
			builder.disassemble("0x170", 0x20);
			builder.createMemoryReadReference("0x104", "0x4");
			builder.createMemoryReadReference("0x142", "0x1c0");
			builder.createMemoryReadReference("0x172", "0x1c0");
			builder.createMemoryReference("0x14a", "0x1a0", RefType.DATA, SourceType.ANALYSIS, 0);
			builder.createMemoryReference("0x17a", "0x1b0", RefType.DATA, SourceType.ANALYSIS, 0);
			Function wrapper = builder.createFunction("0x100");
			Function firstCaller = builder.createFunction("0x140");
			Function secondCaller = builder.createFunction("0x170");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				AddressSet functions = new AddressSet(wrapper.getBody());
				functions.add(firstCaller.getBody());
				functions.add(secondCaller.getBody());
				assertTrue(analyzer.added(program, functions, TaskMonitor.DUMMY, new MessageLog()));
			});
			assertEquals("IORequest *", program.getListing().getDefinedDataAt(builder.addr("0x1c0"))
					.getDataType().getDisplayName());
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void givesAnExecWaitIoForwarderItsPersistentSignatureAndNamesExecBase() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("waitio-wrapper", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x200);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			builder.setBytes("0x100", "2f0e2c780004226f00084eaefe262c5f4e75");
			builder.disassemble("0x100", 0x14);
			builder.createMemoryReadReference("0x104", "0x4");
			Function wrapper = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			boolean[] added = new boolean[1];
			MessageLog log = new MessageLog();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				added[0] = analyzer.added(program, new AddressSet(wrapper.getBody()), TaskMonitor.DUMMY, log);
			});
			assertTrue(log.toString(), added[0]);

			assertVectorTarget(program, builder.addr("0x10a"), "exec_library_WaitIO");
			Function updatedWrapper = program.getFunctionManager().getFunctionAt(builder.addr("0x100"));
			assertEquals(1, updatedWrapper.getParameterCount());
			assertEquals("ioRequest", updatedWrapper.getParameter(0).getName());
			assertEquals("IORequest *", updatedWrapper.getParameter(0).getDataType().getDisplayName());
			assertEquals(4, updatedWrapper.getParameter(0).getStackOffset());
			assertEquals(SourceType.ANALYSIS, updatedWrapper.getSignatureSource());
			assertEquals("SysBase", program.getSymbolTable().getPrimarySymbol(builder.addr("0x4")).getName());
			assertEquals("ExecBase *", program.getListing().getDefinedDataAt(builder.addr("0x4"))
					.getDataType().getDisplayName());
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void givesAVoidExecForwarderItsPersistentStackSignature() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("close-device-wrapper", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x200);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			// void CloseDevice(IORequest *): preserve A6, load the Exec base and A1, call -450(A6), restore A6.
			builder.setBytes("0x100", "2f0e2c780004226f00084eaefe3e2c5f4e75");
			builder.disassemble("0x100", 0x14);
			builder.createMemoryReadReference("0x104", "0x4");
			Function wrapper = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			boolean[] added = new boolean[1];
			MessageLog log = new MessageLog();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				added[0] = analyzer.added(program, new AddressSet(wrapper.getBody()), TaskMonitor.DUMMY, log);
			});
			assertTrue(log.toString(), added[0]);
			assertVectorTarget(program, builder.addr("0x10a"), "exec_library_CloseDevice");

			Function updatedWrapper = program.getFunctionManager().getFunctionAt(builder.addr("0x100"));
			assertEquals("void", updatedWrapper.getReturnType().getDisplayName());
			assertEquals(1, updatedWrapper.getParameterCount());
			assertEquals("ioRequest", updatedWrapper.getParameter(0).getName());
			assertEquals("IORequest *", updatedWrapper.getParameter(0).getDataType().getDisplayName());
			assertEquals(4, updatedWrapper.getParameter(0).getStackOffset());
			assertEquals(SourceType.ANALYSIS, updatedWrapper.getSignatureSource());
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void propagatesIoRequestThroughStandardDeviceBeginIoDispatch() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("device-dispatch", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x300);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			// void BeginIO(IORequest *ioRequest): A1=ioRequest; A6=ioRequest->io_Device; jsr -30(A6)
			builder.setBytes("0x100", "2f0e226f00082c6900144eaeffe22c5f4e75");
			// void Submit(IORequest *ioRequest): preserve A2, forward it to BeginIO, restore A2.
			builder.setBytes("0x140", "48e70020246f00082f0a4eb900000100584f4cdf04004e75");
			builder.disassemble("0x100", 0x14);
			builder.disassemble("0x140", 0x18);
			Function beginIoWrapper = builder.createFunction("0x100");
			Function submit = builder.createFunction("0x140");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			boolean[] added = new boolean[1];
			MessageLog log = new MessageLog();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				added[0] = analyzer.added(program, new AddressSet(beginIoWrapper.getBody()), TaskMonitor.DUMMY, log);
			});
			assertTrue(log.toString(), added[0]);

			Function typedBeginIo = program.getFunctionManager().getFunctionAt(builder.addr("0x100"));
			assertEquals(1, typedBeginIo.getParameterCount());
			assertEquals("IORequest *", typedBeginIo.getParameter(0).getDataType().getDisplayName());
			Function typedSubmit = program.getFunctionManager().getFunctionAt(builder.addr("0x140"));
			assertEquals(1, typedSubmit.getParameterCount());
			assertEquals("IORequest *", typedSubmit.getParameter(0).getDataType().getDisplayName());
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void doesNotTreatAnOpenLibraryHelperAsAForwarder() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("open-helper", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x200);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			builder.setBytes("0x100", "2f0e2c780004226f0008202f000c4eaefdd870002c5f4e75");
			builder.disassemble("0x100", 0x18);
			builder.createMemoryReadReference("0x104", "0x4");
			Function helper = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			boolean[] added = new boolean[1];
			MessageLog log = new MessageLog();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				added[0] = analyzer.added(program, new AddressSet(helper.getBody()), TaskMonitor.DUMMY, log);
			});
			assertTrue(log.toString(), added[0]);

			Function updatedHelper = program.getFunctionManager().getFunctionAt(builder.addr("0x100"));
			assertTrue("a helper that discards OpenLibrary's result must not acquire its signature",
					!"Library *".equals(updatedHelper.getReturnType().getDisplayName()));
			assertEquals(0, updatedHelper.getParameterCount());
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void doesNotResolveAConflictedBaseAtControlFlowMerge() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("api-merge", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x200);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			builder.setBytes("0x100", "66062c78008060042c7800844eaeffb24e75");
			builder.disassemble("0x100", 0x12);
			builder.createMemoryReadReference("0x102", "0x80");
			builder.createMemoryReadReference("0x108", "0x84");
			Function function = builder.createFunction("0x100");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			boolean[] added = new boolean[1];
			MessageLog log = new MessageLog();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				added[0] = analyzer.added(program, new AddressSet(function.getBody()), TaskMonitor.DUMMY, log);
				HashMap<ghidra.program.model.address.Address, String> bases = new HashMap<>();
				bases.put(builder.addr("0x80"), "exec_library");
				bases.put(builder.addr("0x84"), "dos_library");
				try {
					analyzer.resolveApiCallsInFunction(program, function, bases, new HashSet<>(), new HashMap<>(),
							TaskMonitor.DUMMY);
				}
				catch (ghidra.util.exception.CancelledException e) {
					throw new AssertionError(e);
				}
			});
			assertTrue(log.toString(), added[0]);

			for (Reference reference : program.getListing().getInstructionAt(builder.addr("0x10c")).getReferencesFrom()) {
				assertTrue("a branch-conflicted A6 base must not get an API call override: " + reference,
						!RefType.CALL_OVERRIDE_UNCONDITIONAL.equals(reference.getReferenceType()));
			}
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void specializesAnIoRequestParameterAndDirectGlobalArgumentAsIoAudio() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("audio-request-specialization", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x300);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			// Standard BeginIO forwarding wrapper: A1=request; A6=request->io_Device; jsr -30(A6).
			builder.setBytes("0x100", "2f0e2c780004226f00084eaefe262c5f4e75");
			// SubmitAudio writes the IOAudio fields beyond the embedded IORequest, then calls BeginIO.
			builder.setBytes("0x140", "48e70020246f0008256f000c0022256f00100026356f0014002a" +
					"356f0016002c356f0018002e2f0a4eb900000100584f4cdf04004e75");
			// Caller passes a statically addressed request pointer directly, not via its own stack parameter.
			builder.setBytes("0x1a0", "2f39000000804eb900000140584f4e75");
			builder.disassemble("0x100", 0x14);
			builder.disassemble("0x140", 0x36);
			builder.disassemble("0x1a0", 0x12);
			builder.createMemoryReadReference("0x104", "0x4");
			builder.createMemoryReadReference("0x1a0", "0x80");
			Function beginIo = builder.createFunction("0x100");
			Function submitAudio = builder.createFunction("0x140");
			builder.createFunction("0x1a0");

			Program program = builder.getProgram();
			AmigaHunkAnalyzer analyzer = new AmigaHunkAnalyzer();
			boolean[] added = new boolean[1];
			MessageLog log = new MessageLog();
			builder.withTransaction(() -> {
				program.setExecutableFormat("Amiga Hunk Executable");
				assertTrue(analyzer.canAnalyze(program));
				added[0] = analyzer.added(program, new AddressSet(beginIo.getBody()), TaskMonitor.DUMMY, log);
			});
			assertTrue(log.toString(), added[0]);
			Function typedSubmit = program.getFunctionManager().getFunctionAt(submitAudio.getEntryPoint());
			assertEquals("IOAudio *", typedSubmit.getParameter(0).getDataType().getDisplayName());
			assertEquals("IOAudio *", program.getListing().getDefinedDataAt(builder.addr("0x80"))
					.getDataType().getDisplayName());
		}
		finally {
			builder.dispose();
		}
	}

}
