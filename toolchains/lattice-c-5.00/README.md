# Lattice C 5.00

This is a separate corpus entry for the Lattice C 5.00 compiler/linker
release.  It is retained independently from 5.02 even where the emitted
Hunk layout or startup convention matches: cross-version comparison is a
primary purpose of the corpus.

`media/` retains all five original ADFs.  The supplied disks 3 and 4 are
marked as damaged in their source archive.  Their ADFs are preserved, while
the runnable `volume/lc/` is reproduced only from files that can be read
from 5.00's own disks 1, 2, and 5.  No files from 5.02 are substituted.

The initial volume contains the compiler phases, BLINK, headers, startup
object, and libraries required for the versioned regression samples.  The
missing optional material from damaged disks will remain documented rather
than silently repaired from another release.

## Release evidence and validation

The retained 5.00 binaries identify themselves as follows:

- `lc`: Lattice AmigaDOS C Compiler, Version 5.0
- `lc1` and `lc2`: Lattice AMIGA 680x0 C Compiler, V5.0
- `blink`: Blink - Version 5.0

The direct `lc1`, `lc2`, and BLINK sequence succeeds under Vamos for both
version-local samples.  The `lc` driver itself is not used because its
`CreateProc` phase launch is unsupported by the checked Vamos release.

| Sample | Output | Size | SHA-256 |
| --- | --- | ---: | --- |
| runtime A4 | `samples/runtime-a4/lattice-runtime-a4` | 2,812 bytes | `44CA9F9BADEFD085E5EF514B216277365819697B96DECF7705B477937BEFD3A2` |
| BLINK overlay | `samples/overlay/lattice-overlay` | 3,300 bytes | `2F090F9E52689B8ED65E1CD9AF1FDE11A918B093243473D7A9ABB184C80CB115` |

The startup uses Lattice's validated `LinkerDB` A4 initialization convention,
but begins 24 bytes into a root code Hunk.  The loader consequently locates
the exact CRT sequence anywhere in resident code, requiring a unique match;
it does not infer the convention from the release name.
