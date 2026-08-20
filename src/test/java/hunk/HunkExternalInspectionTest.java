package hunk;

import static org.junit.Assert.assertTrue;

import java.nio.ByteBuffer;
import java.nio.file.Files;
import java.nio.file.Path;

import org.junit.Assume;
import org.junit.Test;

import ghidra.app.util.bin.BinaryReader;
import ghidra.app.util.bin.ByteArrayProvider;
import ghidra.app.util.importer.MessageLog;

/**
 * Opt-in diagnostic for an external LoadSeg file.  It deliberately reads the
 * original Hunk payload, rather than an imported Program, so it can establish
 * the raw value and relocation target behind an address rendered by Ghidra.
 *
 * <p>Run it only when needed, for example:
 * <pre>
 * $env:HUNK_INSPECT_FILE = 'C:\\path\\to\\program'
 * $env:HUNK_INSPECT_SEGMENT = '1'
 * $env:HUNK_INSPECT_OFFSET = '0x1d26'
 * .\\gradlew.bat test --tests hunk.HunkExternalInspectionTest
 * </pre>
 * Segment indexes are physical {@link BinImage} segment IDs. The diagnostic
 * prints the complete index first, including each segment's logical Hunk slot
 * and overlay-node ownership.
 */
public class HunkExternalInspectionTest {

	private static final String FILE_VARIABLE = "HUNK_INSPECT_FILE";
	private static final String SEGMENT_VARIABLE = "HUNK_INSPECT_SEGMENT";
	private static final String OFFSET_VARIABLE = "HUNK_INSPECT_OFFSET";
	private static final int CONTEXT_BYTES = 16;

	@Test
	public void reportsRawPayloadAndRelocationForConfiguredExternalHunk() throws Exception {
		String fileName = System.getenv(FILE_VARIABLE);
		Assume.assumeTrue("Set " + FILE_VARIABLE + " to inspect an external Hunk file", fileName != null && !fileName.isBlank());

		Path file = Path.of(fileName);
		assertTrue("External Hunk file does not exist: " + file, Files.isRegularFile(file));
		int segmentId = parseNonNegativeEnvironment(SEGMENT_VARIABLE);
		int payloadOffset = parseNonNegativeEnvironment(OFFSET_VARIABLE);

		byte[] executable = Files.readAllBytes(file);
		try (ByteArrayProvider provider = new ByteArrayProvider(executable)) {
			HunkLoadSegFile hunkFile = BinFmtHunk.loadSegFile(
					new HunkBlockFile(new BinaryReader(provider, false), true), new MessageLog());
			BinImage image = BinFmtHunk.createImage(hunkFile, new MessageLog());
			Segment[] segments = image.getSegments();

			printSegmentIndex(segments);
			assertTrue("Physical segment index " + segmentId + " is outside 0.." + (segments.length - 1),
					segmentId < segments.length);
			Segment source = segments[segmentId];
			byte[] payload = source.getData();
			assertTrue("Physical segment " + segmentId + " is BSS and has no file payload", payload != null);
			assertTrue("Payload offset 0x" + Integer.toHexString(payloadOffset) + " is outside segment " + segmentId +
					" (size 0x" + Integer.toHexString(payload.length) + ")", payloadOffset < payload.length);

			System.out.printf("%nInspection: physical segment %d (%s), payload offset 0x%X%n",
					source.getId(), source.getName(), payloadOffset);
			printPayloadContext(payload, payloadOffset);
			printCoveringRelocations(source, payloadOffset, payload);
		}
	}

	private static int parseNonNegativeEnvironment(String variable) {
		String value = System.getenv(variable);
		Assume.assumeTrue("Set " + variable + " (decimal or 0x-prefixed hexadecimal)", value != null && !value.isBlank());
		long parsed = Long.decode(value);
		assertTrue(variable + " must fit an unsigned 31-bit payload index", parsed >= 0 && parsed <= Integer.MAX_VALUE);
		return (int) parsed;
	}

	private static void printSegmentIndex(Segment[] segments) {
		System.out.println("Physical Hunk segments:");
		for (Segment segment : segments) {
			HunkLoadSegFile.Node node = segment.getNode();
			String nodeDescription = node == null ? "none" : String.format("header=0x%X overlay=%s level=%d ordinate=%d",
					node.getHeaderFileOffset(), node.isOverlay(), node.getHierarchyLevel(), node.getHierarchyOrdinate());
			byte[] payload = segment.getData();
			String payloadDescription = payload == null ? "BSS" : String.format("0x%X", payload.length);
			System.out.printf("  id=%d type=%s name=%s payload=%s logical-slot=%d node=[%s]%n",
					segment.getId(), segment.getType(), segment.getName(), payloadDescription,
					segment.getLogicalSlot(), nodeDescription);
		}
	}

	private static void printPayloadContext(byte[] payload, int offset) {
		int begin = Math.max(0, offset - CONTEXT_BYTES);
		int end = Math.min(payload.length, offset + CONTEXT_BYTES);
		System.out.printf("Raw payload [0x%X, 0x%X):", begin, end);
		for (int index = begin; index < end; index++) {
			if ((index - begin) % 16 == 0) {
				System.out.printf("%n  0x%08X:", index);
			}
			System.out.printf(" %02X", Byte.toUnsignedInt(payload[index]));
		}
		System.out.println();
	}

	private static void printCoveringRelocations(Segment source, int queryOffset, byte[] payload) {
		boolean found = false;
		for (Segment target : source.getRelocationsToSegments()) {
			for (Reloc relocation : source.getRelocations(target)) {
				int start = relocation.getOffset();
				int end = start + relocation.getWidth();
				if (queryOffset < start || queryOffset >= end) {
					continue;
				}
				found = true;
				System.out.printf("Relocation: source+0x%X width=%d kind=%s target=id=%d (%s), raw-field=%s%n",
						start, relocation.getWidth(), relocation.getKind(), target.getId(), target.getName(),
						formatRawField(payload, relocation));
			}
		}
		if (!found) {
			System.out.println("No relocation record covers this payload offset.");
		}
	}

	private static String formatRawField(byte[] payload, Reloc relocation) {
		int offset = relocation.getOffset();
		return switch (relocation.getWidth()) {
			case 1 -> String.format("0x%02X", Byte.toUnsignedInt(payload[offset]));
			case 2 -> String.format("0x%04X", Short.toUnsignedInt(ByteBuffer.wrap(payload, offset, 2).getShort()));
			case 4 -> String.format("0x%08X", ByteBuffer.wrap(payload, offset, 4).getInt());
			default -> "unsupported-width-" + relocation.getWidth();
		};
	}
}
