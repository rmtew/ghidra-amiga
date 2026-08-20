# Lattice C 3.03

This corpus preserves the complete original single-disk `C-DEVEL` compiler
environment. Its own example scripts establish the direct build sequence:
`LC1`, `LC2`, then ALINK with `LStartUp.obj`, `lc.lib`, and `amiga.lib`.

`volume/c-devel/` retains the source disk layout rather than translating it
to a later Lattice release's `lc/` installation. The version-local runtime
sample is retained here for comparison.

## Validation status

`LC1` and `LC2` run under the checked Vamos release using the native command
form from the supplied `MakeSimple` script. ALINK 3.03 does not yet run under
that Vamos build: it reads immediately beyond the configured RAM end (at
`0x100001` with 1 MiB and `0x200001` with 2 MiB). This is an emulator-model
issue, not a source or media failure, and is recorded rather than worked
around with a later Lattice linker or executable.

Consequently, C 3.03 is a preserved, phase-validated corpus entry but not
yet a linked-output regression profile. A future compatible emulator/runtime
must complete the native ALINK command from `examples/MakeSimple` before a
fixture or analyser convention is claimed for this version.
