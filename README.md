# ghidra-amiga
based on [ghidra_amiga_ldr](https://github.com/lab313ru/ghidra_amiga_ldr) by Vladimir Kononovich (No license)
and [ghidra-amiga-whdload](https://github.com/apparentlymart/ghidra-amiga-whdload) by Martin Atkins (MIT license)

see https://github.com/astrelsky/vscode-ghidra-skeleton

## Development

`data/amiga_ndk39.gdt` is a checked-in extension resource generated from the
vendored NDK 3.9 headers. Rebuild it with Ghidra's headless launcher:

```powershell
$project = Join-Path $env:TEMP 'amiga-ndk-gdt-project'
New-Item -ItemType Directory -Force $project | Out-Null
& '<GHIDRA_INSTALL_DIR>\support\analyzeHeadless.bat' $project NdkArchiveBuild `
  -scriptPath "$PWD\ghidra_scripts" -noanalysis `
  -postScript CreateAmigaNdk39Gdt.java "$PWD" "$PWD\data\amiga_ndk39.gdt"
```

The script consumes `data/ndk39_headers.txt`, validates that it covers every
non-forwarding declaration header under `3rdparty/NDK_3.9/include_h`, and then
parses the headers for the 68000 ABI. The manifest deliberately excludes only
the `clib/`, `proto/`, `pragma/`, and `pragmas/` forwarding-declaration trees.
SAS/C calling-convention annotations are neutralised only while parsing; the
vendored headers remain unchanged. The build fails if a header is missing,
stale, duplicated, or omitted.

## Debugging
- Build with Ghidra's bundled Gradle wrapper, then install the generated ZIP from `dist/` using
  **File -> Install Extensions...**. Close Ghidra before replacing an already-installed version;
  Windows locks the extension JAR while Ghidra is running.

## Overlay executables

The Hunk loader recognises root and overlay nodes as separate Ghidra overlay address spaces. It retains
the `HUNK_OVERLAY` payload in a read-only `HUNK_OVERLAY_TABLE` block and labels validated runtime
targets without pretending that different overlay spaces are resident simultaneously.

- ALink, BLink and SLink hierarchical tables are decoded when their published layout validates.
- MANX/Aztec C flat tables are decoded, including their `bsr.w` trampolines. Both documented zero-based
  and legacy one-based node identifiers are accepted after validation.
- For a validated MANX table, the loader also recognises the MANX startup chain and its
  `lea absolute.l,A4; rts` helper. It applies that A4 base only when the helper is directly called from
  startup and the resulting address lies in resident root DATA/BSS; the context is then available to root
  and overlay code alike.
- Validated MANX `jsr (d16,A4)` calls through root `jmp absolute.l` stubs are mapped as computed calls.
  This makes compiler-generated call stubs visible even when the linker placed them in a DATA hunk.
- Amiga library analysis automatically materialises FD tables only for known `.library`, `.device`, and
  `.resource` names referenced from executable code. This covers ordinary open calls and compiler wrappers
  without creating every available API table from unrelated filename-like text.
- A structurally recognised MANX flat overlay table selects the extension's `68000:BE:32:MANX`
  language and `MANX C` compiler specification.  It retains the normal 68000 instruction model,
  while correctly allocating stack parameters in two-byte slots.
- The `MANX Runtime` function analyser runs only for that ABI. It recognises a conservative set of
  compiler runtime arithmetic and string helpers from instruction/control-flow shape and propagates those
    names to loader-created A4 forwarding stubs. It never relies on game addresses or overwrites
    existing non-default names.
- `Address-Taken Code Discovery` is enabled automatically for Hunk imports. It promotes only executable
  data-reference targets whose pseudo-disassembled control flow reaches a 68000 return, and records the source
  data reference without inventing an indirect caller or assigning application-specific semantics. On other
  68000 inputs it remains opt-in. Candidates which do not meet that standard remain unchanged as
  `Address-Taken Code Candidate` analysis bookmarks. **Analysis → Address-Taken Code Review** opens a
  modeless table: selecting a candidate navigates to it, while Promote and Dismiss leave the listing available
  for inspection.
- Other overlay-manager tables remain visible as raw metadata; their nodes and ordinary Hunk relocations
  are still imported correctly, but no speculative overlay-call targets are created.

Direct Hunk relocations are preserved in Ghidra's relocation table and as overlay-space-aware data
references. Calls through an overlay manager are marked as computed calls only after the table and
trampoline have both validated.

The loader implements the LoadSeg relocation forms with their documented field widths, including
`HUNK_RELRELOC32`, `HUNK_RELRELOC16`, and `HUNK_RELRELOC8`. Base-register relocations in relocatable
object files remain deliberately unresolved: they require the linker-selected register base and are
not executable-file relocations. The `ApplyRegBase` script is for manually proven, non-MANX cases and
applies a value only to the function under the cursor.

When an overlay manager or individual target cannot be validated, the import completes without
inventing references. A `WARNING` is recorded in Import Results and persisted on the
`HUNK_OVERLAY_TABLE` block comment so the limitation remains visible in the saved program.

The test suite includes an optional real-file integration test. It is skipped unless
`TEST_HUNK_OVERLAY_PATH` points to a locally available overlay executable; no game binary or other
sample is included in, copied into, or required by this repository.

## TODO
