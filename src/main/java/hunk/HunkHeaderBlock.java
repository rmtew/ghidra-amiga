package hunk;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import ghidra.app.util.bin.BinaryReader;

class HunkHeaderBlock extends HunkBlock {

	private List<Integer> hunkTable;
	private int tableSize;
	private int firstHunk;
	private int lastHunk;

	HunkHeaderBlock(BinaryReader reader, boolean isExecutable) throws HunkParseError {
		super(HunkType.HUNK_HEADER, reader);
		
		parse(reader, isExecutable);
		calcHunkSize(reader);
	}
	
	@Override
	void parse(BinaryReader reader, boolean isExecutable) throws HunkParseError {
		hunkTable = new ArrayList<>();
		
		while (true) {
			try {
				String name = HunkBlock.readName(reader);
				
				if (name == null) {
					throw new IOException();
				} else if (name.length() == 0) {
					break;
				}

			} catch (IOException e) {
				throw new HunkParseError("Error parsing HUNK_HEADER names");
			}
		}
		
		try {
			tableSize = reader.readNextInt();
			firstHunk = reader.readNextInt();
			lastHunk = reader.readNextInt();
			
			// Overlay-node headers use global hunk numbers while their table size may
			// describe only that node.  Therefore the range is not bounded by tableSize.
			if (tableSize < 0 || firstHunk < 0 || lastHunk < firstHunk) {
				throw new HunkParseError("HUNK_HEADER has an invalid table size or hunk range");
			}
			
			for (int a = 0; a < lastHunk - firstHunk + 1; ++a) {
				int hunkSize = reader.readNextInt();
				
				if (hunkSize < 0) {
					throw new HunkParseError("HUNK_HEADER contains invalid hunk_size");
				}
				
				hunkTable.add(hunkSize & 0x3FFFFFFF);
			}
		} catch (IOException e) {
			throw new HunkParseError(e);
		}
	}

	Integer[] getHunkTable() {
		return hunkTable.toArray(Integer[]::new);
	}

	int getTableSize() {
		return tableSize;
	}

	int getFirstHunk() {
		return firstHunk;
	}

	int getLastHunk() {
		return lastHunk;
	}
}
