# Aztec C 5.0a: relocation addend minus two

This minimal, runnable Hunk executable is the regression fixture for an
absolute relocation whose field refers to two bytes before the base of another
loaded segment.

`base-minus-two.s` is assembled and linked by the user-provided Aztec C 5.0a
toolchain. It has one code segment and one initialized data segment. The code
contains:

```asm
movea.l #relocation_target-2,a0
move.w  (a0),d0
rts
```

The linker writes `0xfffffffe` to the four-byte immediate field and emits an
absolute 32-bit relocation to the data segment. At runtime that field becomes
the data segment base minus two. The subsequent word load is deliberately an
access to the loader-owned segment prefix rather than to the mapped data
segment itself.

## Provenance and rebuild

Producer: Manx Aztec C 5.0a (assembler and linker dated 9 January 1990), run
through Vamos. The compiler media is user-provided and is not included here.

From the repository root, using the ignored extracted Aztec volume:

```powershell
$env:UV_CACHE_DIR = (Resolve-Path '.uv-cache').Path
$volume = (Resolve-Path 'work/aztec-c-v5/volume').Path
$fixture = (Resolve-Path 'src/test/resources/fixtures/aztec-c/5.0a/relocation-addend-minus-two').Path
vamos -V "aztec:$volume" -V "fixture:$fixture" --cwd fixture: -p aztec:bin as base-minus-two.s
vamos -V "aztec:$volume" -V "fixture:$fixture" --cwd fixture: -p aztec:bin ln -- -o base-minus-two base-minus-two.o
```

Verification command:

```powershell
hunktool info -R -x base-minus-two
```

Expected evidence includes:

```text
00000000: 20 7c ff ff ff fe 30 10 4e 75 00 00
reloc absreloc32 #1
00000002 Segment #1
```

The fixture runs under Vamos and exits with status `114` on the recorded
environment, the word observed at data-segment-base minus two.

## Ghidra presentation limitation

The loader records this relocation as an imported `OffsetReference`: its base
is the data-segment base and its signed offset is `-2`. That semantic
information is retained in the program database.

Ghidra 12.1.2's 68000 listing renderer nevertheless displays the immediate as
the resolved scalar (`#0x21f00a` in the imported fixture), rather than an
expression such as `DATA_01_BASE-2`. This is a native presentation limitation,
not a relocation-loader arithmetic failure. Do not mask it with a synthetic
label or equate at the resolved, out-of-segment address: that would make the
listing look symbolic while misrepresenting what the symbol denotes.

`AmigaHunkLoaderTest.documentsCurrentGhidraRenderingOf68000OffsetReferences`
locks down both facts. It confirms that the imported offset reference keeps
its base and addend, then deliberately records the current scalar rendering.
If Ghidra gains native offset-expression rendering, the test should fail and
be consciously updated along with this note.

SHA-256 (`base-minus-two`):

```text
e8a652cc98cfbce318fe9f15ed3f567c7016773bda147e0d7878d080f12b2e4f
```
