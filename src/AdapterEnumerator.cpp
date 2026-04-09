#include "AdapterEnumerator.hpp"
#include "Logger.hpp"

#ifdef _WIN32
#include <Windows.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <codecvt>
#include <locale>
#endif

namespace dlss_secondary {

AdapterEnumerator::AdapterEnumerator(Logger& logger) : logger_(logger) {}

EnumeratedAdapters AdapterEnumerator::Enumerate() {
    EnumeratedAdapters result;

#ifdef _WIN32
    Microsoft::WRL::ComPtr<IDXGIFactory6> factory;
    const HRESULT factoryHr = CreateDXGIFactory2(0, IID_PPV_ARGS(&factory));
    if (FAILED(factoryHr)) {
        logger_.Error("CreateDXGIFactory2 failed; cannot enumerate adapters.");
        return result;
    }

    for (UINT index = 0;; ++index) {
        Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
        if (factory->EnumAdapters1(index, &adapter) == DXGI_ERROR_NOT_FOUND) {
            break;
        }

        DXGI_ADAPTER_DESC1 desc {};
        adapter->GetDesc1(&desc);

        AdapterInfo info;
        info.name = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(desc.Description);
        info.dedicatedVideoMemory = desc.DedicatedVideoMemory;
        info.sharedSystemMemory = desc.SharedSystemMemory;
        info.vendorId = desc.VendorId;
        info.deviceId = desc.DeviceId;
        info.luidLowPart = desc.AdapterLuid.LowPart;
        info.luidHighPart = desc.AdapterLuid.HighPart;
        info.isSoftware = (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0;
        info.supportsD3D12 = !info.isSoftware;
        info.supportsCrossAdapterHeap = !info.isSoftware;

        result.all.push_back(info);

        if (!result.amdPrimary && info.vendorId == 0x1002 && !info.isSoftware) {
            result.amdPrimary = info;
        }

        if (!result.nvidiaSecondary && info.vendorId == 0x10DE && !info.isSoftware) {
            result.nvidiaSecondary = info;
        }
    }
#else
    logger_.Warn("Running on non-Windows host; using synthetic adapter list for documentation-only execution.");

    AdapterInfo amd;
    amd.name = "Synthetic AMD Adapter (for CI)";
    amd.vendorId = 0x1002;
    amd.deviceId = 0xFFFF;
    amd.dedicatedVideoMemory = 8ull * 1024 * 1024 * 1024;
    amd.sharedSystemMemory = 16ull * 1024 * 1024 * 1024;
    amd.luidLowPart = 1;
    amd.luidHighPart = 0;
    amd.supportsD3D12 = true;
    amd.supportsCrossAdapterHeap = true;

    AdapterInfo nvidia;
    nvidia.name = "Synthetic NVIDIA RTX Adapter (for CI)";
    nvidia.vendorId = 0x10DE;
    nvidia.deviceId = 0xFFFF;
    nvidia.dedicatedVideoMemory = 12ull * 1024 * 1024 * 1024;
    nvidia.sharedSystemMemory = 16ull * 1024 * 1024 * 1024;
    nvidia.luidLowPart = 2;
    nvidia.luidHighPart = 0;
    nvidia.supportsD3D12 = true;
    nvidia.supportsCrossAdapterHeap = true;

    result.all = {amd, nvidia};
    result.amdPrimary = amd;
    result.nvidiaSecondary = nvidia;
#endif

    return result;
}

} // namespace dlss_secondary
