# SAS/C 6.50 generated fixtures

These fixtures were built with the original SAS/C 6.50 Amiga tools: `sc`,
`asm`, and `slink`. They are intentionally small, source-complete examples of
the linker output consumed by this extension; no compiler binaries, SDK files,
or disk images are included.

Build each fixture by compiling its `*.c` files with `sc`, placing
`manager/ovs.o` beside its `withfile`, then running:

```text
slink WITH withfile
```

`basic-overlay` covers one root-to-overlay target, `multi-target-overlay`
covers two targets in one overlay node, and `nested-overlay` uses SLink's `*`
child-node marker to cover a root → child → grandchild hierarchy. The manager
directory retains the original supervisor source and the exact compatibility
variant assembled for SLink 6.50; see its README for the two intentional symbol
spelling changes.
