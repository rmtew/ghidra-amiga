# SAS/C 6.50 overlay manager

`ovs.a` is the SAS/C 6.50 AmigaDOS 68000 overlay-supervisor source supplied
on the compiler's source disk.  It is included here because it is both the
source of the manager linked into these fixtures and the most direct
documentation of SAS/C's HUNK_OVERLAY convention.

SLink 6.50 expects the C-linkage symbol `_ovlyMgr`.  The supplied source's
normal non-`NEWOCV` path exports `ovlyMgr`, so `ovs-slink-6.50.a` changes only
those two declarations (the `xdef` and label) to `_ovlyMgr`.  `ovs.o` was
assembled from that compatibility source with the SAS/C 6.50 `asm` tool and is
the manager object used by every fixture in this version directory.

```text
FE75BA6A7CE58A67CE56824583D55845BA9F9053AF88B793E306121D9EEFEA44  ovs.a
6A592DE2CF8A4315A83C29B12EFA5E074978BA7FFA8349B68AF892DA91D19244  ovs-slink-6.50.a
B7C92B89ECE62031CF48013E1C6A9D493501DDFBAC41AD4A3C959743CB8EFAE1  ovs.o
```
