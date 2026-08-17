package amiga;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.util.List;

import javax.xml.parsers.DocumentBuilderFactory;

import org.junit.Test;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import ghidra.app.util.bin.ByteArrayProvider;
import hunk.HunkType;

public class AmigaHunkLoaderTest {

	@Test
	public void calculates68000BranchTargetsRelativeToTheOpcodeSuccessor() {
		assertEquals(0x18, AmigaHunkLoader.getBranchTargetDelta(0x6000, (short) 0x16));
		assertEquals(0x72, AmigaHunkLoader.getBranchTargetDelta(0x6170, (short) 0));
		assertEquals(1, AmigaHunkLoader.getBranchTargetDelta(0x60ff, (short) 0));
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

	private static Document parseXml(File file) throws Exception {
		return DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(file);
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
