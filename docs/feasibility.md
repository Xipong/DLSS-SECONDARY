# Feasibility: AMD Primary Rendering + NVIDIA Secondary DLSS (Windows 11, DX12)

## Verdict (short)
- **Theoretically possible** for DLSS Super Resolution in a **custom explicit multi-adapter engine**.
- **Practically fragile and likely too costly** due to inter-adapter transfer and synchronization overhead.
- **Not viable as a generic wrapper for arbitrary shipped games** under legal/engineering constraints.
- **DLSS Frame Generation (FG) is out of scope** for this prototype and likely not feasible in split-adapter mode.

## Can DLSS-SR run on a secondary RTX while frames are rendered on AMD?
Potentially yes, but only if all required DLSS inputs are produced and transferred into resources that are valid and synchronized on the RTX adapter:
1. Low-resolution color buffer.
2. Depth buffer.
3. Motion vectors (with correct space conventions).
4. Exposure / reactive masks (depending on integration path).
5. Camera matrices, jitter offsets, frame index, and reset flags.

A custom DX12 app can architect this pipeline explicitly with dual devices, shared heaps/resources (where supported), and cross-adapter fences.

## Can DLSS-FG be supported?
Treat as **not feasible for this phase**.
Reasons:
- FG has stricter timing and history constraints.
- Requires precise frame pacing and optical-flow related integration details.
- Additional data dependencies make cross-adapter synchronization riskier and latency-sensitive.

## Required transfers across adapters
Per frame (minimum):
- Color (typically FP16 or similar HDR format).
- Depth (D32 or equivalent).
- Motion vectors (RG16F or equivalent).
- Optional masks / exposure / transparency tags.

At 2560x1440, nominal payload can exceed ~60 MB/frame depending on formats and extra buffers.

## Synchronization requirements
- Source queue fence signal after all producer passes complete on AMD.
- Cross-adapter copy queue wait + copy + signal.
- RTX compute/graphics queue wait before DLSS dispatch.
- Optional return copy of final upscaled output back to AMD/present path if presentation remains on AMD.

Each stage introduces latency and can add jitter when queues desynchronize.

## Least-impossible approach ranking
1. **A) Custom sample app with explicit multi-adapter** ✅ (best chance).
2. **B) DX12 proxy/injection into controlled sample** ⚠️ (hard, brittle, but diagnosable).
3. **C) Generic game-agnostic wrapper** ❌ (architecturally blocked in practice).

## Hard blockers
- Lack of guaranteed cross-vendor shared-resource behavior across all systems.
- DLSS integration assumptions around owning the render graph and timing.
- Round-trip copy penalties if present queue is on AMD.
- Fragile frame pacing under variable PCIe contention.

## Expected penalties
- **Bandwidth:** tens of MB/frame minimum payload.
- **Latency:** likely several ms/frame copy + sync, increasing at higher resolutions.
- **Frame pacing:** high risk of periodic stutter when copy queue backlog occurs.
