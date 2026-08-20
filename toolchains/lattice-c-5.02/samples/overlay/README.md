# Lattice C 5.02 BLINK overlay fixture

`lattice-overlay.lnk` is a BLINK `WITH` file: it puts `root.o` and the CRT in
the resident root, then declares `overlay.o` as one level-1 overlay node.
The emitted Hunk file has a root node and one standard `HUNK_OVERLAY` node;
the extension test checks its A4 context across both.

The generated `lattice-overlay` SHA-256 is:

```text
25221AED76449454B5E0C3A5D46FE0FF872084603C38994332A64C1F75ECB090
```

Run `Get-FileHash -Algorithm SHA256 lattice-overlay` to verify this value
after rebuilding.  `root.o` and `overlay.o` are retained build evidence;
quad files are intentionally omitted.
