# Binary fixture layout

Generated compiler/linker fixtures are organised by producer and exact version:

```text
<producer>/<version>/<fixture>/
```

Keep the source inputs, generated objects, linked output, build provenance,
and checksums together in that directory.  Do not replace a fixture when a
different compiler or linker version produces a variant; add the new version
alongside it so parser and analyzer compatibility remain independently
testable.

Fixtures copied from a third-party distribution use a producer-specific
directory and must document their provenance and checksum.  Do not add media,
toolchains, ROMs, or full SDKs/disks as fixtures.
