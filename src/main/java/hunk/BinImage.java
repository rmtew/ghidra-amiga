package hunk;

import java.util.ArrayList;
import java.util.IdentityHashMap;
import java.util.List;
import java.util.Map;

public class BinImage {

	private final List<Segment> segments;
	private final Map<HunkSegment, Segment> physicalSegments;

	BinImage() {
		segments = new ArrayList<>();
		physicalSegments = new IdentityHashMap<>();
	}

	void addSegment(Segment seg) {
		seg.setId(segments.size());
		segments.add(seg);
	}

	void addSegment(Segment seg, HunkSegment hunkSegment) {
		addSegment(seg);
		physicalSegments.put(hunkSegment, seg);
	}

	Segment getPhysicalSegment(HunkSegment hunkSegment) {
		return physicalSegments.get(hunkSegment);
	}

	public Segment[] getSegments() {
		return segments.toArray(Segment[]::new);
	}
}
