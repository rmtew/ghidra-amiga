package hunk;

import java.io.IOException;

import ghidra.app.util.bin.BinaryReader;

class HunkOverlayBlock extends HunkBlock {
	private int tableLength;
	private byte[] tableData;
	private HunkOverlayTable hierarchicalTable;
	private HunkManxOverlayTable manxTable;
	
	HunkOverlayBlock(BinaryReader reader, boolean isExecutable) throws HunkParseError {
		super(HunkType.HUNK_OVERLAY, reader);

		parse(reader, isExecutable);
		calcHunkSize(reader);
	}

	@Override
	void parse(BinaryReader reader, boolean isExecutable) throws HunkParseError {
		try {
			tableLength = reader.readNextInt();
			if (tableLength < 0 || tableLength >= Integer.MAX_VALUE / 4) {
				throw new HunkParseError("HUNK_OVERLAY contains an invalid table length");
			}
			// The on-disk length is l; the overlay table is l + 1 longwords large.
			tableData = reader.readNextByteArray((tableLength + 1) * 4);
			try {
				hierarchicalTable = HunkOverlayTable.parse(tableData);
			} catch (HunkParseError ignored) {
				// HUNK permits manager-specific table formats. The raw payload is retained.
				hierarchicalTable = null;
			}
			try {
				manxTable = HunkManxOverlayTable.parse(tableData);
			} catch (HunkParseError ignored) {
				manxTable = null;
			}
		} catch (IOException e) {
			throw new HunkParseError(e);
		}
	}

	int getTableLength() { return tableLength; }
	byte[] getTableData() { return tableData.clone(); }
	HunkOverlayTable getHierarchicalTable() { return hierarchicalTable; }
	HunkManxOverlayTable getManxTable() { return manxTable; }

}
