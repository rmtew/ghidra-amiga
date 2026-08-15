package hunk;

public class Reloc {
	public enum Kind {
		ABSOLUTE,
		PC_RELATIVE,
		BASE_RELATIVE
	}

	private final int offset;
	private final int width;
	private final int addend;
	private final Kind kind;
	
	Reloc(int offset, int width, Kind kind) {
		this.offset = offset;
		this.width = width;
		this.addend = 0;
		this.kind = kind;
	}

	public final int getOffset() {
		return offset;
	}

	public final int getWidth() {
		return width;
	}

	public final int getAddend() {
		return addend;
	}

	public final Kind getKind() {
		return kind;
	}
	
}
