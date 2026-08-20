# Amiga overlay-loader validation

This note records observed behaviour of the overlay support added on
`feature/amiga-overlay-loader`.  It is intentionally about loader output, not
game semantics.

## Test subject

- Program: `Treasure` (SSI, *Treasures of the Savage Frontier*), extracted
  from the user's Amiga disk image.
- Import: fresh Ghidra 12.1.2 import using this extension.
- Result: analysis completed with 1,282 functions and no active analysis task.

## Observed overlay layout

The imported program contains a root address space and six separately mapped
overlay spaces.  The root contains `HUNK_OVERLAY_TABLE` at
`0x0025bee0-0x0025bf5f` (128 bytes).  The overlay blocks are:

| Address space | Range |
| --- | --- |
| `OVERLAY_01` | `0x0021f000-0x0021fe77` |
| `OVERLAY_02` | `0x0021f000-0x0022746f` |
| `OVERLAY_03` | `0x0021f000-0x00221aeb` |
| `OVERLAY_04` | `0x0021f000-0x00224957` |
| `OVERLAY_05` | `0x0021f000-0x00221ebf` |
| `OVERLAY_06` | `0x0021f000-0x0021f3ff` |

This separate-space representation is required: all overlay nodes are loaded
at the same run-time address, so mapping them into one flat memory block would
produce false control-flow and data references.

## MANX overlay dispatch validation

The loader identified the MANX overlay table and converted its dispatch
entries into computed-call references from root trampolines.  On fresh import,
each target below was created as a function in the corresponding overlay
space:

| Root call site | Overlay target |
| --- | --- |
| `0x0021fe68` | `OVERLAY_01::0x0021f374` |
| `0x0021fe90` | `OVERLAY_02::0x0021ffb4` |
| `0x0021ffa0` | `OVERLAY_03::0x0021f1e2` |
| `0x0021ffd0` | `OVERLAY_04::0x0021f134` |
| `0x00220000` | `OVERLAY_05::0x0021fc00` |
| `0x00220030` | `OVERLAY_06::0x0021f152` |

These are structural loader facts.  They do not establish what the functions
do; game-specific names belong in `treasure.md` only when supported by code
and disk data.

## MANX A4 startup contract

Treasure's root startup chain reaches `0x0025a790`, whose body is exactly:

```asm
lea 0x00227282.l,A4
rts
```

The loader recognizes this only after the MANX overlay table and trampolines
validate. Its recognizer follows the root entry's direct startup transfers,
requires a directly called two-instruction A4 helper, and requires the base to
be in resident root DATA/BSS. A valid result is applied as register context to
executable root and overlay blocks. This avoids treating arbitrary A4 use or
an unvalidated per-function guess as a global base.

## MANX A4 call stubs

The same startup base identifies MANX calls such as `jsr (-0x7614,A4)`.
In Treasure that effective address is `0x0021fc6e`, in root `DATA_01`, whose
bytes are `jmp 0x002551c0`. The stub is executable despite its Hunk type.
The loader maps only this exact `jsr (d16,A4)` to root `jmp absolute.l` form,
with a destination inside resident root code. It creates a function at the
stub plus a computed-call reference from the call site and a jump reference to
the final destination.

Fresh-import verification created 2,025 functions (up from 1,282 before A4
stubs were materialized). A representative chain is:

```
OVERLAY_01::0x0021f798  --computed call-->  0x0021fc6e
0x0021fc6e (ManxA4CallStub_21FC6E)  --jump-->  0x002551c0
```

Ghidra's 68000 decompiler continues to display the source instruction as an
indirect call because its p-code operation is `CALLIND`; the imported xref and
the listing navigation are nevertheless correct and persistent. The loader
does not rewrite executable bytes or lie about the instruction form merely to
make pseudocode prettier.
