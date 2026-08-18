# Lattice C 4.00 BLINK overlay fixture

This is an authentic BLINK 7.2 overlay output built from the version-local
sources and the release's own `c.o` and `lc.lib`. It uses BLINK 7.2's
interleaved-root ordering: `HUNK_OVERLAY` appears before the remaining root
Hunks, while the actual overlay node later has its normal `HUNK_HEADER` and
`HUNK_BREAK`. The loader preserves that distinction and resolves the node
through the table's recorded file position.

The checked executable SHA-256 is:

```text
D65F6A40336F8105F9DEF9017A613D4685BEBD76A05E07C49041A6392EC3FB69
```
