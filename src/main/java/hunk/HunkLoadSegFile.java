package hunk;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import generic.stl.Pair;

public class HunkLoadSegFile {

	private final List<HunkSegment> segments;
	private final List<Node> nodes;
	private HunkOverlayTable overlayTable;
	private HunkManxOverlayTable manxOverlayTable;
	private byte[] overlayTableData;

	public static final class Node {
		private final int firstHunk;
		private final int lastHunk;
		private final boolean overlay;
		private final int headerFileOffset;

		Node(HunkHeaderBlock header, int headerFileOffset, boolean overlay) {
			this.firstHunk = header.getFirstHunk();
			this.lastHunk = header.getLastHunk();
			this.overlay = overlay;
			this.headerFileOffset = headerFileOffset;
		}

		public int getFirstHunk() {
			return firstHunk;
		}

		public int getLastHunk() {
			return lastHunk;
		}

		public boolean isOverlay() {
			return overlay;
		}

		/** File offset of this node's HUNK_HEADER, as used by HUNK_OVERLAY. */
		public int getHeaderFileOffset() {
			return headerFileOffset;
		}
	}
	
	public HunkLoadSegFile() {
		segments = new ArrayList<>();
		nodes = new ArrayList<>();
	}
	
	public HunkSegment[] getSegments() {
		return segments.toArray(HunkSegment[]::new);
	}

	public Node[] getNodes() {
		return nodes.toArray(Node[]::new);
	}

	/** The recognised ALink/BLink/SLink table, or null for a different manager. */
	public HunkOverlayTable getOverlayTable() {
		return overlayTable;
	}

	/** The recognised flat MANX table, or null for another manager. */
	public HunkManxOverlayTable getManxOverlayTable() {
		return manxOverlayTable;
	}

	/** The exact HUNK_OVERLAY payload, irrespective of overlay-manager format. */
	public byte[] getOverlayTableData() {
		return overlayTableData == null ? null : overlayTableData.clone();
	}
	
	public void parseBlockFile(HunkBlockFile bf) throws HunkParseError {
		
		if (bf == null) {
			return;
		}
		
		final List<Pair<Integer, HunkBlock>> blocks = bf.getHunkBlocks();
		
		if (blocks == null || blocks.isEmpty()) {
			throw new HunkParseError("No hunk blocks found!");
		}
		
		boolean isUnit = (blocks.get(0).second.getHunkType() == HunkType.HUNK_UNIT) || (blocks.get(0).second.getHunkType() == HunkType.HUNK_CODE);

		if (isUnit) {
			segments.addAll(parsePairNodeBlocks(blocks, null));
			return;
		}

		HunkHeaderBlock rootHeader = (HunkHeaderBlock) blocks.get(0).second;
		if (rootHeader.getHunkType() != HunkType.HUNK_HEADER) {
			throw new HunkParseError("No HEADER block found!");
		}

		segments.addAll(Collections.nCopies(rootHeader.getTableSize(), null));
		HunkHeaderBlock nodeHeader = null;
		int nodeHeaderOffset = -1;
		List<HunkBlock> nodeBlocks = new ArrayList<>();

		for (Pair<Integer, HunkBlock> pair : blocks) {
			HunkBlock block = pair.second;

			if (block instanceof HunkHeaderBlock) {
				if (nodeHeader != null) {
					throw new HunkParseError("Overlay node is missing its terminator");
				}
				nodeHeader = (HunkHeaderBlock) block;
				nodeHeaderOffset = pair.first;
				continue;
			}

			if (nodeHeader == null) {
				throw new HunkParseError("Hunk data found outside a HUNK_HEADER node");
			}

			if (block.getHunkType() == HunkType.HUNK_OVERLAY || block.getHunkType() == HunkType.HUNK_BREAK) {
				boolean isRootTerminator = block.getHunkType() == HunkType.HUNK_OVERLAY;
				if (isRootTerminator) {
					if (!nodes.isEmpty() || !(block instanceof HunkOverlayBlock)) {
						throw new HunkParseError("HUNK_OVERLAY may only terminate the root node");
					}
					HunkOverlayBlock overlayBlock = (HunkOverlayBlock) block;
					overlayTable = overlayBlock.getHierarchicalTable();
					manxOverlayTable = overlayBlock.getManxTable();
					overlayTableData = overlayBlock.getTableData();
				}
				addOverlayNode(nodeHeader, nodeHeaderOffset, nodeBlocks, !isRootTerminator);
				nodeHeader = null;
				nodeHeaderOffset = -1;
				nodeBlocks = new ArrayList<>();
			} else {
				nodeBlocks.add(block);
			}
		}

		if (nodeHeader != null) {
			addOverlayNode(nodeHeader, nodeHeaderOffset, nodeBlocks, false);
		}

		if (segments.contains(null)) {
			throw new HunkParseError("Overlay file does not define every segment in the root HUNK_HEADER table");
		}
	}

	private void addOverlayNode(HunkHeaderBlock header, int headerFileOffset, List<HunkBlock> blocks, boolean overlay) throws HunkParseError {
		List<HunkSegment> nodeSegments = parseNodeBlocks(blocks, header);
		for (int i = 0; i < nodeSegments.size(); ++i) {
			int hunkNumber = header.getFirstHunk() + i;
			if (hunkNumber >= segments.size() || segments.get(hunkNumber) != null) {
				throw new HunkParseError("Overlay node contains an invalid or duplicate hunk number");
			}
			segments.set(hunkNumber, nodeSegments.get(i));
		}
		nodes.add(new Node(header, headerFileOffset, overlay));
	}

	private static List<HunkSegment> parsePairNodeBlocks(List<? extends Pair<Integer, HunkBlock>> pairs, HunkHeaderBlock header) throws HunkParseError {
		List<HunkBlock> blocks = new ArrayList<>();
		for (Pair<Integer, HunkBlock> pair : pairs) {
			HunkBlock block = pair.second;
			if (!(block instanceof HunkHeaderBlock) && !(block instanceof HunkUnitBlock)) {
				blocks.add(block);
			}
		}
		return parseNodeBlocks(blocks, header);
	}

	private static List<HunkSegment> parseNodeBlocks(List<HunkBlock> blocks, HunkHeaderBlock header) throws HunkParseError {
		List<List<HunkBlock>> groupedSegments = new ArrayList<>();
		List<HunkBlock> current = null;

		for (HunkBlock block : blocks) {
			if (block.getHunkType() == HunkType.HUNK_END) {
				current = null;
				continue;
			}
			if (!(block.isValidLoadsegBeginHunk() || block.isValidLoadsegExtraHunk())) {
				throw new HunkParseError(String.format("Invalid block found in hunk node: %d", block.getHunkType().getValue()));
			}
			if (block.isValidLoadsegBeginHunk()) {
				current = new ArrayList<>();
				groupedSegments.add(current);
			} else if (current == null) {
				throw new HunkParseError("Hunk node contains data before its first segment");
			}
			current.add(block);
		}

		if (header != null && header.getHunkTable().length != groupedSegments.size()) {
			throw new HunkParseError("Can't match hunks to HUNK_HEADER range");
		}

		List<HunkSegment> parsed = new ArrayList<>();
		for (int i = 0; i < groupedSegments.size(); ++i) {
			HunkSegment segment = new HunkSegment();
			segment.parse(groupedSegments.get(i));
			if (header != null) {
				segment.setSizeLongs(header.getHunkTable()[i]);
			}
			parsed.add(segment);
		}
		return parsed;
	}
}
