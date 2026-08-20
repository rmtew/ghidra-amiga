# Aztec C multi-target segmented-code fixture

This generated Aztec C 5.0a executable is the companion to
`aztec-segload-smoke`.  It exercises two references from the resident root
node into one independently loadable overlay node:

```text
cc root.c
cc overlay.c
ln -o segload-multi +o0 root.o aztec:lib/c.lib +o1 overlay.o aztec:lib/c.lib
```

The repository contains only the two small sources and their generated object
and executable outputs.  It does not contain Aztec C media, compiler tools, or
runtime libraries.

The MANX overlay table has one node and one segment descriptor with two
trampolines.  Their targets are the two compiled overlay functions, at offsets
`0x000000` and `0x00001c`; the latter demonstrates that the loader must retain
the table's target offset rather than treating every overlay call as its entry
point.

SHA-256:

```text
7A443A5CFA77A21524714FB198ECB6DEF82C0B4445CB8E17608BA581E9D9D5B3  root.c
0BD9CA5FF8EEC9045D2B2EF5483870C78417A5D26C57066E25FFB139B4C69772  overlay.c
72383F426DDE202D76BC3209551B188C3321264041072974A39D7F6F61A4F3F0  root.o
E2CE2E5D5A1BBF049A216DAF826A374280EFF4B90CA8012CDBDF51F0C8DCF54F  overlay.o
6642D0652A15CE61FE171A34CCE27CFC9F7F1F3E911D22F6F0348D4C1B1D266D  segload-multi
```
