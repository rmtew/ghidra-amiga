package hunk;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import generic.stl.Pair;

public class HunkLoadSegFile {

	private final List<HunkSegment> segments;
	private final List<Node> nodes;
	private HunkOverlayTable overlayTable;
	private HunkManxOverlayTable manxOverlayTable;
	private byte[] overlayTableData;
	private int logicalSlotCount;

	/** A physical Hunk node and the logical segment slots it supplies. */
	public static final class Node {
		private final int firstHunk;
		private final int lastHunk;
		private final boolean overlay;
		private final int headerFileOffset;
		private final List<HunkSegment> segments = new ArrayList<>();
		private final Map<Integer, HunkSegment> segmentsByLogicalSlot = new HashMap<>();
		private Node parent;
		private int hierarchyLevel;
		private int hierarchyOrdinate;

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

		/** Physical segments carried by this node, in HUNK_HEADER order. */
		public HunkSegment[] getSegments() {
			return segments.toArray(HunkSegment[]::new);
		}

		/** Returns this node's physical segment occupying {@code logicalSlot}. */
		public HunkSegment getSegmentAtLogicalSlot(int logicalSlot) {
			return segmentsByLogicalSlot.get(logicalSlot);
		}

		/** Parent from the published hierarchical table, or null for root/unknown topology. */
		public Node getParent() {
			return parent;
		}

		public int getHierarchyLevel() {
			return hierarchyLevel;
		}

		public int getHierarchyOrdinate() {
			return hierarchyOrdinate;
		}

		private void addSegment(int logicalSlot, HunkSegment segment) {
			segments.add(segment);
			segmentsByLogicalSlot.put(logicalSlot, segment);
		}

		private void setHierarchy(Node parent, int level, int ordinate) {
			this.parent = parent;
			this.hierarchyLevel = level;
			this.hierarchyOrdinate = ordinate;
		}
	}

	public HunkLoadSegFile() {
		segments = new ArrayList<>();
		nodes = new ArrayList<>();
	}

	/** All physical segments in file order. Logical slots may intentionally repeat. */
	public HunkSegment[] getSegments() {
		return segments.toArray(HunkSegment[]::new);
	}

	/** Number of logical segment-table slots declared by the root header. */
	public int getLogicalSlotCount() {
		return logicalSlotCount;
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

		boolean isUnit = blocks.get(0).second.getHunkType() == HunkType.HUNK_UNIT ||
				blocks.get(0).second.getHunkType() == HunkType.HUNK_CODE;
		if (isUnit) {
			segments.addAll(parsePairNodeBlocks(blocks, null));
			return;
		}

		HunkHeaderBlock rootHeader = (HunkHeaderBlock) blocks.get(0).second;
		if (rootHeader.getHunkType() != HunkType.HUNK_HEADER) {
			throw new HunkParseError("No HEADER block found!");
		}
		logicalSlotCount = rootHeader.getTableSize();

		HunkHeaderBlock nodeHeader = null;
		int nodeHeaderOffset = -1;
		List<HunkBlock> nodeBlocks = new ArrayList<>();
		boolean sawRootOverlay = false;
		for (Pair<Integer, HunkBlock> pair : blocks) {
			HunkBlock block = pair.second;
			if (block instanceof HunkHeaderBlock) {
				if (nodeHeader != null) {
					if (!sawRootOverlay || !nodes.isEmpty()) {
						throw new HunkParseError("Overlay node is missing its terminator");
					}
					// Early BLINK writes HUNK_OVERLAY within the root node, before its
					// remaining root hunks. The following HUNK_HEADER is the first
					// actual overlay node and therefore closes the root implicitly.
					addOverlayNode(nodeHeader, nodeHeaderOffset, nodeBlocks, false);
				}
				nodeHeader = (HunkHeaderBlock) block;
				nodeHeaderOffset = pair.first;
				nodeBlocks = new ArrayList<>();
				continue;
			}
			if (nodeHeader == null) {
				throw new HunkParseError("Hunk data found outside a HUNK_HEADER node");
			}
			if (block.getHunkType() == HunkType.HUNK_OVERLAY) {
				if (!nodes.isEmpty() || !(block instanceof HunkOverlayBlock)) {
					throw new HunkParseError("HUNK_OVERLAY may only occur in the root node");
				}
				HunkOverlayBlock overlayBlock = (HunkOverlayBlock) block;
				overlayTable = overlayBlock.getHierarchicalTable();
				manxOverlayTable = overlayBlock.getManxTable();
				overlayTableData = overlayBlock.getTableData();
				sawRootOverlay = true;
			}
			else if (block.getHunkType() == HunkType.HUNK_BREAK) {
				addOverlayNode(nodeHeader, nodeHeaderOffset, nodeBlocks, true);
				nodeHeader = null;
				nodeHeaderOffset = -1;
				nodeBlocks = new ArrayList<>();
			}
			else {
				nodeBlocks.add(block);
			}
		}

		if (nodeHeader != null) {
			addOverlayNode(nodeHeader, nodeHeaderOffset, nodeBlocks, false);
		}
		buildHierarchicalParents();
	}

	private void addOverlayNode(HunkHeaderBlock header, int headerFileOffset, List<HunkBlock> blocks, boolean overlay)
			throws HunkParseError {
		List<HunkSegment> nodeSegments = parseNodeBlocks(blocks, header);
		Node node = new Node(header, headerFileOffset, overlay);
		for (int i = 0; i < nodeSegments.size(); ++i) {
			int logicalSlot = header.getFirstHunk() + i;
			if (logicalSlot >= logicalSlotCount) {
				throw new HunkParseError("Overlay node contains a hunk number outside the root HUNK_HEADER table");
			}
			HunkSegment segment = nodeSegments.get(i);
			node.addSegment(logicalSlot, segment);
			segments.add(segment);
		}
		nodes.add(node);
	}

	/**
	 * Associates nodes with the parent path explicitly identified by the standard
	 * hierarchical table. A node without table-symbol evidence retains an unknown
	 * parent rather than receiving a guessed path.
	 */
	private void buildHierarchicalParents() throws HunkParseError {
		if (nodes.isEmpty()) {
			return;
		}
		nodes.get(0).setHierarchy(null, 0, 0);
		if (overlayTable == null) {
			return;
		}

		Map<Integer, Node> nodesByHeaderOffset = new HashMap<>();
		for (Node node : nodes) {
			if (node.isOverlay()) {
				nodesByHeaderOffset.put(node.getHeaderFileOffset(), node);
			}
		}
		for (HunkOverlayTable.Symbol symbol : overlayTable.getSymbols()) {
			Node node = nodesByHeaderOffset.get(symbol.getFilePosition());
			if (node == null) {
				continue;
			}
			if (node.hierarchyLevel != 0 && (node.hierarchyLevel != symbol.getLevel() ||
					node.hierarchyOrdinate != symbol.getOrdinate())) {
				throw new HunkParseError("HUNK_OVERLAY assigns inconsistent hierarchy coordinates to one node");
			}
			node.setHierarchy(null, symbol.getLevel(), symbol.getOrdinate());
		}

		Map<Integer, Node> latestNodeAtLevel = new HashMap<>();
		latestNodeAtLevel.put(0, nodes.get(0));
		for (int nodeIndex = 1; nodeIndex < nodes.size(); nodeIndex++) {
			Node node = nodes.get(nodeIndex);
			if (node.hierarchyLevel == 0) {
				continue;
			}
			Node parent = latestNodeAtLevel.get(node.hierarchyLevel - 1);
			node.setHierarchy(parent, node.hierarchyLevel, node.hierarchyOrdinate);
			latestNodeAtLevel.put(node.hierarchyLevel, node);
			latestNodeAtLevel.keySet().removeIf(level -> level > node.hierarchyLevel);
		}
	}

	/** Resolves a logical slot through the source node's loaded root-to-node path. */
	public HunkSegment resolveLogicalSlot(Node sourceNode, int logicalSlot) {
		for (Node node = sourceNode; node != null; node = node.parent) {
			HunkSegment segment = node.getSegmentAtLogicalSlot(logicalSlot);
			if (segment != null) {
				return segment;
			}
		}
		return null;
	}

	/** Returns the node owning a physical segment, or null for a non-LoadSeg segment. */
	public Node getOwner(HunkSegment segment) {
		for (Node node : nodes) {
			for (HunkSegment candidate : node.segments) {
				if (candidate == segment) {
					return node;
				}
			}
		}
		return null;
	}

	private static List<HunkSegment> parsePairNodeBlocks(List<? extends Pair<Integer, HunkBlock>> pairs,
			HunkHeaderBlock header) throws HunkParseError {
		List<HunkBlock> blocks = new ArrayList<>();
		for (Pair<Integer, HunkBlock> pair : pairs) {
			HunkBlock block = pair.second;
			if (!(block instanceof HunkHeaderBlock) && !(block instanceof HunkUnitBlock)) {
				blocks.add(block);
			}
		}
		return parseNodeBlocks(blocks, header);
	}

	private static List<HunkSegment> parseNodeBlocks(List<HunkBlock> blocks, HunkHeaderBlock header)
			throws HunkParseError {
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
			}
			else if (current == null) {
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
