package hunk;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import ghidra.app.util.bin.BinaryReader;

class HunkRelocLongBlock extends HunkRelocBlock {
	
	public HunkRelocLongBlock(HunkType type, BinaryReader reader, boolean isExecutable, int size) throws HunkParseError {
		super(type, reader, size, getKind(type, isExecutable));

		parse(reader, isExecutable);
		calcHunkSize(reader);
	}

	@Override
	void parse(BinaryReader reader, boolean isExecutable) throws HunkParseError {
		while (true) {
			try {
				int num = reader.readNextInt();
				
				if (num == 0) {
					break;
				}
				
				int hunkNum = reader.readNextInt();
				
				List<Reloc> toAdd = new ArrayList<>();
				
				for (int i = 0; i < num; ++i) {
					toAdd.add(new Reloc(reader.readNextInt(), size, kind));
				}
				
				relocs.add(new RelocData(hunkNum, toAdd));
			} catch (IOException e) {
				throw new HunkParseError(e);
			}
		}
	}

	private static Reloc.Kind getKind(HunkType type, boolean isExecutable) throws HunkParseError {
		switch (type) {
		case HUNK_ABSRELOC32:
			return Reloc.Kind.ABSOLUTE;
		case HUNK_DREL32:
			return isExecutable ? Reloc.Kind.ABSOLUTE : Reloc.Kind.BASE_RELATIVE;
		case HUNK_RELRELOC32:
		case HUNK_RELRELOC16:
		case HUNK_RELRELOC8:
			return Reloc.Kind.PC_RELATIVE;
		case HUNK_DREL16:
		case HUNK_DREL8:
			return Reloc.Kind.BASE_RELATIVE;
		default:
			throw new HunkParseError("Unsupported relocation kind: " + type);
		}
	}

}
