package hunk;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.List;

/** The published hierarchical ALink/BLink/SLink HUNK_OVERLAY table. */
public final class HunkOverlayTable {

	public static final class Symbol {
		private final int filePosition;
		private final int level;
		private final int ordinate;
		private final int firstSegment;
		private final int symbolSegment;
		private final int symbolOffset;

		private Symbol(int filePosition, int level, int ordinate, int firstSegment,
				int symbolSegment, int symbolOffset) {
			this.filePosition = filePosition;
			this.level = level;
			this.ordinate = ordinate;
			this.firstSegment = firstSegment;
			this.symbolSegment = symbolSegment;
			this.symbolOffset = symbolOffset;
		}

		public int getFilePosition() { return filePosition; }
		public int getLevel() { return level; }
		public int getOrdinate() { return ordinate; }
		public int getFirstSegment() { return firstSegment; }
		public int getSymbolSegment() { return symbolSegment; }
		public int getSymbolOffset() { return symbolOffset; }
	}

	private final int treeDepth;
	private final List<Integer> ordinates;
	private final List<Symbol> symbols;

	private HunkOverlayTable(int treeDepth, List<Integer> ordinates, List<Symbol> symbols) {
		this.treeDepth = treeDepth;
		this.ordinates = List.copyOf(ordinates);
		this.symbols = List.copyOf(symbols);
	}

	public int getTreeDepth() { return treeDepth; }
	public List<Integer> getOrdinates() { return ordinates; }
	public List<Symbol> getSymbols() { return symbols; }

	static HunkOverlayTable parse(byte[] payload) throws HunkParseError {
		if (payload.length < Integer.BYTES || payload.length % Integer.BYTES != 0) {
			throw new HunkParseError("HUNK_OVERLAY table has an invalid length");
		}
		ByteBuffer words = ByteBuffer.wrap(payload).order(ByteOrder.BIG_ENDIAN);
		int totalWords = payload.length / Integer.BYTES;
		int treeDepth = words.getInt();
		if (treeDepth < 1 || treeDepth > totalWords) {
			throw new HunkParseError("HUNK_OVERLAY is not a hierarchical overlay table");
		}

		List<Integer> ordinates = new ArrayList<>(treeDepth - 1);
		for (int level = 1; level < treeDepth; level++) {
			ordinates.add(words.getInt());
		}

		int remainingWords = totalWords - treeDepth;
		if (remainingWords % 8 != 0) {
			throw new HunkParseError("HUNK_OVERLAY hierarchical symbol table is not aligned");
		}

		List<Symbol> symbols = new ArrayList<>(remainingWords / 8);
		while (words.hasRemaining()) {
			int filePosition = words.getInt();
			words.getInt();
			words.getInt();
			int level = words.getInt();
			int ordinate = words.getInt();
			int firstSegment = words.getInt();
			int symbolSegment = words.getInt();
			int symbolOffset = words.getInt();
			if (level < 1 || level >= treeDepth || ordinate < 1 || firstSegment < 1 ||
					symbolSegment < firstSegment || symbolOffset < 0) {
				throw new HunkParseError("HUNK_OVERLAY contains an invalid hierarchical symbol entry");
			}
			symbols.add(new Symbol(filePosition, level, ordinate, firstSegment, symbolSegment, symbolOffset));
		}
		return new HunkOverlayTable(treeDepth, ordinates, symbols);
	}
}
