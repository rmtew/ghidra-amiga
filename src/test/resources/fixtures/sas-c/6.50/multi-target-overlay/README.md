# SAS/C 6.50 multi-target overlay fixture

This real SAS/C 6.50 `sc`/`slink` output has two root-to-overlay calls:
`overlay_first()` and `overlay_second()`.  Both targets occupy the same
independently loadable overlay node, so the hierarchical `HUNK_OVERLAY` table
contains two symbol entries for hunk 3.

The toolchain and control-file convention match the adjacent basic fixture.
The shared [SAS/C overlay manager](../manager/README.md) supplies the original
source, minimal SLink-6.50 compatibility source, and assembled `ovs.o` input.
To reproduce this link, stage that object next to `withfile` before invoking
SLink.

```text
E3BBA87C6CE043C7CCFBFFE23BE3324BD3D4C0063BFA9A6CF82221F8726AC16E  root.o
9865986CB7AC301ED3BB5B75DF21DA923C7F370FA2C18EB1445CEF4024B9E8CA  overlay.o
C3FE975A349F6F147DAB1E04ADEF42E976D5F5D828B7844369911A69F643FE6E  sas-multi
```
