# Architecture Plan

## Objective
Build a feasibility-first DX12 prototype that proves, with metrics, where cross-adapter DLSS breaks or survives.

## Pipeline
1. Enumerate adapters and choose:
   - Primary renderer: AMD.
   - Secondary accelerator: NVIDIA RTX.
2. Render animated scene on AMD.
3. Generate DLSS-style inputs (color/depth/motion vectors + camera metadata).
4. Transfer resources to RTX through explicit cross-adapter path.
5. Measure transfer + sync cost.
6. (Optional phase) invoke Streamline/DLSS-SR on RTX.
7. Emit CSV/log outputs and classify outcome.

## Modules
- `AdapterEnumerator`: DXGI adapter discovery, capabilities, LUID reporting.
- `Renderer`: AMD-owned scene rendering and input buffer generation.
- `CrossAdapterBridge`: copy/sync orchestration and transfer diagnostics.
- `BenchmarkWriter`: CSV metrics output for all frame stages.
- `Diagnostics`: high-visibility capability and pipeline reporting.

## Data flow
AMD Render -> AMD fence signal -> Cross-adapter copy -> RTX queue wait -> DLSS stage (optional) -> result classification

## Instrumentation
- CPU wall-clock per stage.
- GPU stage estimates (prototype) and future timestamp query placeholders.
- Effective bandwidth estimate (GB/s).
- Pass/fail reason strings for each frame.
