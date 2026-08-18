package amiga;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.util.List;

import javax.xml.parsers.DocumentBuilderFactory;

import org.junit.Test;
import org.junit.BeforeClass;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import ghidra.app.util.bin.ByteArrayProvider;
import ghidra.app.util.bin.BinaryReader;
import ghidra.app.util.importer.MessageLog;
import ghidra.GhidraApplicationLayout;
import ghidra.framework.Application;
import ghidra.framework.ApplicationConfiguration;
import ghidra.program.database.ProgramBuilder;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.Program;
import ghidra.program.model.symbol.OffsetReference;
import ghidra.program.model.symbol.RefType;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.SourceType;
import hunk.BinFmtHunk;
import hunk.BinImage;
import hunk.HunkBlockFile;
import hunk.HunkLoadSegFile;
import hunk.Relocate;
import hunk.Segment;
import hunk.SegmentType;
import hunk.HunkType;
import hunk.Reloc;

public class AmigaHunkLoaderTest {

	@BeforeClass
	public static void initializeGhidra() throws Exception {
		if (!Application.isInitialized()) {
			Application.initializeApplication(
					new GhidraApplicationLayout(new File(System.getProperty("ghidra.install.dir"))),
					new ApplicationConfiguration());
		}
	}

	@Test
	public void calculates68000BranchTargetsRelativeToTheOpcodeSuccessor() {
		assertEquals(0x18, AmigaHunkLoader.getBranchTargetDelta(0x6000, (short) 0x16));
		assertEquals(0x72, AmigaHunkLoader.getBranchTargetDelta(0x6170, (short) 0));
		assertEquals(1, AmigaHunkLoader.getBranchTargetDelta(0x60ff, (short) 0));
	}

	@Test
	public void preservesAztecAssemblyRelocationToTwoBytesBeforeDataSegment() throws Exception {
		byte[] executable = fixtureBytes("/fixtures/aztec-c/5.0a/relocation-addend-minus-two/base-minus-two");
		try (ByteArrayProvider provider = new ByteArrayProvider(executable)) {
			HunkLoadSegFile hunkFile = BinFmtHunk.loadSegFile(
					new HunkBlockFile(new BinaryReader(provider, false), true), new MessageLog());
			BinImage image = BinFmtHunk.createImage(hunkFile, new MessageLog());
			Segment[] segments = image.getSegments();
			assertEquals(3, segments.length);
			Segment code = segments[0];
			Segment data = segments[1];
			assertEquals(SegmentType.SEGMENT_TYPE_CODE, code.getType());
			assertEquals(SegmentType.SEGMENT_TYPE_DATA, data.getType());

			Reloc[] relocations = code.getRelocations(data);
			assertEquals(1, relocations.length);
			assertEquals(2, relocations[0].getOffset());
			assertEquals(4, relocations[0].getWidth());
			assertEquals(Reloc.Kind.ABSOLUTE, relocations[0].getKind());
			List<byte[]> unbased = new Relocate(image).relocate(new int[] { 0, 0, 0 });
			assertEquals(0xfffffffe, ByteBuffer.wrap(unbased.get(0)).getInt(relocations[0].getOffset()));

			List<byte[]> relocated = new Relocate(image).relocate(new int[] { 0x1000, 0x2000, 0x3000 });
			assertEquals("The native 32-bit relocation preserves data-base minus two", 0x1ffe,
					ByteBuffer.wrap(relocated.get(0)).getInt(relocations[0].getOffset()));
		}
	}

	@Test
	public void documentsCurrentGhidraRenderingOf68000OffsetReferences() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("offset-reference-presentation", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0x1000", 0x100);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0x1000")), true);
			// movea.l #data_segment_base-2,A0; move.w (A0),D0; rts
			builder.setBytes("0x1000", "207c0000100a30104e75");
			builder.disassemble("0x1000", 10);

			Program program = builder.getProgram();
			Address source = builder.addr("0x1002");
			Address base = builder.addr("0x100c");
			int transaction = program.startTransaction("record Hunk relocation addend");
			try {
				Reference reference = program.getReferenceManager().addOffsetMemReference(source, base, true, -2,
						RefType.DATA, SourceType.IMPORTED, 0);
				program.getReferenceManager().setPrimary(reference, true);
			}
			finally {
				program.endTransaction(transaction, true);
			}

			Reference reference = program.getReferenceManager().getPrimaryReferenceFrom(source, 0);
			assertTrue("The importer retains the Hunk target base and signed addend", reference instanceof OffsetReference);
			OffsetReference offsetReference = (OffsetReference) reference;
			assertEquals(base, offsetReference.getBaseAddress());
			assertEquals(-2, offsetReference.getOffset());
			assertEquals(SourceType.IMPORTED, offsetReference.getSource());

			Instruction instruction = program.getListing().getInstructionAt(builder.addr("0x1000"));
			assertEquals("Ghidra 12.1.2 renders an offset reference as its resolved scalar; update this"
					+ " documented limitation only when native operand rendering changes.",
					"#0x100a", instruction.getDefaultOperandRepresentation(0));
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void detectsStructuredManxOverlayMetadataForAbiSelection() throws Exception {
		try (ByteArrayProvider provider = new ByteArrayProvider(createOverlayExecutable(true))) {
			assertTrue(AmigaHunkLoader.isManxOverlayExecutable(provider));
		}
		try (ByteArrayProvider provider = new ByteArrayProvider(createOverlayExecutable(false))) {
			assertFalse(AmigaHunkLoader.isManxOverlayExecutable(provider));
		}
	}

	@Test
	public void findsManxA4BaseWithoutRequiringAnOverlayTable() throws Exception {
		ProgramBuilder builder = new ProgramBuilder("manx-a4", "68000:BE:32:default");
		try {
			builder.createMemory("ram", "0", 0x400);
			builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0")), true);
			// Root branches to startup; startup calls LEA abs.l,A4; RTS.
			builder.setBytes("0x100", "600e");
			builder.setBytes("0x110", "4eb9000001204e75");
			builder.setBytes("0x120", "49f9000003004e75");

			Program program = builder.getProgram();
			Address base = AmigaHunkLoader.findManxA4Base(builder.addr("0x100"), program);
			assertEquals(builder.addr("0x300"), base);

			builder.setBytes("0x126", "4e71");
			assertEquals("The initializer must end in RTS", null,
					AmigaHunkLoader.findManxA4Base(builder.addr("0x100"), program));
		}
		finally {
			builder.dispose();
		}
	}

	@Test
	public void findsAndAppliesA4ContextInAuthenticPlainManxRuntime() throws Exception {
		byte[] executable = fixtureBytes("/fixtures/aztec-c/5.0a/runtime-a4-plain/manx-runtime-a4");
		try (ByteArrayProvider provider = new ByteArrayProvider(executable)) {
			HunkLoadSegFile hunkFile = BinFmtHunk.loadSegFile(
					new HunkBlockFile(new BinaryReader(provider, false), true), new MessageLog());
			assertEquals(1, hunkFile.getNodes().length);
			BinImage image = BinFmtHunk.createImage(hunkFile, new MessageLog());
			Segment[] segments = image.getSegments();
			int[] addresses = new int[segments.length];
			int nextAddress = 0x1000;
			for (Segment segment : segments) {
				addresses[segment.getId()] = nextAddress;
				nextAddress += segment.getSize();
			}

			List<byte[]> data = new Relocate(image).relocate(addresses);
			ProgramBuilder builder = new ProgramBuilder("plain-manx-a4", "68000:BE:32:default");
			try {
				builder.createMemory("ram", "0x1000", 0x10000);
				builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0x1000")), true);
				for (Segment segment : segments) {
					builder.setBytes(String.format("0x%x", addresses[segment.getId()]), toHex(data.get(segment.getId())));
				}

				Address a4Base = AmigaHunkLoader.findManxA4Base(builder.addr("0x1000"), builder.getProgram());
				assertNotNull(a4Base);
				int transaction = builder.getProgram().startTransaction("apply MANX A4 context");
				assertTrue(AmigaHunkLoader.applyA4Context(builder.getProgram(), a4Base, "MANX", new MessageLog()));
				builder.getProgram().endTransaction(transaction, true);
				assertEquals(a4Base.getOffset(), builder.getProgram().getProgramContext()
						.getRegisterValue(builder.getProgram().getRegister("A4"), builder.addr("0x1000"))
						.getUnsignedValue().longValue());
			}
			finally {
				builder.dispose();
			}
		}
	}

	@Test
	public void findsSasCLinkerDbInAuthenticSasCStartup() throws Exception {
		byte[] executable = fixtureBytes("/fixtures/sas-c/6.50/runtime-a4/sas-runtime-a4");
		try (ByteArrayProvider provider = new ByteArrayProvider(executable)) {
			HunkLoadSegFile hunkFile = BinFmtHunk.loadSegFile(
					new HunkBlockFile(new BinaryReader(provider, false), true), new MessageLog());
			BinImage image = BinFmtHunk.createImage(hunkFile, new MessageLog());
			Segment[] segments = image.getSegments();
			int[] addresses = new int[segments.length];
			int nextAddress = 0x1000;
			for (Segment segment : segments) {
				addresses[segment.getId()] = nextAddress;
				nextAddress += segment.getSize();
			}

			List<byte[]> data = new Relocate(image).relocate(addresses);
			ProgramBuilder builder = new ProgramBuilder("sas-c-a4", "68000:BE:32:default");
			try {
				builder.createMemory("ram", "0x1000", 0x10000);
				builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0x1000")), true);
				for (Segment segment : segments) {
					builder.setBytes(String.format("0x%x", addresses[segment.getId()]), toHex(data.get(segment.getId())));
				}

				assertEquals(builder.addr("0x14a4"),
						AmigaHunkLoader.findSasCLinkerDb(builder.addr("0x1000"), builder.getProgram()));
				int transaction = builder.getProgram().startTransaction("apply SAS/C A4 context");
				assertTrue(AmigaHunkLoader.applyA4Context(builder.getProgram(), builder.addr("0x14a4"),
						"SAS/C", new MessageLog()));
				builder.getProgram().endTransaction(transaction, true);
				assertEquals(0x14a4L, builder.getProgram().getProgramContext()
						.getRegisterValue(builder.getProgram().getRegister("A4"), builder.addr("0x1000"))
						.getUnsignedValue().longValue());
				builder.setBytes("0x1008", "4e71");
				assertEquals("A lone A4 setup is not SAS/C CRT evidence", null,
						AmigaHunkLoader.findSasCLinkerDb(builder.addr("0x1000"), builder.getProgram()));
			}
			finally {
				builder.dispose();
			}
		}
	}

	@Test
	public void appliesSasCA4ContextToAuthenticSlinkOverlayCode() throws Exception {
		byte[] executable = fixtureBytes("/fixtures/sas-c/6.50/runtime-a4-overlay/sas-runtime-a4-overlay");
		try (ByteArrayProvider provider = new ByteArrayProvider(executable)) {
			HunkLoadSegFile hunkFile = BinFmtHunk.loadSegFile(
					new HunkBlockFile(new BinaryReader(provider, false), true), new MessageLog());
			assertEquals(2, hunkFile.getNodes().length);
			assertTrue(hunkFile.getNodes()[1].isOverlay());
			BinImage image = BinFmtHunk.createImage(hunkFile, new MessageLog());
			Segment[] segments = image.getSegments();
			int[] addresses = new int[segments.length];
			int nextAddress = 0x1000;
			for (Segment segment : segments) {
				addresses[segment.getId()] = nextAddress;
				nextAddress += 0x1000;
			}

			List<byte[]> data = new Relocate(image).relocate(addresses);
			ProgramBuilder builder = new ProgramBuilder("sas-c-a4-overlay", "68000:BE:32:default");
			try {
				for (Segment segment : segments) {
					String address = String.format("0x%x", addresses[segment.getId()]);
					builder.createMemory("segment_" + segment.getId(), address, 0x1000);
					builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr(address)),
							segment.getType() == SegmentType.SEGMENT_TYPE_CODE);
					builder.setBytes(address, toHex(data.get(segment.getId())));
				}

				Address linkerDb = AmigaHunkLoader.findSasCLinkerDb(hunkFile, segments, toAddresses(builder, addresses),
						builder.getProgram());
				assertNotNull(linkerDb);
				int transaction = builder.getProgram().startTransaction("apply SAS/C A4 context to overlays");
				assertTrue(AmigaHunkLoader.applyA4Context(builder.getProgram(), linkerDb, "SAS/C", new MessageLog()));
				builder.getProgram().endTransaction(transaction, true);
				for (Segment segment : segments) {
					if (segment.getType() == SegmentType.SEGMENT_TYPE_CODE) {
						String address = String.format("0x%x", addresses[segment.getId()]);
						assertEquals(linkerDb.getOffset(), builder.getProgram().getProgramContext()
								.getRegisterValue(builder.getProgram().getRegister("A4"), builder.addr(address))
								.getUnsignedValue().longValue());
					}
				}
			}
			finally {
				builder.dispose();
			}
		}
	}

	@Test
	public void findsLatticeCLinkerDbInAuthenticLatticeC502Startup() throws Exception {
		byte[] executable = fixtureBytes("/fixtures/lattice-c/5.02/runtime-a4/lattice-runtime-a4");
		try (ByteArrayProvider provider = new ByteArrayProvider(executable)) {
			HunkLoadSegFile hunkFile = BinFmtHunk.loadSegFile(
					new HunkBlockFile(new BinaryReader(provider, false), true), new MessageLog());
			BinImage image = BinFmtHunk.createImage(hunkFile, new MessageLog());
			Segment[] segments = image.getSegments();
			int[] addresses = new int[segments.length];
			int nextAddress = 0x1000;
			for (Segment segment : segments) {
				addresses[segment.getId()] = nextAddress;
				nextAddress += segment.getSize();
			}

			List<byte[]> data = new Relocate(image).relocate(addresses);
			ProgramBuilder builder = new ProgramBuilder("lattice-c-5.02-a4", "68000:BE:32:default");
			try {
				builder.createMemory("ram", "0x1000", 0x10000);
				builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0x1000")), true);
				for (Segment segment : segments) {
					builder.setBytes(String.format("0x%x", addresses[segment.getId()]), toHex(data.get(segment.getId())));
				}

				Address linkerDb = builder.addr("0x1988");
				assertEquals(linkerDb, AmigaHunkLoader.findLatticeCLinkerDb(builder.addr("0x101c"), builder.getProgram()));
				assertEquals(linkerDb, AmigaHunkLoader.findLatticeCLinkerDb(hunkFile, segments,
						toAddresses(builder, addresses), builder.getProgram()));
				int transaction = builder.getProgram().startTransaction("apply Lattice C A4 context");
				assertTrue(AmigaHunkLoader.applyA4Context(builder.getProgram(), linkerDb, "Lattice C", new MessageLog()));
				builder.getProgram().endTransaction(transaction, true);
				assertEquals(linkerDb.getOffset(), builder.getProgram().getProgramContext()
						.getRegisterValue(builder.getProgram().getRegister("A4"), builder.addr("0x1000"))
						.getUnsignedValue().longValue());
				builder.setBytes("0x103c", "4e71");
				assertEquals("The CRT must retain its ExecBase fetch", null,
						AmigaHunkLoader.findLatticeCLinkerDb(builder.addr("0x101c"), builder.getProgram()));
			}
			finally {
				builder.dispose();
			}
		}
	}

	@Test
	public void appliesLatticeCA4ContextToAuthenticBlinkOverlayCode() throws Exception {
		byte[] executable = fixtureBytes("/fixtures/lattice-c/5.02/overlay/lattice-overlay");
		try (ByteArrayProvider provider = new ByteArrayProvider(executable)) {
			HunkLoadSegFile hunkFile = BinFmtHunk.loadSegFile(
					new HunkBlockFile(new BinaryReader(provider, false), true), new MessageLog());
			assertEquals(2, hunkFile.getNodes().length);
			assertTrue(hunkFile.getNodes()[1].isOverlay());
			assertEquals(hunkFile.getNodes()[0], hunkFile.getNodes()[1].getParent());
			assertEquals(1, hunkFile.getNodes()[1].getHierarchyLevel());
			assertEquals(1, hunkFile.getNodes()[1].getHierarchyOrdinate());
			BinImage image = BinFmtHunk.createImage(hunkFile, new MessageLog());
			Segment[] segments = image.getSegments();
			int[] addresses = new int[segments.length];
			int nextAddress = 0x1000;
			for (Segment segment : segments) {
				addresses[segment.getId()] = nextAddress;
				nextAddress += 0x1000;
			}

			List<byte[]> data = new Relocate(image).relocate(addresses);
			ProgramBuilder builder = new ProgramBuilder("lattice-c-5.02-overlay", "68000:BE:32:default");
			try {
				for (Segment segment : segments) {
					String address = String.format("0x%x", addresses[segment.getId()]);
					builder.createMemory("segment_" + segment.getId(), address, 0x1000);
					builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr(address)),
							segment.getType() == SegmentType.SEGMENT_TYPE_CODE);
					builder.setBytes(address, toHex(data.get(segment.getId())));
				}

				Address linkerDb = AmigaHunkLoader.findLatticeCLinkerDb(hunkFile, segments,
						toAddresses(builder, addresses), builder.getProgram());
				assertEquals(builder.addr("0x4000"), linkerDb);
				int transaction = builder.getProgram().startTransaction("apply Lattice C A4 context to overlays");
				assertTrue(AmigaHunkLoader.applyA4Context(builder.getProgram(), linkerDb, "Lattice C", new MessageLog()));
				builder.getProgram().endTransaction(transaction, true);
				for (Segment segment : segments) {
					if (segment.getType() == SegmentType.SEGMENT_TYPE_CODE) {
						String address = String.format("0x%x", addresses[segment.getId()]);
						assertEquals(linkerDb.getOffset(), builder.getProgram().getProgramContext()
								.getRegisterValue(builder.getProgram().getRegister("A4"), builder.addr(address))
								.getUnsignedValue().longValue());
					}
				}
			}
			finally {
				builder.dispose();
			}
		}
	}

	@Test
	public void findsLatticeCLinkerDbInAuthenticLatticeC500Startup() throws Exception {
		assertLatticeRuntimeA4Context("/fixtures/lattice-c/5.00/runtime-a4/lattice-runtime-a4", "Lattice C 5.00");
	}

	@Test
	public void appliesLatticeC500A4ContextToAuthenticBlinkOverlayCode() throws Exception {
		assertLatticeOverlayA4Context("/fixtures/lattice-c/5.00/overlay/lattice-overlay", "Lattice C 5.00");
	}

	@Test
	public void findsLatticeCLinkerDbInAuthenticLatticeC400Startup() throws Exception {
		assertLatticeRuntimeA4Context("/fixtures/lattice-c/4.00/runtime-a4/lattice-runtime-a4", "Lattice C 4.00");
	}

	@Test
	public void appliesLatticeC400A4ContextToAuthenticBlinkOverlayCode() throws Exception {
		assertLatticeOverlayA4Context("/fixtures/lattice-c/4.00/overlay/lattice-overlay", "Lattice C 4.00");
	}

	private static void assertLatticeRuntimeA4Context(String fixture, String profileName) throws Exception {
		byte[] executable = fixtureBytes(fixture);
		try (ByteArrayProvider provider = new ByteArrayProvider(executable)) {
			HunkLoadSegFile hunkFile = BinFmtHunk.loadSegFile(
					new HunkBlockFile(new BinaryReader(provider, false), true), new MessageLog());
			BinImage image = BinFmtHunk.createImage(hunkFile, new MessageLog());
			Segment[] segments = image.getSegments();
			int[] addresses = new int[segments.length];
			int nextAddress = 0x1000;
			for (Segment segment : segments) {
				addresses[segment.getId()] = nextAddress;
				nextAddress += segment.getSize();
			}

			List<byte[]> data = new Relocate(image).relocate(addresses);
			ProgramBuilder builder = new ProgramBuilder(profileName + " A4", "68000:BE:32:default");
			try {
				builder.createMemory("ram", "0x1000", 0x10000);
				builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr("0x1000")), true);
				for (Segment segment : segments) {
					builder.setBytes(String.format("0x%x", addresses[segment.getId()]), toHex(data.get(segment.getId())));
				}

				Address linkerDb = AmigaHunkLoader.findLatticeCLinkerDb(hunkFile, segments,
						toAddresses(builder, addresses), builder.getProgram());
				assertNotNull(profileName + " startup must expose BLINK's LinkerDB", linkerDb);
				int transaction = builder.getProgram().startTransaction("apply " + profileName + " A4 context");
				assertTrue(AmigaHunkLoader.applyA4Context(builder.getProgram(), linkerDb, profileName, new MessageLog()));
				builder.getProgram().endTransaction(transaction, true);
				assertEquals(linkerDb.getOffset(), builder.getProgram().getProgramContext()
						.getRegisterValue(builder.getProgram().getRegister("A4"), builder.addr("0x1000"))
						.getUnsignedValue().longValue());
			}
			finally {
				builder.dispose();
			}
		}
	}

	private static void assertLatticeOverlayA4Context(String fixture, String profileName) throws Exception {
		byte[] executable = fixtureBytes(fixture);
		try (ByteArrayProvider provider = new ByteArrayProvider(executable)) {
			HunkLoadSegFile hunkFile = BinFmtHunk.loadSegFile(
					new HunkBlockFile(new BinaryReader(provider, false), true), new MessageLog());
			assertEquals(2, hunkFile.getNodes().length);
			assertTrue(hunkFile.getNodes()[1].isOverlay());
			assertEquals(hunkFile.getNodes()[0], hunkFile.getNodes()[1].getParent());
			assertEquals(1, hunkFile.getNodes()[1].getHierarchyLevel());
			assertEquals(1, hunkFile.getNodes()[1].getHierarchyOrdinate());
			BinImage image = BinFmtHunk.createImage(hunkFile, new MessageLog());
			Segment[] segments = image.getSegments();
			int[] addresses = new int[segments.length];
			int nextAddress = 0x1000;
			for (Segment segment : segments) {
				addresses[segment.getId()] = nextAddress;
				nextAddress += 0x1000;
			}

			List<byte[]> data = new Relocate(image).relocate(addresses);
			ProgramBuilder builder = new ProgramBuilder(profileName + " overlay", "68000:BE:32:default");
			try {
				for (Segment segment : segments) {
					String address = String.format("0x%x", addresses[segment.getId()]);
					builder.createMemory("segment_" + segment.getId(), address, 0x1000);
					builder.setExecute(builder.getProgram().getMemory().getBlock(builder.addr(address)),
							segment.getType() == SegmentType.SEGMENT_TYPE_CODE);
					builder.setBytes(address, toHex(data.get(segment.getId())));
				}

				Address linkerDb = AmigaHunkLoader.findLatticeCLinkerDb(hunkFile, segments,
						toAddresses(builder, addresses), builder.getProgram());
				assertNotNull(profileName + " BLINK overlay must expose LinkerDB", linkerDb);
				int transaction = builder.getProgram().startTransaction("apply " + profileName + " A4 context to overlays");
				assertTrue(AmigaHunkLoader.applyA4Context(builder.getProgram(), linkerDb, profileName, new MessageLog()));
				builder.getProgram().endTransaction(transaction, true);
				for (Segment segment : segments) {
					if (segment.getType() == SegmentType.SEGMENT_TYPE_CODE) {
						String address = String.format("0x%x", addresses[segment.getId()]);
						assertEquals(linkerDb.getOffset(), builder.getProgram().getProgramContext()
								.getRegisterValue(builder.getProgram().getRegister("A4"), builder.addr(address))
								.getUnsignedValue().longValue());
					}
				}
			}
			finally {
				builder.dispose();
			}
		}
	}

	@Test
	public void declaresTheTwoByteManxStackAbi() throws Exception {
		File languageDirectory = new File("data/languages");
		Document language = parseXml(new File(languageDirectory, "68000_manx.ldefs"));
		Element languageElement = (Element) language.getElementsByTagName("language").item(0);
		assertEquals("68000:BE:32:MANX", languageElement.getAttribute("id"));
		Element compiler = (Element) language.getElementsByTagName("compiler").item(0);
		assertEquals("manx", compiler.getAttribute("id"));
		assertEquals("68000_manx.cspec", compiler.getAttribute("spec"));

		Document compilerSpec = parseXml(new File(languageDirectory, "68000_manx.cspec"));
		Element prototype = (Element) compilerSpec.getElementsByTagName("prototype").item(0);
		assertEquals("__manx", prototype.getAttribute("name"));
		Element parameterEntry = (Element) compilerSpec.getElementsByTagName("pentry").item(0);
		assertEquals("2", parameterEntry.getAttribute("align"));
		Element stackPointer = (Element) compilerSpec.getElementsByTagName("stackpointer").item(0);
		assertEquals("true", stackPointer.getAttribute("reversejustify"));
	}

	@Test
	public void recognisesManxRuntimeShapesWithoutImageAddresses() {
		assertEquals("mulu", ManxRuntimeAnalyzer.normalizeMnemonic("mulu.w"));
		assertEquals("movem", ManxRuntimeAnalyzer.normalizeMnemonic("movem.l"));
		assertEquals("dbf", ManxRuntimeAnalyzer.normalizeMnemonic("dbf"));
		assertEquals(ManxRuntimeAnalyzer.RuntimeHelper.MULTIPLY_LONG32_LOW,
				ManxRuntimeAnalyzer.classifyMnemonicSequence(List.of("movem", "move", "mulu", "move", "swap",
						"mulu", "swap", "clr", "add", "swap", "mulu", "swap", "clr", "add", "movem", "rts"), false, false));
		assertEquals(ManxRuntimeAnalyzer.RuntimeHelper.UNSIGNED_LONG_DIVISION_CORE,
				ManxRuntimeAnalyzer.classifyMnemonicSequence(List.of("movem", "swap", "tst", "bne", "swap", "move",
						"move", "clr", "swap", "divu", "move", "swap", "move", "divu", "move", "clr", "swap",
						"movem", "rts", "swap", "move", "move", "clr", "swap", "swap", "clr", "moveq", "add",
						"addx", "cmp", "bhi", "sub", "addq", "dbf", "movem", "rts"), false, false));
		assertEquals(ManxRuntimeAnalyzer.RuntimeHelper.DIVIDE_LONG32,
				ManxRuntimeAnalyzer.classifyMnemonicSequence(List.of("movem", "clr", "tst", "bpl", "neg", "addq",
						"tst", "bpl", "neg", "eori", "bsr", "tst", "beq", "neg", "movem", "tst", "rts"), true, false));
		assertEquals(ManxRuntimeAnalyzer.RuntimeHelper.MODULO_LONG32,
				ManxRuntimeAnalyzer.classifyMnemonicSequence(List.of("movem", "clr", "tst", "bpl", "neg", "addq",
						"tst", "bpl", "neg", "bsr", "move", "bra"), true, true));
		assertEquals(ManxRuntimeAnalyzer.RuntimeHelper.UNSIGNED_DIVIDE_LONG32,
				ManxRuntimeAnalyzer.classifyMnemonicSequence(List.of("move", "bsr", "move", "tst", "rts"), true,
						false));
		assertEquals(ManxRuntimeAnalyzer.RuntimeHelper.STRING_LENGTH,
				ManxRuntimeAnalyzer.classifyMnemonicSequence(List.of("movea", "move", "tst", "bne", "subq", "suba",
						"move", "rts"), false, false));
	}

	@Test
	public void recognisesConservativeAddressTakenCodeEvidence() {
		assertTrue(AddressTakenCodeAnalyzer.is68000Language("68000:BE:32:default"));
		assertTrue(AddressTakenCodeAnalyzer.is68000Language("68000:BE:32:MANX"));
		assertFalse(AddressTakenCodeAnalyzer.is68000Language("68020:BE:32:default"));
		assertTrue(AddressTakenCodeAnalyzer.isAddressInstalledIntoMemory("lea (-0x3fba,PC),A0",
				"move.l A0,(-0x1eec,A4)"));
		assertFalse(AddressTakenCodeAnalyzer.isAddressInstalledIntoMemory("lea (-0x3fba,PC),A0",
				"move.l A1,(-0x1eec,A4)"));
		assertFalse(AddressTakenCodeAnalyzer.isAddressInstalledIntoMemory("lea (-0x3fba,PC),A0", null));
	}

	@Test
	public void normalisesKnownAmigaApiNamesForAutomaticDiscovery() {
		assertEquals("graphics_library", AmigaHunkAnalyzer.toApiBaseName("graphics.library"));
		assertEquals("trackdisk_device", AmigaHunkAnalyzer.toApiBaseName("TRACKDISK.DEVICE"));
		assertEquals("potgo_resource", AmigaHunkAnalyzer.toApiBaseName("potgo.resource"));
		assertEquals(null, AmigaHunkAnalyzer.toApiBaseName("Music.task"));
	}

	private static Document parseXml(File file) throws Exception {
		return DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(file);
	}

	private static byte[] fixtureBytes(String resource) throws Exception {
		try (InputStream stream = AmigaHunkLoaderTest.class.getResourceAsStream(resource)) {
			assertTrue("Fixture is present: " + resource, stream != null);
			return stream.readAllBytes();
		}
	}

	private static String toHex(byte[] data) {
		StringBuilder result = new StringBuilder(data.length * 2);
		for (byte value : data) {
			result.append(String.format("%02x", value));
		}
		return result.toString();
	}

	private static Address[] toAddresses(ProgramBuilder builder, int[] addresses) {
		Address[] result = new Address[addresses.length];
		for (int index = 0; index < addresses.length; index++) {
			result[index] = builder.addr(String.format("0x%x", addresses[index]));
		}
		return result;
	}

	private static byte[] createOverlayExecutable(boolean manx) throws Exception {
		ByteArrayOutputStream bytes = new ByteArrayOutputStream();
		try (DataOutputStream output = new DataOutputStream(bytes)) {
			writeHeader(output, 3, 0, 1);
			writeCodeSegment(output, 0x4e75); // resident startup
			writeCodeSegment(output, 0x4e75); // resident trampoline area
			output.writeInt(HunkType.HUNK_OVERLAY.getValue());
			if (manx) {
				output.writeInt(4); // table length excludes its final long-word
				output.writeInt(1); // one overlay node
				output.writeInt(0); // file position is not needed for ABI detection
				output.writeShort(0); // trampoline offset
				output.writeShort(8); // descriptor list starts at byte 12
				output.writeShort(2); // overlay segment
				output.writeShort(1); // one trampoline
				output.writeInt(0); // terminator
			} else {
				output.writeInt(1); // table length excludes its final long-word
				output.writeInt(0);
				output.writeInt(0); // not a valid MANX table
			}
			writeHeader(output, 3, 2, 2);
			writeCodeSegment(output, 0x4e75);
			output.writeInt(HunkType.HUNK_BREAK.getValue());
		}
		return bytes.toByteArray();
	}

	private static void writeHeader(DataOutputStream output, int tableSize, int firstHunk, int lastHunk)
			throws Exception {
		output.writeInt(HunkType.HUNK_HEADER.getValue());
		output.writeInt(0);
		output.writeInt(tableSize);
		output.writeInt(firstHunk);
		output.writeInt(lastHunk);
		for (int hunk = firstHunk; hunk <= lastHunk; hunk++) {
			output.writeInt(1);
		}
	}

	private static void writeCodeSegment(DataOutputStream output, int instruction) throws Exception {
		output.writeInt(HunkType.HUNK_CODE.getValue());
		output.writeInt(1);
		output.writeInt(instruction);
		output.writeInt(HunkType.HUNK_END.getValue());
	}
}
