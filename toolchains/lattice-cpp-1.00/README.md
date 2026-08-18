# Lattice C++ 1.00

This is the unmodified, unmarked two-disk Lattice C++ 1.00 distribution
(October 1988). It is retained separately from every Lattice C release:
disk 1 provides the C and C++ front ends (`cc`, `cpp`, `cfront`, `lc*`, and
`blink`), while disk 2 provides the C++ headers, runtime libraries, and
examples.

`media/` preserves the two ADFs used to build `volume/`; the latter is the
minimum runnable compiler tree, keeping its own tools, headers, libraries,
and examples. `samples/hello/hello.cp` is the vendor's supplied hello-world
source, preserved as a small smoke input rather than claimed as a generated
fixture.

## Validation status

Directly running `c:cc` under Vamos identifies itself as "Lattice C++
compiler, version 1.0" and locates its `LC:` worker binaries when the volume
assignments below are supplied. The driver creates a separate worker with
AmigaDOS `CreateProc`; current Vamos then fails in its allocator while freeing
that worker's memory. Consequently this entry deliberately does **not**
claim a linked output or analyser profile yet. The failure is recorded so a
future Vamos-compatible run can continue from the authentic source media and
volume, without using a different compiler or linker.

```text
vamos -m 4096 \\
  -V lattice:<toolchains/lattice-cpp-1.00/volume> -V work:<sample-dir> \\
  -a c:lattice:c -a LC:lattice:c -a lib:lattice:lib \\
  -a CPInclude:lattice:include -a QUAD:work: -p c: --cwd work: \\
  c:cc -- -L hello.cp
```

The native C++ driver uses `cpp`, `cfront`, `lc`, and `blink` through its
`CreateProc` worker. `samples/hello/build` records those native phases for
reference; it is not executed under the present Vamos build because `Execute`
has the same known allocator/RAM-end limitation.
