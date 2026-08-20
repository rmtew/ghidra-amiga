# Aztec C 5.0a / MANX

## Contents

- `media/` contains the four canonical Aztec C 5.0a ADFs copied from the
  user-supplied disk collection.
- `volume/` is the extracted, Vamos-ready tool volume. Its `bin/` directory
  contains `cc`, `as`, and `ln`; `lib/c.lib` is the normal C runtime library.
- `docs/Aztec-C-v5.0-manual.txt` is a searchable `pdftotext -layout`
  conversion of *Aztec C v5.0 - Manual-ENG.pdf*. The source PDF was not
  altered and is retained outside this repository's corpus.
- `samples/` contains the authored source, objects, linker inputs, and output
  files used during loader/analyser validation.

## Vamos invocation

The checked tool version is `amitools v0.1.dev1+g3b57f2052.d20260313`.
From the repository root, mount the copied volume and a chosen sample:

```powershell
$aztec = (Resolve-Path toolchains/aztec-c-5.0a/volume).Path.Replace('\','/')
$sample = (Resolve-Path toolchains/aztec-c-5.0a/samples/runtime-a4-plain).Path.Replace('\','/')
vamos -V "aztec:$aztec" -V "work:$sample" -p aztec:bin --cwd work: aztec:bin/cc root.c
vamos -V "aztec:$aztec" -V "work:$sample" -p aztec:bin --cwd work: aztec:bin/ln -- -o manx-runtime-a4 root.o aztec:lib/c.lib
```

The `--` before `ln` options is required so Vamos does not parse `-o` itself.
The resulting `manx-runtime-a4` SHA-256 is:

```text
7808A634BBEB21B0C95088C0BC15806F860E582AF94412B8CDC2D60D909EFC76
```

It is byte-identical to
`src/test/resources/fixtures/aztec-c/5.0a/runtime-a4-plain/manx-runtime-a4`,
which is consumed by `AmigaHunkLoaderTest`.

## Sample/test relationships

| Corpus sample | Purpose | Test fixture |
| --- | --- | --- |
| `runtime-a4-plain/` | Plain MANX CRT A4 recovery | `fixtures/aztec-c/5.0a/runtime-a4-plain/` |
| `relocation-base-minus-two/` | `HUNK_RELOC32` base-minus-two addend | `fixtures/aztec-c/5.0a/relocation-addend-minus-two/` |
| `segload-smoke/` | Aztec SegLoad smoke executable | `fixtures/aztec-c/5.0a/segload-smoke/` |
| `segload-multi/` | Multi-module SegLoad executable (`segload-multi.lnk`) | `fixtures/aztec-c/5.0a/segload-multi/` |
| `runtime-helpers/` | MANX runtime helper recognition probe | no direct fixture yet |

The source PDF's SHA-256 and the conversion command are recorded in
`docs/README.md`.
