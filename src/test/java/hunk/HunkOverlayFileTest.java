package hunk;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertThrows;
import static org.junit.Assert.assertTrue;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.util.List;

import org.junit.Test;
import org.junit.Assume;

import ghidra.app.util.bin.BinaryReader;
import ghidra.app.util.bin.ByteArrayProvider;
import ghidra.app.util.bin.FileByteProvider;

public class HunkOverlayFileTest {

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
		try (FileByteProvider provider = new FileByteProvider(new java.io.File(path), null, java.nio.file.AccessMode.READ)) {
			HunkBlockFile blockFile = new HunkBlockFile(new BinaryReader(provider, false), true);
			HunkLoadSegFile loadSegFile = new HunkLoadSegFile();
			loadSegFile.parseBlockFile(blockFile);

			assertTrue(loadSegFile.getNodes().length > 1);
			assertTrue(loadSegFile.getOverlayTableData().length > 0);
			org.junit.Assert.assertNotNull("Configured executable is expected to use the documented MANX overlay manager", loadSegFile.getManxOverlayTable());
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
		}
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
}
