# Lattice C++ 5.10

This is the Lattice C++ 5.10 distribution dated 22 April 1992. It is kept as
its own compiler release even though its two-disk media layout resembles C++
1.00. `media/disk1-m.adf` preserves the supplied `[m]` disk-1 image and
`media/disk2.adf` preserves the supplied unmarked disk-2 image; no damaged
disk-2 variant or older C++ file has been substituted.

Disk 1 supplies the driver and tools; disk 2 supplies the matching C++
headers, runtime libraries, and examples. `volume/` is built only from that
pair. The vendor hello-world source is kept in `samples/hello/` as a smoke
input and the native phase recipe is recorded in its `build` script.

## Validation status

`c:cc` identifies itself as Lattice C++ version 1.0 with 1992 copyright, and
its binary hash differs from C++ 1.00. With `LC:`, `CPInclude:`, `QUAD:`, and
`lib:` mapped to this version-local volume, it finds and starts the worker
used to compile `hello.cp`. Current Vamos fails in its `CreateProc` worker
cleanup (`FreeMem: Unknown memory to free`), so this corpus intentionally
does not claim a linked executable, regression fixture, or ABI profile.

```text
vamos -m 4096 \\
  -V lattice:<toolchains/lattice-cpp-5.10/volume> -V work:<sample-dir> \\
  -a c:lattice:c -a LC:lattice:c -a lib:lattice:lib \\
  -a CPInclude:lattice:include -a QUAD:work: -p c: --cwd work: \\
  c:cc -- -L hello.cp
```

Retain the original media and rerun this command in a Vamos version or Amiga
environment that supports the driver's process lifecycle before making any
analysis claim about C++ 5.10 output.
