# Aztec C sample builds

Set these common mounts once per sample directory:

```powershell
$aztec = (Resolve-Path toolchains/aztec-c-5.0a/volume).Path.Replace('\','/')
$vamos = 'vamos'
```

Each command below is run with `-V "aztec:$aztec" -V "work:$sample" -p
aztec:bin --cwd work:`. Use `--` before linker options.

| Sample | Build |
| --- | --- |
| `runtime-a4-plain` | `cc root.c`; `ln -- -o manx-runtime-a4 root.o aztec:lib/c.lib` |
| `relocation-base-minus-two` | `as base-minus-two.s`; `ln -- -o base-minus-two base-minus-two.o` |
| `segload-smoke` | `cc root.c`; `cc overlay.c`; `ln -- -o segload-smoke -f segload-smoke.lnk` |
| `segload-multi` | `cc root.c`; `cc overlay.c`; `ln -- -o segload-multi -f segload-multi.lnk` |
| `runtime-helpers` | `cc root.c`; `ln -- -o manx-runtime-helpers root.o aztec:lib/c.lib` |

The two SegLoad command files use explicit `aztec:lib/c.lib` paths. This
avoids the legacy `-lc` shorthand's implicit `CLIB` environment requirement.

Verified SHA-256 outputs:

```text
runtime-a4-plain/manx-runtime-a4
7808A634BBEB21B0C95088C0BC15806F860E582AF94412B8CDC2D60D909EFC76

relocation-base-minus-two/base-minus-two
E8A652CC98CFBCE318FE9F15ED3F567C7016773BDA147E0D7878D080F12B2E4F

segload-smoke/segload-smoke
D0A3F881D411C8AEA1B59F94DA329DAE01681D075A5D041868ED020920D6EF70

segload-multi/segload-multi
6642D0652A15CE61FE171A34CCE27CFC9F7F1F3E911D22F6F0348D4C1B1D266D

runtime-helpers/manx-runtime-helpers
2626F0CE86739A34A3CED7BFFB687FA46160F6720C39262E281F07C51EC32470
```
