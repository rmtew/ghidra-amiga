# MANX runtime recognition notes

This note describes reusable reverse-engineering patterns for executables
compiled with MANX/Aztec C. It is not a description of any particular game.

## Calling convention

MANX uses the normal 68000 long-word return address, so the first parameter is
at `SP+4` on callee entry. Stack parameters then occupy **two-byte slots**,
with narrow values left-justified in their slot: a `char` followed by a
`ushort` is at `SP+4` and `SP+6`, not at `SP+4` and `SP+8`. The extension
supplies this as the `68000:BE:32:MANX` language and
`MANX C` compiler specification. A recognised flat MANX overlay table selects
it automatically; select that language manually for a separately proven,
non-overlay MANX executable.

## A4 dispatch stubs

A MANX program can use an A4-relative call table. A call of the form
`jsr (d16,A4)` may reach a root-resident `jmp absolute.l` forwarding stub.
Once the destination is proved, rename the stub after its target (for example
`Function_Thunk1`) rather than leaving a generic generated stub name. This
improves the call graph and the decompiler without changing program bytes.

Keep generic names for unproved entries: an A4 displacement alone does not
identify a library routine.

## Address-taken callbacks

Some MANX initializers construct an A4 callback table with
`lea target(PC),Ax; move.l Ax,(d16,A4)`. The reference to `target` is data,
so normal function-start analysis cannot know whether it is a callback, a
string, or other data. The `MANX A4 Callbacks` analyzer therefore promotes
only executable targets that begin with a 68000 `LINK` prologue. It names the
result after its A4 slot and records that it is address-taken; it does not
invent a static caller when the callback may be invoked by an event loop.

## Long arithmetic helpers

Common MANX runtime support uses register arguments and is best documented as
runtime support rather than application logic:

| Pattern | Role |
| --- | --- |
| 16-bit partial products combined into D0 | Low 32 bits of a 32-bit multiply. |
| Wrapper around the unsigned division core which restores D1 | Unsigned 32-bit quotient. |
| Sign-normalising wrapper around unsigned division | Signed 32-bit quotient. |
| Unsigned division with a `DIVU` fast path and restoring loop | 32-bit quotient/remainder core. |
| Wrapper returning the core's remainder | Signed 32-bit modulo. |
| Byte scan to NUL, subtracting the initial pointer | `strlen`. |

The helpers conventionally receive operands in D0 and D1. The division core
returns the quotient in D0 and remainder in D1; wrappers may preserve or
discard the remainder. Verify register preservation and signedness against the
actual instructions before applying names.

The extension's `MANX Runtime` analyser performs that naming automatically
only when all of the required properties match: partial-product multiplication,
the unsigned division core's two `DIVU` operations plus restoring `DBF` loop,
an exact unsigned/signed wrapper that directly calls that recognised core, or
the conventional `strlen` instruction sequence. It
does not match by address or game-specific byte sequence and leaves existing
non-default function names alone.

## Analysis guidance

After proving a semantic target, name every forwarding hop in that call chain:
the A4 call-table entry, any near wrapper, and the final routine. This makes
decompiled application code readable while preserving the distinction between
MANX runtime code and program-specific functions.
