package hunk;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.List;

/** The documented flat MANX/Aztec C HUNK_OVERLAY table. */
public final class HunkManxOverlayTable {

	public static final class SegmentDescriptor {
		private final int segment;
		private final int trampolineCount;

		private SegmentDescriptor(int segment, int trampolineCount) {
			this.segment = segment;
			this.trampolineCount = trampolineCount;
		}

		public int getSegment() { return segment; }
		public int getTrampolineCount() { return trampolineCount; }
	}

	public static final class Node {
		private final int filePosition;
		private final int trampolineOffset;
		private final int symbolTableOffset;
		private final List<SegmentDescriptor> segments;

		private Node(int filePosition, int trampolineOffset, int symbolTableOffset,
				List<SegmentDescriptor> segments) {
			this.filePosition = filePosition;
			this.trampolineOffset = trampolineOffset;
			this.symbolTableOffset = symbolTableOffset;
			this.segments = List.copyOf(segments);
		}

		public int getFilePosition() { return filePosition; }
		public int getTrampolineOffset() { return trampolineOffset; }
		public int getSymbolTableOffset() { return symbolTableOffset; }
		public List<SegmentDescriptor> getSegments() { return segments; }
	}

	private final List<Node> nodes;

	private HunkManxOverlayTable(List<Node> nodes) {
		this.nodes = List.copyOf(nodes);
	}

	public List<Node> getNodes() { return nodes; }

	static HunkManxOverlayTable parse(byte[] payload) throws HunkParseError {
		if (payload.length < 12 || payload.length % 4 != 0) {
			throw new HunkParseError("HUNK_OVERLAY is too small for a MANX table");
		}
		ByteBuffer data = ByteBuffer.wrap(payload).order(ByteOrder.BIG_ENDIAN);
		int nodeCount = data.getInt();
		if (nodeCount < 1 || nodeCount > (payload.length - 4) / 8) {
			throw new HunkParseError("HUNK_OVERLAY has an invalid MANX node count");
		}

		int entriesEnd = 4 + nodeCount * 8;
		int[] filePositions = new int[nodeCount];
		int[] trampolineOffsets = new int[nodeCount];
		int[] symbolOffsets = new int[nodeCount];
		int[] symbolAddresses = new int[nodeCount];
		for (int node = 0; node < nodeCount; node++) {
			filePositions[node] = data.getInt();
			trampolineOffsets[node] = Short.toUnsignedInt(data.getShort());
			symbolOffsets[node] = Short.toUnsignedInt(data.getShort());
			symbolAddresses[node] = 4 + symbolOffsets[node]; // relative to the first node record
			if (filePositions[node] < 0 || symbolAddresses[node] < entriesEnd ||
					symbolAddresses[node] >= payload.length || (symbolAddresses[node] & 3) != 0) {
				throw new HunkParseError("HUNK_OVERLAY has an invalid MANX node entry");
			}
		}

		List<Node> nodes = new ArrayList<>(nodeCount);
		for (int node = 0; node < nodeCount; node++) {
			ByteBuffer symbols = ByteBuffer.wrap(payload).order(ByteOrder.BIG_ENDIAN);
			int symbolListEnd = payload.length;
			for (int other = 0; other < nodeCount; other++) {
				if (symbolAddresses[other] > symbolAddresses[node]) {
					symbolListEnd = Math.min(symbolListEnd, symbolAddresses[other]);
				}
			}
			symbols.position(symbolAddresses[node]);
			List<SegmentDescriptor> descriptors = new ArrayList<>();
			boolean terminated = false;
			while (symbols.position() + 4 <= symbolListEnd) {
				int segment = Short.toUnsignedInt(symbols.getShort());
				int count = Short.toUnsignedInt(symbols.getShort());
				if (segment == 0) {
					if (count != 0) {
						throw new HunkParseError("HUNK_OVERLAY MANX symbol list has an invalid terminator");
					}
					terminated = true;
					break;
				}
				if (count == 0) {
					throw new HunkParseError("HUNK_OVERLAY MANX symbol entry has zero trampolines");
				}
				descriptors.add(new SegmentDescriptor(segment, count));
			}
			if (!terminated) {
				throw new HunkParseError("HUNK_OVERLAY MANX symbol list is unterminated");
			}
			nodes.add(new Node(filePositions[node], trampolineOffsets[node], symbolOffsets[node], descriptors));
		}
		return new HunkManxOverlayTable(nodes);
	}
}
