package hunk;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import ghidra.app.util.importer.MessageLog;

public final class BinFmtHunk {

	public static BinImage loadImage(HunkBlockFile hbf, MessageLog log) {
		HunkLoadSegFile lsf = loadSegFile(hbf, log);
		return lsf == null ? null : createImage(lsf, log);
	}

	public static HunkLoadSegFile loadSegFile(HunkBlockFile hbf, MessageLog log) {
		try {
			HunkLoadSegFile lsf = new HunkLoadSegFile();
			lsf.parseBlockFile(hbf);
			return lsf;
		} catch (HunkParseError e) {
			log.appendException(e);
			return null;
		}
	}
	
	public static BinImage createImage(HunkLoadSegFile lsf, MessageLog log) {
		try {
			return createImageFromLoadSegFile(lsf);
		} catch (HunkParseError e) {
			log.appendException(e);
			return null;
		}
	}

	private static BinImage createImageFromLoadSegFile(HunkLoadSegFile lsf) throws HunkParseError {
		int num = 0;
		
		BinImage bi = new BinImage();
		
		HunkSegment[] segs = lsf.getSegments();
		
		for (HunkSegment seg : segs) {
			int size = seg.getSizeLongs() * 4;
			byte[] data;

			HunkSegmentBlock segBlock = seg.getSegmentBlock();
			if (segBlock == null) {
				continue;
			}

			data = segBlock.getData();
			
			SegmentType segType;
			if (seg.getHunkType() == HunkType.HUNK_CODE) {
				segType = SegmentType.SEGMENT_TYPE_CODE;
			} else if (seg.getHunkType() == HunkType.HUNK_DATA) {
				segType = SegmentType.SEGMENT_TYPE_DATA;
			} else if (seg.getHunkType() == HunkType.HUNK_BSS ) {
				segType = SegmentType.SEGMENT_TYPE_BSS;
			} else if (seg.getHunkType() == HunkType.HUNK_SYMBOL) {
				segType = SegmentType.SEGMENT_TYPE_DATA;
			} else {
				throw new HunkParseError(String.format("Unknown Segment Type for BinImage: %d", seg.getHunkType().getValue()));
			}
			
			HunkLoadSegFile.Node node = lsf.getOwner(seg);
			int logicalSlot = node == null ? num : findLogicalSlot(node, seg);
			Segment bs = new Segment(segType, size, data, num++, logicalSlot, node);
			bs.setSegmentInfo(seg);
			bi.addSegment(bs, seg);
		}
		
		Segment[] biSegs = bi.getSegments();
		
		for (Segment seg : biSegs) {
			HunkSegment hSeg = seg.getSegmentInfo();
			HunkRelocBlock[] relocBlocks = hSeg.getRelocBlocks();
			HunkSymbolBlock[] symbolBlocks = hSeg.getSymbolBlocks();
			
			if (relocBlocks != null) {
				addHunkRelocs(relocBlocks, seg, lsf, bi);
			}
			if(symbolBlocks != null) {
				addHunkSymbols(symbolBlocks, seg, biSegs);
			}
		}
		
		return bi;
	}

	private static int findLogicalSlot(HunkLoadSegFile.Node node, HunkSegment segment) throws HunkParseError {
		for (int slot = node.getFirstHunk(); slot <= node.getLastHunk(); slot++) {
			if (node.getSegmentAtLogicalSlot(slot) == segment) {
				return slot;
			}
		}
		throw new HunkParseError("Physical segment is not owned by its HUNK_HEADER node");
	}
	
	private static void addHunkRelocs(HunkRelocBlock[] relocBlocks, Segment seg, HunkLoadSegFile loadSegFile,
			BinImage image) throws HunkParseError {
		for (HunkRelocBlock blk : relocBlocks) {
			for (RelocData r : blk.getRelocs()) {
				int hunkNum = r.getHunkNum();
				Reloc[] offsets = r.getRelocs();
				
				HunkSegment targetHunk;
				if (seg.getNode() == null) {
					HunkSegment[] allHunks = loadSegFile.getSegments();
					if (hunkNum < 0 || hunkNum >= allHunks.length) {
						throw new HunkParseError("Invalid hunk segment number");
					}
					targetHunk = allHunks[hunkNum];
				}
				else {
					targetHunk = loadSegFile.resolveLogicalSlot(seg.getNode(), hunkNum);
					if (targetHunk == null) {
						throw new HunkParseError(String.format(
								"Relocation from logical hunk %d targets unavailable slot %d", seg.getLogicalSlot(), hunkNum));
					}
				}
				Segment toSeg = image.getPhysicalSegment(targetHunk);
				if (toSeg == null) {
					throw new HunkParseError("Relocation target has no physical image segment");
				}
				
				List<Reloc> rl = new ArrayList<>();
				rl.addAll(Arrays.asList(seg.getRelocations(toSeg)));
				rl.addAll(Arrays.asList(offsets));
				
				seg.addRelocations(toSeg, rl);
			}
		}
	}
	
	private static void addHunkSymbols(HunkSymbolBlock[] symbolBlocks, Segment seg, Segment[] allSegs) throws HunkParseError {
		for(HunkSymbolBlock blk: symbolBlocks) {
			for(SymbolData s : blk.getSymbols()) {
				Symbol[] offsets = s.getSymbols();
				int id = seg.getNum();
				int hunkNum = 0;
				
				//traverse array backwards for last code or data hunk
				for(int as = id; as > 0; as--) {
					if((allSegs[as].getType() == SegmentType.SEGMENT_TYPE_CODE) || (allSegs[as].getType() == SegmentType.SEGMENT_TYPE_DATA)) {
						hunkNum = as;
						break;
					}
				}

				if (hunkNum >= allSegs.length) {
					throw new HunkParseError("Invalid hunk segment number");
				}
				
				Segment toSeg = allSegs[hunkNum++];
				
				List<Symbol> sl = new ArrayList<>();
				sl.addAll(Arrays.asList(seg.getSymbols(toSeg)));
				sl.addAll(Arrays.asList(offsets));
				
				seg.addSymbols(toSeg, sl);
			}
		}
	}
}
