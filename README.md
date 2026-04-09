# DLSS-SECONDARY Research Prototype

This repository contains a **feasibility-first** prototype for a Windows 11 DX12 design where:
- AMD GPU is the primary renderer.
- NVIDIA RTX GPU is a secondary accelerator candidate for DLSS-SR.

The goal is to produce an **honest feasibility answer**, not a fake compatibility claim.

## Status Summary
- **Theoretical:** possible in a tightly controlled custom engine/sample.
- **Practical:** likely constrained by cross-adapter copy + synchronization overhead.
- **Generic game-agnostic wrapper:** currently considered architecturally blocked.

## Build (Windows 11 x64 expected)
```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

## Run
```powershell
./build/Release/dlss_secondary.exe --frames 240
./build/Release/dlss_secondary.exe --frames 240 --no-cross-adapter
```

## Outputs
- `bench/run.log` - verbose diagnostics.
- `bench/metrics.csv` - per-frame timing and transfer metrics.

## Prototype CLI
- `--frames <N>`: number of frames to run.
- `--no-cross-adapter`: disables secondary-adapter transfer path.

## Current limitations
- Streamline/DLSS API is not yet linked; this stage currently validates pipeline preconditions and cost.
- DX12 calls are scaffolded and must be validated on real AMD+RTX hardware on Windows 11.
- Frame Generation is intentionally out of scope for initial feasibility.

## Deliverables
- `docs/feasibility.md`
- `docs/architecture.md`
- `docs/findings.md`
- `src/` sample source
- `bench/` benchmark artifacts (generated at runtime)
