# Commodore ALink overlay fixture

`ALINK` is the 29,364-byte Amiga object linker distributed with Commodore's
Native Developer Kit.  This fixture is the byte-identical copy supplied in
NDK 1.3, 2.0, and 3.1:

```text
EC8E7CBCEAFFBB32FF4FCBB900B99F90AF18D9F168C21143CC772FE4434D7D69  ALINK
```

It is included only as a small, real Hunk compatibility fixture.  It is not a
replacement for the NDK and this repository does not include NDK
headers, libraries, documentation, disk images, ROMs, or Workbench media.

The executable's code/data happens to contain words equal to `HUNK_OVERLAY`
and `HUNK_BREAK`.  They are *not* top-level Hunk records: ALink is a normal,
single-node executable.  Its regression test makes that distinction explicit,
so the parser cannot invent an overlay from a Hunk-tag value embedded in a
segment.
