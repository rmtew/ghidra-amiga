# SAS/C 6.50 runtime A4-overlay fixture

This source-complete fixture combines the standard SAS/C 6.50 `c.o` runtime,
an A4-relative global, and one SLink overlay node. It verifies that a
compiler-validated `LinkerDB` context applies to both resident and overlay
code without relying on a MANX overlay convention.

Build provenance (the compiler, libraries, and overlay manager are not
included):

```text
sc root.c
sc global.c
sc overlay.c
slink WITH withfile
```

`ovs.o` is the shared SAS/C 6.50 overlay manager from `../manager` when
reproducing the link.

SHA-256:

```text
FEC6189959AFF645205ED085D2AD7C9F1C89D889DDDC20EAA748CC70537E2CB2  root.o
D22AC206601C5393D97489A04664EC1B2AAAE118A7AF122AC32306CAE26266B3  global.o
C6CD133E2C9C6C242D4466D905897D2363B3A39193CA41D40CE561AF2577D843  overlay.o
F93693D0CBD280416C2BDB097D6B2F1F28DE1CBF4DA7F8A2473FD8770FFE612F  sas-runtime-a4-overlay
```
