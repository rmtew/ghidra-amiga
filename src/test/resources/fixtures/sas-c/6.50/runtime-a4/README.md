# SAS/C 6.50 runtime A4 fixture

This source-complete fixture is an authentic SAS/C 6.50 `sc`/`slink` output.
It links the standard `c.o` runtime with one A4-relative global and is kept to
verify the compiler's startup convention, independently of overlays.

The runtime establishes A4 with `lea LinkerDB,a4`; `LinkerDB` is supplied by
SLink. `read_global()` accesses `global_counter`, so the linked output also
contains a real compiler-generated A4-relative data access.

Build provenance (the compiler and libraries are not included):

```text
sc root.c
sc global.c
slink WITH withfile
```

SHA-256:

```text
588C59A079D950703F3051C9879110BE19A7189E830D0AEB1B58D0E877708318  root.o
E71986882921D9B94BE4DB530B19DBF01EC561AE2AA9CDFF66FAF14BB723648F  global.o
144A07ED62BE1641AE72EB68EF8723419B646F7F17307EB4A66BBA62362EB103  sas-runtime-a4
```
