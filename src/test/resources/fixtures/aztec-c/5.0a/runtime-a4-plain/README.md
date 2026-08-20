# Aztec C 5.0a plain runtime A4 fixture

This source-complete fixture is a non-overlay Aztec C 5.0a executable. It
links the normal MANX C runtime and accesses one compiler global, providing a
real regression case for A4 recovery without a `HUNK_OVERLAY` table.

Build provenance (the compiler and C library are not included):

```text
cc root.c
ln -o manx-runtime-a4 root.o aztec:lib/c.lib
```

SHA-256:

```text
AC62B7922DA9887E89EC25FB6D896B8295768B9F01AA95AAD5FDF91C6D53F141  root.o
7808A634BBEB21B0C95088C0BC15806F860E582AF94412B8CDC2D60D909EFC76  manx-runtime-a4
```
