# Aztec C segmented-code smoke fixture

This fixture is a minimal, generated Aztec C 5.0a segmented executable.  It
exists to exercise the documented MANX/Aztec `HUNK_OVERLAY` representation
with a real linker output rather than a hand-assembled byte stream.

It contains no files from the Aztec distribution: `root.c` and `overlay.c`
are this repository's sources, while `root.o`, `overlay.o`, and
`segload-smoke` are their generated outputs.

## Build provenance

The local build used the user's Aztec C 5.0a media and Vamos.  The compiler,
linker, runtime library, disk images, and emulator patch are deliberately not
part of this repository.

```text
cc root.c
cc overlay.c
ln -o segload-smoke +o0 root.o aztec:lib/c.lib +o1 overlay.o aztec:lib/c.lib
```

The `+o0` root and `+o1` overlay directives cause the linker to include the
MANX runtime and `.segload` manager in the resident node and place
`overlay_entry` in the independently loadable node.

SHA-256:

```text
B2016A0B6ADE7341F63981C7C3D0036BDA7294C8B769DCEC7E2DFB6B77C5B18D  root.o
0A0366625D78CF663AC5309500B725E9770CD98491970D9FF958D8CA08C2197A  overlay.o
D0A3F881D411C8AEA1B59F94DA329DAE01681D075A5D041868ED020920D6EF70  segload-smoke
```

## Recovered structure

The executable has resident `CODE`, `DATA`, and `BSS` hunks, a MANX
`HUNK_OVERLAY` table, and one overlay `CODE` hunk.  Ghidra maps the latter in
the `OVERLAY_01` address space.

`main` calls `overlay_entry` through the A4-relative root trampoline at
`0x0021f8c4`.  Its eight-byte initial form is:

```text
61 00 00 06 01 00 00 00
BSR.W +6, overlay-node 1, target offset 0
```

The extension validates that record, creates the computed-call reference, and
materialises the target as `OVERLAY_01::0x0021f000`.  That target is the
compiled empty `overlay_entry()` body.  Once the MANX loader has loaded the
node, it rewrites this entry to an absolute jump; the on-disk form above is
the one an analyzer must recognise.
