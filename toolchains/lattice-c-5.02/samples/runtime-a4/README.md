# Lattice C 5.02 A4 fixture

`root.c` is compiled with `lc1 -b` so its global data uses Lattice C's
base-relative model.  The shipped `c.o` CRT sets A4 to BLINK's linker-defined
`LinkerDB`.  The generated executable is byte-identical to
`src/test/resources/fixtures/lattice-c/5.02/runtime-a4/lattice-runtime-a4`.

`root.o` is retained as an intermediate evidence artifact.  `root.q` is an
ephemeral compiler intermediate and is intentionally not retained.
