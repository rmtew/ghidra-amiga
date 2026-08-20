# Lattice C 5.04 update

This is a distinct patch release, not standalone compiler media. Its retained
`readme.504` explicitly requires a prior Lattice C 5.02 installation. The
corpus therefore preserves the five exact 5.02 base compiler ADFs plus the
original 5.04 update ADF; `volume/lc/` starts as an unmodified separate copy
of that 5.02 install.

The supplied LPATCH 3.0 runs successfully under Vamos when the old binary
and matching `.pch` file are colocated, proving the update disk itself is
readable and operational. The full supplied `install_hd` script cannot yet
run under the checked Vamos release because the required AmigaDOS `Execute`
program hits the emulator's RAM-end access bug. Consequently this entry does
not yet claim 5.04 binaries, outputs, or analyser fixtures. Complete the
native update in a compatible execution environment, then retain the patched
volume and version-local samples separately from 5.02.
