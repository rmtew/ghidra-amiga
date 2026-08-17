# SAS/C 6.50 basic overlay fixture

This is a real SAS/C 6.50 `sc`/`slink` output generated from the adjacent
`root.c` and `overlay.c`.  The root calls `overlay_value()` across one
hierarchical overlay-node boundary.

`withfile` is the SLink control file.  The linked output contains a root
`HUNK_HEADER` whose global hunk table reserves hunk 3, a hierarchical
`HUNK_OVERLAY` table, then an overlay-node `HUNK_HEADER` for hunk 3 terminated
by `HUNK_BREAK`.

The shared [SAS/C overlay manager](../manager/README.md) is retained centrally
with its original source, the minimal SLink-6.50 compatibility variant, and
the assembled `ovs.o` link input.  To reproduce this link, stage that `ovs.o`
next to `withfile` before invoking SLink.

```text
71C3DBDA253E7A9FD1E282F543F5F445D868F6292AC566089384E960BE7D27F7  root.o
6A57D918DEC41FAD85B0B4F4DE2A00A818CF6182F5E016288035642C5FD676FE  overlay.o
B16762060F6EE476ADD9A1E8EC92FCCDCA57DB501CA79AA0E9B1E66AEAA68E3B  sas-basic
```
