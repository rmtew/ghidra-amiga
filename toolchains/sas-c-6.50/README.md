# SAS/C 6.50

## Contents

- `media/` contains the six canonical SAS/C 6.50 ADFs extracted directly from
  the original per-disk archives in the user-supplied disk collection.
- `volume/` is the working extracted SAS/C environment. The runnable compiler
  and linker are at `volume/disk6/sc/c/sc` and `volume/disk6/sc/c/slink`.
- `docs/` contains the supplied searchable SAS/C text documentation.
- `samples/hello-overlay/` is the small original overlay experiment. The
  runtime-A4 overlay source and output remain in the preserved volume snapshot
  at `volume/disk6/sc/samples/sas-runtime-a4-overlay/`.

## Vamos invocation

The checked tool version is `amitools v0.1.dev1+g3b57f2052.d20260313`.
The SAS/C volume requires the compiler command path and `LIB:` assign:

```powershell
$sc = (Resolve-Path toolchains/sas-c-6.50/volume/disk6/sc).Path.Replace('\','/')
$sample = (Resolve-Path toolchains/sas-c-6.50/volume/disk6/sc/samples/sas-runtime-a4-overlay).Path.Replace('\','/')
vamos -V "sc:$sc" -V "work:$sample" -p sc:c --cwd work: sc:c/sc root.c
vamos -V "sc:$sc" -V "work:$sample" -p sc:c --cwd work: sc:c/sc global.c
vamos -V "sc:$sc" -V "work:$sample" -p sc:c --cwd work: sc:c/sc overlay.c
vamos -V "sc:$sc" -V "work:$sample" -p sc:c -a lib:sc:lib --cwd work: sc:c/slink WITH withfile
```

The generated `sas-runtime-a4-overlay` SHA-256 is:

```text
F93693D0CBD280416C2BDB097D6B2F1F28DE1CBF4DA7F8A2473FD8770FFE612F
```

It is byte-identical to
`src/test/resources/fixtures/sas-c/6.50/runtime-a4-overlay/sas-runtime-a4-overlay`,
which is consumed by `AmigaHunkLoaderTest`.

## Analysis relation

The sample proves the SAS/C 6.50 `c.o` startup establishes `A4` at LinkerDB
and that the loader applies that same validated context to the SLink overlay
node. This is independent from MANX A4 call stubs and ABI selection.
