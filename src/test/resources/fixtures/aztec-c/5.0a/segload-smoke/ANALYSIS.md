# Recovered Aztec/Manx overlay layout

`segload-smoke` is a minimal real linker output, so its layout is useful as a
format and ABI reference rather than as game-specific reverse-engineering
notes.

## Resident startup and A4

The entry at `0x0021f000` branches to the Aztec runtime startup.  That startup
directly calls this helper:

```text
0021f0a4  lea  0x002278ba.l,A4
0021f0aa  rts
```

`0x002278ba` is deliberately outside the program's mapped DATA/BSS blocks.
It is a small-data anchor, positioned so signed 16-bit A4 displacements reach
resident globals below it.  For example, the startup immediately uses
`lea (-0x7fda,A4),A1`, which resolves to `0x0021f8e0` inside resident
`DATA_01`.

Consequently, a loader must validate the canonical startup helper and apply
its A4 value even when the anchor itself is not mapped.  Requiring the anchor
to lie inside DATA/BSS rejects genuine Aztec/Manx output.

## Overlay call

The root program calls `overlay_entry` through an A4-relative entry:

```text
0021f020  jsr  (-0x7ff6,A4)       ; 0x0021f8c4
0021f8c4  61 00 00 06 01 00 00 00
```

The eight-byte on-disk entry is a MANX `.segload` trampoline: a `BSR.W` to the
resident overlay manager, followed by a one-based node number (`1`) and a
24-bit target offset (`0`).  The `HUNK_OVERLAY` table identifies that node as
the overlay code hunk.  The extension therefore represents the target as the
separate address-space entry `OVERLAY_01::0x0021f000` and creates the computed
cross-overlay call there.

`aztec-segload-multi` verifies the same mechanism with two targets and a
non-zero target offset.
