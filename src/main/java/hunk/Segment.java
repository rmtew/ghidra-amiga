package hunk;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;

public class Segment {

	private final SegmentType type;
	private final int segSize;
	private final byte[] data;
	private final HashMap<Segment, List<Reloc>> relocsList;
	private final HashMap<Segment, List<Symbol>> symbolsList;
	private int id;
	private int num;
	private final int logicalSlot;
	private final HunkLoadSegFile.Node node;
	private HunkSegment segmentInfo;
	
	Segment(SegmentType type, int size, byte[] data, int num) {
		this(type, size, data, num, num, null);
	}

	Segment(SegmentType type, int size, byte[] data, int num, int logicalSlot, HunkLoadSegFile.Node node) {
		this.type = type;
		this.segSize = size;
		this.data = data;
		this.num = num;
		this.logicalSlot = logicalSlot;
		this.node = node;
		segmentInfo = null;
		
		relocsList = new HashMap<>();
		symbolsList = new HashMap<>();
		
		id = -1;
	}
	
	public SegmentType getType() {
		return type;
	}

	public int getSize() {
		return segSize;
	}

    byte[] getData() {
		return data;
	}
    
    public String getName() {
    	return ((segmentInfo == null) || (segmentInfo.getName() == null)) ? String.format("%s_%02d", type.toString(), id) : segmentInfo.getName();
    }

    void setSegmentInfo(HunkSegment seg) {
    	segmentInfo = seg;
	}
	
	public HunkSegment getSegmentInfo() {
		return segmentInfo;
	}
	
	public int getId() {
		return id;
	}
	
	public void setId(int id) {
		this.id = id;
	}
	
	public int getNum() {
		return num;
	}

	/** Root-header logical Hunk number, which may be shared by sibling nodes. */
	public int getLogicalSlot() {
		return logicalSlot;
	}

	/** Physical node owning this segment, or null for a non-LoadSeg image. */
	public HunkLoadSegFile.Node getNode() {
		return node;
	}
	
	public void setNum(int num) {
		this.num = num;
	}
	
	void addRelocations(Segment segment, final List<Reloc> relocs) {
		relocsList.put(segment, relocs);
	}
	
	void addSymbols(Segment segment, final List<Symbol> symbols) {
		symbolsList.put(segment, symbols);
	}
	
	public Segment[] getRelocationsToSegments() {
		return new HashSet<>(relocsList.keySet()).toArray(Segment[]::new);
	}
	
	public Reloc[] getRelocations(Segment toSeg) {
		final List<Reloc> relocs = relocsList.getOrDefault(toSeg, new ArrayList<>());
		
		return relocs.toArray(Reloc[]::new);
	}

	public Symbol[] getSymbols(Segment toSeg) {
		final List<Symbol> symbols = symbolsList.getOrDefault(toSeg, new ArrayList<>());
		
		return symbols.toArray(Symbol[]::new);
	}

}
