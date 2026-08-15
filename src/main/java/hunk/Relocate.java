package hunk;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

public class Relocate {

	private final BinImage binImage;
	
	public Relocate(BinImage binImage) {
		if (binImage == null) {
			this.binImage = new BinImage();
		} else {
			this.binImage = binImage;
		}
	}
	
	private int[] getSizes() {
		Segment[] segs = binImage.getSegments();
		int[] sizes = new int[segs.length];
		
		for (int i = 0; i < segs.length; ++i) {
			sizes[i] = segs[i].getSize();
		}
		
		return sizes;
	}
	
	public int[] getSeqAddresses(long baseAddr) {
		int[] sizes = getSizes();
		int[] addrs = new int[sizes.length];
		
		int addr = (int)baseAddr;
		for (int i = 0; i < sizes.length; ++i) {
			addrs[i] = addr;
			addr += sizes[i];
		}
		
		return addrs;
	}
	
	public List<byte[]> relocate(int[] addrs) throws HunkParseError {
		Segment[] segs = binImage.getSegments();
		
		if (segs.length != addrs.length) {
			throw new HunkParseError("Reloc addrs != Reloc segments");
		}
		
		List<byte[]> datas = new ArrayList<>();
		
		for (Segment seg : segs) {
			byte[] data = new byte[seg.getSize()];
			copyData(data, seg);
			relocData(data, seg, addrs);
			datas.add(data);
		}
		
		return datas;
	}

	private static void copyData(byte[] data, Segment seg) {
		byte[] srcData = seg.getData();
		
		if (srcData != null) {
			int srcLen = srcData.length;
			
			System.arraycopy(srcData, 0, data, 0, srcLen);
		}
	}

	private static void relocData(byte[] data, Segment seg, int[] addrs) throws HunkParseError {
		Segment[] toSegs = seg.getRelocationsToSegments();
		
		for (Segment toSeg : toSegs) {
			Reloc[] relocs = seg.getRelocations(toSeg);
			
			for (Reloc r : relocs) {
				reloc(data, r, addrs[seg.getId()], addrs[toSeg.getId()]);
			}
		}
	}
	
	private static void reloc(byte[] data, Reloc reloc, int sourceAddress, int targetAddress) throws HunkParseError {
		int offset = reloc.getOffset();
		if (offset < 0 || offset > data.length - reloc.getWidth()) {
			throw new HunkParseError("Relocation offset is outside its source segment");
		}

		ByteBuffer buf = ByteBuffer.wrap(data);
		long relocationDelta;
		switch (reloc.getKind()) {
		case ABSOLUTE:
			relocationDelta = Integer.toUnsignedLong(targetAddress);
			break;
		case PC_RELATIVE:
			relocationDelta = (long) targetAddress - sourceAddress - offset;
			break;
		case BASE_RELATIVE:
			throw new HunkParseError("Base-relative relocation requires a linker-defined base register and cannot be loaded as an executable relocation");
		default:
			throw new HunkParseError("Unknown relocation kind");
		}

		switch (reloc.getWidth()) {
		case 4: {
			long value = Integer.toUnsignedLong(buf.getInt(offset)) + reloc.getAddend() + relocationDelta;
			if (value < 0 || value > 0xffffffffL) {
				throw new HunkParseError("32-bit relocation overflows its destination");
			}
			buf.putInt(offset, (int) value);
		} break;
		case 2: {
			long value = buf.getShort(offset) + reloc.getAddend() + relocationDelta;
			if (value < Short.MIN_VALUE || value > Short.MAX_VALUE) {
				throw new HunkParseError("16-bit relocation overflows its destination");
			}
			buf.putShort(offset, (short) value);
		} break;
		case 1: {
			long value = buf.get(offset) + reloc.getAddend() + relocationDelta;
			if (value < Byte.MIN_VALUE || value > Byte.MAX_VALUE) {
				throw new HunkParseError("8-bit relocation overflows its destination");
			}
			buf.put(offset, (byte) value);
		} break;
		default:
			throw new HunkParseError("Unsupported relocation width");
		}
	}
}
