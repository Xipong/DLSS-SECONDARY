# Findings (Initial Prototype Status)

## Scope completed
- Implemented a minimal C++20/CMake prototype with explicit modules for adapter enumeration, rendering packet generation, cross-adapter transfer simulation, and benchmark logging.
- Added feasibility-first docs and no-false-claims guardrails.

## Measurement snapshot (synthetic CI run)
Using 2560x1440 payload assumptions over 60 frames:
- Baseline path (`--no-cross-adapter`):
  - Mean CPU end-to-end: **1.92 ms**.
  - P95 CPU end-to-end: **1.99 ms**.
- Cross-adapter path:
  - Mean CPU end-to-end: **6.12 ms**.
  - P95 CPU end-to-end: **6.16 ms**.
  - Mean CPU copy segment: **4.22 ms**.

Interpretation: even with optimistic assumptions, transfer+sync cost is a dominant budget item and can threaten frame pacing at high refresh targets.

## What currently works
- Detects/prints AMD and NVIDIA adapter identities (or synthetic stand-ins on non-Windows environments).
- Produces per-frame DLSS-style input payload metadata.
- Exercises a cross-adapter transfer path model and logs timing/bandwidth estimates.
- Emits CSV/log artifacts for baseline and cross-adapter comparisons:
  - `bench/metrics_cross_adapter.csv`
  - `bench/metrics_baseline.csv`

## What almost works
- DX12-specific code path is scaffolded but still simplified; true resource sharing, queue ownership transitions, and fence import/export require Windows runtime validation on target hardware.

## What fails / blocked
- No real Streamline/DLSS invocation yet.
- No proof yet that cross-vendor cross-adapter resource path is stable on target machines.
- No Frame Generation attempt due to stronger architectural/timing constraints.

## Honest interim classification
- **Current status:** "Technically plausible in a custom sample, but early evidence suggests it may be too slow/fragile for practical use."
- **Generic utility feasibility:** "No-go" pending fundamental integration blockers and overhead.
