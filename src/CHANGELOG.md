# Changelog

## 2026-07-13

### Added
- Two new built-in example feeders in the example picker: tap-changer +
  capacitor control, enabled and disabled (`exTapCapOn` / `exTapCapOff`).
- Prebuilt `dss_capi` 0.14.5 binaries (macOS arm64/x64, Windows x64),
  published as a GitHub release with setup instructions in `README.md`, so
  building no longer requires compiling `dss_capi` yourself.

### Fixed
- Crash when picking an example from the example picker. The picker closed
  its own containing window from inside its own timer's fire callback,
  destroying that timer mid-dispatch; closing is now done by a timer owned
  by the long-lived parent view instead.
