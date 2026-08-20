package hunk;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertSame;
import static org.junit.Assert.assertThrows;
import static org.junit.Assert.assertTrue;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.List;

import org.junit.Test;
import org.junit.Assume;

import ghidra.app.util.bin.BinaryReader;
import ghidra.app.util.bin.ByteArrayProvider;
import ghidra.app.util.bin.FileByteProvider;

public class HunkOverlayFileTest {

	@Test
	public void parsesGeneratedAztecSegmentedExecutable() throws Exception {
		byte[] bytes;
		try (InputStream stream = getClass().getResourceAsStream(
				"/fixtures/aztec-c/5.0a/segload-smoke/segload-smoke")) {
			assertTrue("Generated Aztec fixture is present", stream != null);
			bytes = stream.readAllBytes();
		}
		try (ByteArrayProvider provider = new ByteArrayProvider(bytes)) {
			HunkLoadSegFile file = new HunkLoadSegFile();
			file.parseBlockFile(new HunkBlockFile(new BinaryReader(provider, false), true));

			assertEquals(4, file.getSegments().length);
			assertEquals(2, file.getNodes().length);
			assertTrue(file.getNodes()[1].isOverlay());
			assertEquals(1, file.getManxOverlayTable().getNodes().size());
			HunkManxOverlayTable.Node overlay = file.getManxOverlayTable().getNodes().get(0);
			assertEquals(1, overlay.getSegments().size());
			assertEquals(1, overlay.getSegments().get(0).getTrampolineCount());
		}
	}

	@Test
	public void preservesMultipleTargetsInGeneratedAztecSegmentedExecutable() throws Exception {
		byte[] bytes;
		try (InputStream stream = getClass().getResourceAsStream(
				"/fixtures/aztec-c/5.0a/segload-multi/segload-multi")) {
			assertTrue("Generated Aztec multi-target fixture is present", stream != null);
			bytes = stream.readAllBytes();
		}
		try (ByteArrayProvider provider = new ByteArrayProvider(bytes)) {
			HunkLoadSegFile file = new HunkLoadSegFile();
			file.parseBlockFile(new HunkBlockFile(new BinaryReader(provider, false), true));

			assertEquals(2, file.getNodes().length);
			HunkManxOverlayTable.Node overlay = file.getManxOverlayTable().getNodes().get(0);
			assertEquals(1, overlay.getSegments().size());
			assertEquals(2, overlay.getSegments().get(0).getTrampolineCount());
		}
	}

	@Test
	public void doesNotMistakeEmbeddedHunkTagsInCommodoreALinkForAnOverlay() throws Exception {
		byte[] bytes;
		try (InputStream stream = getClass().getResourceAsStream("/fixtures/commodore-alink/ALINK")) {
			assertTrue("Commodore ALink fixture is present", stream != null);
			bytes = stream.readAllBytes();
		}
		try (ByteArrayProvider provider = new ByteArrayProvider(bytes)) {
			HunkLoadSegFile file = new HunkLoadSegFile();
			file.parseBlockFile(new HunkBlockFile(new BinaryReader(provider, false), true));

			assertEquals("ALink is a single-node executable", 1, file.getNodes().length);
			assertEquals("ALink root HUNK_HEADER has six segments", 6, file.getSegments().length);
			assertEquals("Hunk-type values inside segment bytes are not overlay blocks", null,
					file.getOverlayTableData());
		}
	}

	@Test
	public void parsesSasCBasicHierarchicalOverlayExecutable() throws Exception {
		HunkLoadSegFile file = parseFixture("/fixtures/sas-c/6.50/basic-overlay/sas-basic");

		assertEquals(2, file.getNodes().length);
		assertTrue(file.getNodes()[1].isOverlay());
		assertEquals(4, file.getSegments().length);
		assertEquals(3, file.getOverlayTable().getTreeDepth());
		assertEquals(1, file.getOverlayTable().getSymbols().size());
		HunkOverlayTable.Symbol symbol = file.getOverlayTable().getSymbols().get(0);
		assertEquals(3, symbol.getFirstSegment());
		assertEquals(3, symbol.getSymbolSegment());
		assertEquals(4, symbol.getSymbolOffset());
	}

	@Test
	public void preservesMultipleSasCOverlayTargets() throws Exception {
		HunkLoadSegFile file = parseFixture("/fixtures/sas-c/6.50/multi-target-overlay/sas-multi");

		assertEquals(2, file.getNodes().length);
		assertEquals(2, file.getOverlayTable().getSymbols().size());
		assertEquals(3, file.getOverlayTable().getSymbols().get(0).getSymbolSegment());
		assertEquals(3, file.getOverlayTable().getSymbols().get(1).getSymbolSegment());
	}

	@Test
	public void preservesSasCNestedOverlayHierarchy() throws Exception {
		HunkLoadSegFile file = parseFixture("/fixtures/sas-c/6.50/nested-overlay/sas-nested");

		assertEquals(3, file.getNodes().length);
		assertTrue(file.getNodes()[1].isOverlay());
		assertTrue(file.getNodes()[2].isOverlay());
		assertEquals(2, file.getOverlayTable().getSymbols().size());
	}

	@Test
	public void parsesRootAndOverlayNodes() throws Exception {
		ByteArrayOutputStream bytes = new ByteArrayOutputStream();
		try (DataOutputStream output = new DataOutputStream(bytes)) {
			writeHeader(output, 2, 0, 0);
			writeCodeSegment(output, 0x4e75);
			output.writeInt(HunkType.HUNK_OVERLAY.getValue());
			// l=9 describes ten table words: depth, one ordinate, and one symbol entry.
			output.writeInt(9);
			output.writeInt(2); // tree depth
			output.writeInt(0); // currently loaded ordinate at level 1
			output.writeInt(0x58); // file offset of the following HUNK_HEADER
			output.writeInt(0); // reserved
			output.writeInt(0); // reserved
			output.writeInt(1); // level
			output.writeInt(1); // ordinate
			output.writeInt(1); // first segment
			output.writeInt(1); // target segment
			output.writeInt(0); // target offset
			writeHeader(output, 2, 1, 1);
			writeCodeSegment(output, 0x4e75);
			output.writeInt(HunkType.HUNK_BREAK.getValue());
		}

		try (ByteArrayProvider provider = new ByteArrayProvider(bytes.toByteArray())) {
			HunkBlockFile blockFile = new HunkBlockFile(new BinaryReader(provider, false), true);
			HunkLoadSegFile loadSegFile = new HunkLoadSegFile();
			loadSegFile.parseBlockFile(blockFile);

			assertEquals(2, loadSegFile.getSegments().length);
			assertEquals(2, loadSegFile.getNodes().length);
			assertFalse(loadSegFile.getNodes()[0].isOverlay());
			assertTrue(loadSegFile.getNodes()[1].isOverlay());
			assertEquals(0, loadSegFile.getNodes()[0].getFirstHunk());
			assertEquals(1, loadSegFile.getNodes()[1].getFirstHunk());
			assertEquals(2, loadSegFile.getOverlayTable().getTreeDepth());
			assertEquals(1, loadSegFile.getOverlayTable().getSymbols().size());
			assertEquals(0x58, loadSegFile.getOverlayTable().getSymbols().get(0).getFilePosition());
			assertEquals(0x58, loadSegFile.getNodes()[1].getHeaderFileOffset());
		}
	}

	@Test
	public void keepsSiblingOverlaySlotsPhysicallyDistinctAndResolvesRelocationsByPath() throws Exception {
		byte[] root = rootNodeBytes();
		byte[] levelOne = overlayNodeBytes(1, false);
		byte[] firstLevelTwo = overlayNodeBytes(2, true);
		byte[] secondLevelTwo = overlayNodeBytes(2, false);
		byte[] levelThree = overlayNodeBytes(3, true);
		int overlayTableBytes = (4 + 4 * 8) * Integer.BYTES;
		int levelOneOffset = root.length + 2 * Integer.BYTES + overlayTableBytes;
		int firstLevelTwoOffset = levelOneOffset + levelOne.length;
		int secondLevelTwoOffset = firstLevelTwoOffset + firstLevelTwo.length;
		int levelThreeOffset = secondLevelTwoOffset + secondLevelTwo.length;

		ByteArrayOutputStream bytes = new ByteArrayOutputStream();
		try (DataOutputStream output = new DataOutputStream(bytes)) {
			output.write(root);
			writeHierarchicalOverlayTable(output, levelOneOffset, firstLevelTwoOffset,
					secondLevelTwoOffset, levelThreeOffset);
			output.write(levelOne);
			output.write(firstLevelTwo);
			output.write(secondLevelTwo);
			output.write(levelThree);
		}

		try (ByteArrayProvider provider = new ByteArrayProvider(bytes.toByteArray())) {
			HunkLoadSegFile file = new HunkLoadSegFile();
			file.parseBlockFile(new HunkBlockFile(new BinaryReader(provider, false), true));

			assertEquals(4, file.getLogicalSlotCount());
			assertEquals(5, file.getSegments().length);
			assertEquals(5, file.getNodes().length);
			assertEquals(2, file.getNodes()[2].getFirstHunk());
			assertEquals(2, file.getNodes()[3].getFirstHunk());
			assertSame(file.getNodes()[3], file.getNodes()[4].getParent());
			assertSame(file.getNodes()[2].getSegments()[0], file.resolveLogicalSlot(file.getNodes()[2], 2));
			assertSame(file.getNodes()[3].getSegments()[0], file.resolveLogicalSlot(file.getNodes()[4], 2));

			BinImage image = BinFmtHunk.createImage(file, new ghidra.app.util.importer.MessageLog());
			assertNotNull(image);
			Segment[] segments = image.getSegments();
			assertEquals(5, segments.length);
			assertSame(segments[2], segments[2].getRelocationsToSegments()[0]);
			assertSame(segments[3], segments[4].getRelocationsToSegments()[0]);
			List<byte[]> relocated = new Relocate(image).relocate(new int[] { 0x1000, 0x1100, 0x1200, 0x1300, 0x1400 });
			assertEquals(0x1200, readInt(relocated.get(2), 0));
			assertEquals(0x1300, readInt(relocated.get(4), 0));
		}
	}

	@Test
	public void parsesManxOverlayTable() throws Exception {
		ByteArrayOutputStream bytes = new ByteArrayOutputStream();
		try (DataOutputStream output = new DataOutputStream(bytes)) {
			output.writeInt(1);       // one overlay node
			output.writeInt(0x80);    // HUNK_HEADER file offset
			output.writeShort(0x20);  // trampoline offset in root segment 1
			output.writeShort(0x08);  // symbol table offset from first node record
			output.writeShort(3);     // target segment
			output.writeShort(2);     // two trampolines in that segment
			output.writeShort(0);     // terminator segment
			output.writeShort(0);
		}

		HunkManxOverlayTable table = HunkManxOverlayTable.parse(bytes.toByteArray());
		assertEquals(1, table.getNodes().size());
		HunkManxOverlayTable.Node node = table.getNodes().get(0);
		assertEquals(0x80, node.getFilePosition());
		assertEquals(0x20, node.getTrampolineOffset());
		assertEquals(3, node.getSegments().get(0).getSegment());
		assertEquals(2, node.getSegments().get(0).getTrampolineCount());
	}

	@Test
	public void rejectsMalformedManxOverlayTables() {
		byte[] unterminatedDescriptorList = {
				0, 0, 0, 1,             // one node
				0, 0, 0, (byte) 0x80,   // HUNK_HEADER file offset
				0, 0x20, 0, 8,           // trampoline and symbol table offsets
				0, 3, 0, 1               // descriptor with no terminating segment-zero entry
		};
		assertThrows(HunkParseError.class, () -> HunkManxOverlayTable.parse(unterminatedDescriptorList));

		byte[] invalidSymbolOffset = {
				0, 0, 0, 1,
				0, 0, 0, (byte) 0x80,
				0, 0x20, 0, 0            // points into the node records, not a descriptor list
		};
		assertThrows(HunkParseError.class, () -> HunkManxOverlayTable.parse(invalidSymbolOffset));

		byte[] descriptorListCrossesIntoNextNode = {
				0, 0, 0, 2,             // two nodes
				0, 0, 0, (byte) 0x80,
				0, 0x20, 0, 16,          // first descriptor list begins at byte 20
				0, 0, 0, (byte) 0x90,
				0, 0x28, 0, 20,          // second descriptor list begins at byte 24
				0, 3, 0, 1,              // unterminated first list
				0, 0, 0, 0               // terminator belonging only to the second list
		};
		assertThrows(HunkParseError.class, () -> HunkManxOverlayTable.parse(descriptorListCrossesIntoNextNode));
	}

	@Test
	public void rejectsInvalidHeaderRange() throws Exception {
		ByteArrayOutputStream bytes = new ByteArrayOutputStream();
		try (DataOutputStream output = new DataOutputStream(bytes)) {
			output.writeInt(HunkType.HUNK_HEADER.getValue());
			output.writeInt(0);
			output.writeInt(1);
			output.writeInt(1); // last hunk precedes first hunk
			output.writeInt(0);
		}
		try (ByteArrayProvider provider = new ByteArrayProvider(bytes.toByteArray())) {
			assertThrows(HunkParseError.class,
					() -> new HunkBlockFile(new BinaryReader(provider, false), true));
		}
	}

	@Test
	public void relocatesPcRelativeFieldsAtTheirDeclaredWidths() throws Exception {
		BinImage image = new BinImage();
		Segment source = new Segment(SegmentType.SEGMENT_TYPE_CODE, 8,
				new byte[] { 0, 0, 0, 0, 0, 0, 0, 0 }, 0);
		Segment target = new Segment(SegmentType.SEGMENT_TYPE_CODE, 4,
				new byte[] { 0, 0, 0, 0 }, 1);
		image.addSegment(source);
		image.addSegment(target);
		source.addRelocations(target, List.of(
				new Reloc(0, 2, Reloc.Kind.PC_RELATIVE),
				new Reloc(2, 1, Reloc.Kind.PC_RELATIVE),
				new Reloc(4, 4, Reloc.Kind.PC_RELATIVE)));

		List<byte[]> relocated = new Relocate(image).relocate(new int[] { 0x1000, 0x1010 });
		byte[] data = relocated.get(0);
		assertEquals(0x10, ((data[0] & 0xff) << 8) | (data[1] & 0xff));
		assertEquals(0x0e, data[2] & 0xff);
		assertEquals(0x0000000c, ((data[4] & 0xff) << 24) | ((data[5] & 0xff) << 16) |
				((data[6] & 0xff) << 8) | (data[7] & 0xff));
	}

	@Test
	public void parsesPcRelativeRelocationHunks() throws Exception {
		ByteArrayOutputStream bytes = new ByteArrayOutputStream();
		try (DataOutputStream output = new DataOutputStream(bytes)) {
			writeHeader(output, 1, 0, 0);
			output.writeInt(HunkType.HUNK_CODE.getValue());
			output.writeInt(1);
			output.writeInt(0);
			output.writeInt(HunkType.HUNK_RELRELOC32.getValue());
			output.writeInt(1); // one relocation
			output.writeInt(0); // target hunk
			output.writeInt(0); // relocation field offset
			output.writeInt(0); // end relocation groups
			output.writeInt(HunkType.HUNK_END.getValue());
		}

		try (ByteArrayProvider provider = new ByteArrayProvider(bytes.toByteArray())) {
			HunkLoadSegFile file = new HunkLoadSegFile();
			file.parseBlockFile(new HunkBlockFile(new BinaryReader(provider, false), true));
			HunkRelocBlock[] blocks = file.getSegments()[0].getRelocBlocks();
			assertEquals(1, blocks.length);
			Reloc relocation = blocks[0].getRelocs()[0].getRelocs()[0];
			assertEquals(4, relocation.getWidth());
			assertEquals(Reloc.Kind.PC_RELATIVE, relocation.getKind());
		}
	}

	@Test
	public void rejectsRelocationsThatDoNotFitTheirField() {
		BinImage image = new BinImage();
		Segment source = new Segment(SegmentType.SEGMENT_TYPE_CODE, 2, new byte[2], 0);
		Segment target = new Segment(SegmentType.SEGMENT_TYPE_CODE, 2, new byte[2], 1);
		image.addSegment(source);
		image.addSegment(target);
		source.addRelocations(target, List.of(new Reloc(0, 1, Reloc.Kind.PC_RELATIVE)));

		assertThrows(HunkParseError.class, () -> new Relocate(image).relocate(new int[] { 0, 0x100 }));
	}

	@Test
	public void appliesAbsoluteRelocationAddendsWithNativeFieldWidthWrapping() throws Exception {
		BinImage image = new BinImage();
		Segment source = new Segment(SegmentType.SEGMENT_TYPE_CODE, 4,
				new byte[] { (byte) 0xff, (byte) 0xff, (byte) 0xff, (byte) 0xfe }, 0);
		Segment target = new Segment(SegmentType.SEGMENT_TYPE_CODE, 4, new byte[4], 1);
		image.addSegment(source);
		image.addSegment(target);
		source.addRelocations(target, List.of(new Reloc(0, 4, Reloc.Kind.ABSOLUTE)));

		List<byte[]> relocated = new Relocate(image).relocate(new int[] { 0, 0x20000 });
		assertEquals(0x0001fffe, readInt(relocated.get(0), 0));
	}

	/**
	 * Optional integration test for a real overlay executable.  It is deliberately
	 * opt-in so the public test suite never depends on a user's game collection.
	 */
	@Test
	public void validatesOptInLocalOverlayExecutable() throws Exception {
		String path = System.getProperty("test.hunk.overlay.path");
		if (path == null || path.isBlank()) {
			path = System.getenv("TEST_HUNK_OVERLAY_PATH");
		}
		Assume.assumeTrue(path != null && !path.isBlank());
		String expectedHash = System.getProperty("test.hunk.overlay.sha256");
		if (expectedHash == null || expectedHash.isBlank()) {
			expectedHash = System.getenv("TEST_HUNK_OVERLAY_SHA256");
		}
		if (expectedHash != null && !expectedHash.isBlank()) {
			assertEquals(expectedHash.replaceAll("\\s", "").toUpperCase(), sha256(Path.of(path)));
		}
		try (FileByteProvider provider = new FileByteProvider(new java.io.File(path), null, java.nio.file.AccessMode.READ)) {
			HunkBlockFile blockFile = new HunkBlockFile(new BinaryReader(provider, false), true);
			HunkLoadSegFile loadSegFile = new HunkLoadSegFile();
			loadSegFile.parseBlockFile(blockFile);

			assertTrue(loadSegFile.getNodes().length > 1);
			assertTrue(loadSegFile.getOverlayTableData().length > 0);
			assertTrue("Configured executable must use hierarchical or MANX overlay metadata",
					loadSegFile.getOverlayTable() != null || loadSegFile.getManxOverlayTable() != null);
			if (loadSegFile.getManxOverlayTable() != null) {
				byte[] rootTrampolines = loadSegFile.getSegments()[1].getSegmentBlock().getData();
				Integer nodeIndexBias = null;
				int nodeIndex = 0;
				for (HunkManxOverlayTable.Node entry : loadSegFile.getManxOverlayTable().getNodes()) {
					HunkLoadSegFile.Node matchingNode = null;
					for (HunkLoadSegFile.Node node : loadSegFile.getNodes()) {
						if (node.isOverlay() && node.getHeaderFileOffset() == entry.getFilePosition()) {
							matchingNode = node;
							break;
						}
					}
					assertTrue("MANX HUNK_OVERLAY entry must identify a matching overlay node", matchingNode != null);
					int trampolineNumber = 0;
					for (HunkManxOverlayTable.SegmentDescriptor descriptor : entry.getSegments()) {
						assertTrue("MANX symbol segment must belong to its overlay node",
								descriptor.getSegment() >= matchingNode.getFirstHunk() && descriptor.getSegment() <= matchingNode.getLastHunk());
						for (int trampoline = 0; trampoline < descriptor.getTrampolineCount(); trampoline++, trampolineNumber++) {
							int offset = entry.getTrampolineOffset() + trampolineNumber * 8;
							assertTrue("MANX trampoline must fit the root trampoline segment", offset + 8 <= rootTrampolines.length);
							int opcode = ((rootTrampolines[offset] & 0xff) << 8) | (rootTrampolines[offset + 1] & 0xff);
							int encodedNode = rootTrampolines[offset + 4] & 0xff;
							int symbolOffset = ((rootTrampolines[offset + 5] & 0xff) << 16) |
									((rootTrampolines[offset + 6] & 0xff) << 8) | (rootTrampolines[offset + 7] & 0xff);
							assertEquals("MANX trampoline opcode", 0x6100, opcode);
							int candidateBias = encodedNode - nodeIndex;
							assertTrue("MANX trampoline node must use a documented or legacy one-based index",
									candidateBias == 0 || candidateBias == 1);
							if (nodeIndexBias == null) {
								nodeIndexBias = candidateBias;
							} else {
								assertEquals("MANX trampoline node numbering must be consistent table-wide",
										nodeIndexBias.intValue(), candidateBias);
							}
							assertTrue("MANX trampoline target must fit its declared segment",
									symbolOffset < loadSegFile.getSegments()[descriptor.getSegment()].getSizeLongs() * 4);
						}
					}
					nodeIndex++;
				}
			}
			BinImage image = BinFmtHunk.createImage(loadSegFile, new ghidra.app.util.importer.MessageLog());
			assertNotNull("Configured executable must produce an image", image);
			new Relocate(image).relocate(new int[image.getSegments().length]);
		}
	}

	private static byte[] rootNodeBytes() throws Exception {
		ByteArrayOutputStream bytes = new ByteArrayOutputStream();
		try (DataOutputStream output = new DataOutputStream(bytes)) {
			writeHeader(output, 4, 0, 0);
			writeCodeSegment(output, 0x4e75);
		}
		return bytes.toByteArray();
	}

	private static byte[] overlayNodeBytes(int slot, boolean relocatesToSlotTwo) throws Exception {
		ByteArrayOutputStream bytes = new ByteArrayOutputStream();
		try (DataOutputStream output = new DataOutputStream(bytes)) {
			writeHeader(output, 4, slot, slot);
			if (relocatesToSlotTwo) {
				writeAbsoluteRelocatingCodeSegment(output, 2);
			} else {
				writeCodeSegment(output, 0x4e75);
			}
			output.writeInt(HunkType.HUNK_BREAK.getValue());
		}
		return bytes.toByteArray();
	}

	private static void writeHierarchicalOverlayTable(DataOutputStream output, int levelOneOffset,
			int firstLevelTwoOffset, int secondLevelTwoOffset, int levelThreeOffset) throws Exception {
		output.writeInt(HunkType.HUNK_OVERLAY.getValue());
		output.writeInt(35); // 36 longwords, including the tree-depth word.
		output.writeInt(4); // root plus three overlay levels
		output.writeInt(0);
		output.writeInt(0);
		output.writeInt(0);
		writeOverlaySymbol(output, levelOneOffset, 1, 1, 1, 1);
		writeOverlaySymbol(output, firstLevelTwoOffset, 2, 1, 2, 2);
		writeOverlaySymbol(output, secondLevelTwoOffset, 2, 2, 2, 2);
		writeOverlaySymbol(output, levelThreeOffset, 3, 1, 3, 3);
	}

	private static void writeOverlaySymbol(DataOutputStream output, int fileOffset, int level, int ordinate,
			int firstSegment, int targetSegment) throws Exception {
		output.writeInt(fileOffset);
		output.writeInt(0);
		output.writeInt(0);
		output.writeInt(level);
		output.writeInt(ordinate);
		output.writeInt(firstSegment);
		output.writeInt(targetSegment);
		output.writeInt(0);
	}

	private static void writeAbsoluteRelocatingCodeSegment(DataOutputStream output, int targetSlot) throws Exception {
		output.writeInt(HunkType.HUNK_CODE.getValue());
		output.writeInt(1);
		output.writeInt(0);
		output.writeInt(HunkType.HUNK_ABSRELOC32.getValue());
		output.writeInt(1);
		output.writeInt(targetSlot);
		output.writeInt(0);
		output.writeInt(0);
		output.writeInt(HunkType.HUNK_END.getValue());
	}

	private static int readInt(byte[] data, int offset) {
		return ((data[offset] & 0xff) << 24) | ((data[offset + 1] & 0xff) << 16) |
				((data[offset + 2] & 0xff) << 8) | (data[offset + 3] & 0xff);
	}

	private static String sha256(Path path) throws Exception {
		MessageDigest digest = MessageDigest.getInstance("SHA-256");
		try (InputStream input = Files.newInputStream(path)) {
			byte[] buffer = new byte[8192];
			for (int read; (read = input.read(buffer)) != -1;) {
				digest.update(buffer, 0, read);
			}
		}
		byte[] hash = digest.digest();
		StringBuilder text = new StringBuilder(hash.length * 2);
		for (byte value : hash) {
			text.append(String.format("%02X", value));
		}
		return text.toString();
	}

	private static void writeHeader(DataOutputStream output, int tableSize, int firstHunk, int lastHunk) throws Exception {
		output.writeInt(HunkType.HUNK_HEADER.getValue());
		output.writeInt(0);
		output.writeInt(tableSize);
		output.writeInt(firstHunk);
		output.writeInt(lastHunk);
		for (int hunk = firstHunk; hunk <= lastHunk; ++hunk) {
			output.writeInt(1);
		}
	}

	private static void writeCodeSegment(DataOutputStream output, int instruction) throws Exception {
		output.writeInt(HunkType.HUNK_CODE.getValue());
		output.writeInt(1);
		output.writeInt(instruction);
		output.writeInt(HunkType.HUNK_END.getValue());
	}

	private HunkLoadSegFile parseFixture(String resource) throws Exception {
		byte[] bytes;
		try (InputStream stream = getClass().getResourceAsStream(resource)) {
			assertTrue("Fixture is present: " + resource, stream != null);
			bytes = stream.readAllBytes();
		}
		try (ByteArrayProvider provider = new ByteArrayProvider(bytes)) {
			HunkLoadSegFile file = new HunkLoadSegFile();
			file.parseBlockFile(new HunkBlockFile(new BinaryReader(provider, false), true));
			return file;
		}
	}
}
