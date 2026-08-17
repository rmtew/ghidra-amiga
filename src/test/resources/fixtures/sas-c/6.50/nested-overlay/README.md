# SAS/C 6.50 nested overlay fixture

This real SAS/C 6.50 `sc`/`slink` executable exercises a two-level overlay
tree. The resident root calls `middle_value()` in the first overlay node; that
function calls `leaf_value()` in its child node.

In SLink's control-file syntax, the `*` on `*leaf.o` starts a child overlay
node. The following `#` closes the child and the final `#` closes its parent.
This compact case verifies that a loader preserves the hierarchy rather than
merely finding multiple overlay headers.

The shared [SAS/C overlay manager](../manager/README.md) supplies the original
source, SLink-6.50 compatibility source, and assembled `ovs.o` input. To
reproduce this link, stage `ovs.o` next to `withfile` before invoking SLink.

```text
2F8E26212470C8A1DAD88FE6A116281765B6C0D6C0A1AF6A010001D11AD1380F  root.o
9DA454C1839DA495B125B0F20A94B7B636DDCA366C1C7A6C31B16C90AC7F7C56  middle.o
E96C5CE9B90000049C1D19288F1D88A0E96B56C6B7B55ABA7122B6B0A72A5653  leaf.o
8820531B02D4400E22A142328EC232C793119A5605CE3C21B18414C34D59C7F4  sas-nested
```
