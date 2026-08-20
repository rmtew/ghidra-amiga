# Lattice C 4.00

This independent corpus entry retains all four original Lattice C 4.00 ADFs.
The shipped hard-disk installer requires only disks 1 and 2 for its core
compiler layout.  Disk 3 has a documented extraction error in an optional
uncompressed header; it remains preserved in `media/` and is not repaired
with files from another release.

`volume/lc/` reproduces the installer's core layout using only readable 4.00
content: compiler/linker tools under `c/`, compressed headers under
`include/`, and runtime libraries under `lib/`.  The version-local samples
are built directly with `LC1`, `LC2`, and BLINK; the `lc` driver is not used
because the checked Vamos release does not implement its `CreateProc` phase
launch.

## Release evidence and validation

- `lc`, `LC1`, and `LC2` identify themselves as Lattice AmigaDOS C Compiler
  version 4.00.
- BLINK identifies itself as version 7.2, credited to Software Distillery
  and SAS Institute.
- The runtime sample is 3,904 bytes with SHA-256
  `1F7401680C67DA8322A09AAE7FC5586244A166ECCA53CE402F82DBE2FF523903`.
- The overlay sample is 4,376 bytes with SHA-256
  `D65F6A40336F8105F9DEF9017A613D4685BEBD76A05E07C49041A6392EC3FB69`.

The shipped `source/c.a` explicitly identifies `LinkerDB` as linker-defined
and loads it into A4. Its prologue is independently recognized by the
extension and tested from the retained runtime fixture.

BLINK 7.2 places `HUNK_OVERLAY` between the first and remaining root Hunks,
then emits its normal overlay `HUNK_HEADER`/`HUNK_BREAK` node at the file
position recorded in that table. The loader recognizes this documented
interleaved-root ordering, preserving root logical slots and the table-backed
overlay hierarchy rather than treating the metadata block as an early root
terminator.
