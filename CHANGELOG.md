# Changelog

All notable changes to this project are documented in this file.

## Unreleased

### Added

- Picasso96, MMULib, and OpenPCI function-definition files for Amiga API
  vector discovery.

### Fixed

- SFD parsing now accepts comment lines, normalizes library names, preserves
  pointer markers attached to parameter names, and tolerates trailing
  semicolons in FD prototypes.
- Resident-library SFD selection is safe for GUI and headless imports.

Thanks to **mheyer** (`sonode@gmx.de`) for the original commits that supplied
these definition files and fixes.

## 2026-08-18

### Added

- Missing NDK content, including the previously unavailable `IOAudio` type.
- HUNK overlay support, with Aztec/Manx 5.0 and SAS/C 6.5 fixture coverage.
- Aztec C 5.0 support:
  - MANX ABI support, including its normal 68000 return-address convention and
    two-byte stack argument slots.
  - Automatic A4 small-data anchor detection.
  - MANX overlay-table support.
  - Basic runtime/CRT recognition, including selected math helpers, `strlen`,
    and `main`.
- SAS/C 6.5 automatic A4 detection.
- Address-taken code discovery for pointers stored to memory when the target
  decodes to code with a reachable return.
- Device ABI dispatch recognition and improved Amiga call typing.

### Changed

- Initial analysis now performs full library-call detection instead of being
  limited to two libraries.
- Wrapper functions are named automatically.

### Fixed

- Intuition-library call parsing no longer fails on an unexpected tab in a
  `RastPort *` type lookup, allowing later library calls to be analysed.
