# Runnable Amiga toolchain corpus

This directory is the repository's canonical, runnable evidence for the
compiler-specific Hunk loader and analyser support. It contains the original
Amiga media selected for this work, the extracted host volumes used by Vamos,
searchable documentation, and the sample projects whose outputs are exercised
by the extension tests.

It is intentionally distinct from `work/`:

- `toolchains/` is versioned and reproducible.
- `work/` is local operational and investigation state. It is not an input to
  a verification run once this corpus is complete.
- `src/test/resources/fixtures/` contains the small artifacts loaded directly
  by JUnit. Each such artifact is identified in the relevant toolchain README
  and must match the corresponding corpus output's SHA-256.

## Integrity

Run the following from the repository root after changing any corpus file:

```powershell
.\scripts\generate-toolchain-manifest.ps1
.\scripts\generate-toolchain-manifest.ps1 -Verify
```

`MANIFEST.sha256` records every binary and text artifact below this directory.
It deliberately does not include itself.

## Scope

The snapshots currently cover:

- Aztec C 5.0a / MANX: compiler ABI, runtime A4 anchor, relocations, and
  SegLoad experiments.
- SAS/C 6.50: `c.o` LinkerDB A4 context and SLink overlay output.

The snapshots are historical evidence, not a package-management mechanism.
Do not replace a tool, disk, or generated sample in place without recording
its provenance and regenerating the manifest.
