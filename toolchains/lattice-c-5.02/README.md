# Lattice C 5.02

## Contents

- `media/` contains the five compiler ADFs and the two supplied Lattice C
  5.0 documentation ADFs used as the source for the searchable manuals.
- `volume/lc/` reproduces the layout made by the shipped `s/install_hd`
  script: compiler tools under `c/`, headers under `include/`, libraries
  under `lib/`, plus the installed examples and source.
- `docs/` contains the original plain-text manuals extracted from the
  documentation ADFs.  They describe the 5.0 base product; the 5.02
  compiler/linker tool banners are recorded in the local release evidence.
- `samples/runtime-a4/` builds the small base-relative-data executable used
  by the Lattice C A4 regression fixture.
- `samples/overlay/` builds a root plus one overlay node using BLINK's
  documented `WITH` file syntax.

## Tool version evidence

The retained `read.me` and disk-volume labels say Lattice C 5.0, but the
actual shipped executables identify the patched release:

- `lc`: Lattice AmigaDOS C Compiler, Version 5.02
- `lc1` and `lc2`: Lattice AMIGA 680x0 C Compiler, V5.02
- `blink`: Blink - Version 5.02

The corpus is therefore named after the executable toolchain version.

## Vamos invocation

The checked emulator is `amitools v0.1.dev1+g3b57f2052.d20260313`.  The
`lc` driver itself invokes compiler phases with `CreateProc`, which that
Vamos version does not implement.  Run the documented phases directly.

```powershell
$lattice = (Resolve-Path toolchains/lattice-c-5.02/volume/lc).Path.Replace('\','/')
$sample = (Resolve-Path toolchains/lattice-c-5.02/samples/runtime-a4).Path.Replace('\','/')
$vamos = "$env:USERPROFILE/.local/bin/vamos.exe"

& $vamos -V "lattice:$lattice" -V "work:$sample" `
  -a 'lc:lattice:c' -a 'lib:lattice:lib' -a 'include:lattice:include' `
  -p lc: --cwd work: lc:lc1 -- -b root.c
& $vamos -V "lattice:$lattice" -V "work:$sample" `
  -a 'lc:lattice:c' -a 'lib:lattice:lib' -a 'include:lattice:include' `
  -p lc: --cwd work: lc:lc2 root.q
& $vamos -V "lattice:$lattice" -V "work:$sample" `
  -a 'lc:lattice:c' -a 'lib:lattice:lib' -a 'include:lattice:include' `
  -p lc: --cwd work: lc:blink 'FROM root.o,lib:c.o LIB lib:lc.lib TO lattice-runtime-a4'
```

The `--` before `-b` is required: otherwise Vamos would consume the compiler
option.  The generated `lattice-runtime-a4` SHA-256 is:

```text
5BFFFDC9F34372C1A6AAD16D037B3D452600F99900520A539A9239281E25F9E1
```

For the overlay sample, compile `root.c` and `overlay.c` with the first two
commands, then run `lc:blink WITH lattice-overlay.lnk`.  Its output SHA-256
is recorded in the sample README.

## Sample/test relationships

| Corpus sample | Purpose | Test fixture |
| --- | --- | --- |
| `runtime-a4/` | Lattice C 5.02 `LinkerDB` A4 context | `fixtures/lattice-c/5.02/runtime-a4/` |
| `overlay/` | BLINK standard hierarchical overlay layout and A4 propagation | `fixtures/lattice-c/5.02/overlay/` |
