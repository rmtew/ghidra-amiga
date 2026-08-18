# `segload-smoke`

This is the minimum Aztec C segmented-code experiment.

1. Compile `root.c` and `overlay.c` with the Aztec `cc` in the adjacent
   `volume/` tree.
2. Link with `ln -o segload-smoke -f segload-smoke.lnk`. The required `--`
   separator belongs to the outer Vamos command, before linker options.
3. Import `segload-smoke` and use its `.sym` output to identify `_segload`,
   the root segment, segment 1, and the cross-segment jump-table entry.

`root.c` deliberately calls the segment-1 function.  The result should force
the linker to include the default root-resident `.segload` module from `c.lib`.
The command file uses the explicit `aztec:lib/c.lib` path rather than Aztec's
`-lc` shorthand, which depends on an external `CLIB` environment setting.
