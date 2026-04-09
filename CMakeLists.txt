cmake_minimum_required(VERSION 3.24)
project(DLSSSecondaryPrototype VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(DLSS_SECONDARY_ENABLE_CROSS_ADAPTER "Enable cross-adapter transfer path" ON)
option(DLSS_SECONDARY_ENABLE_VULKAN_EXPERIMENT "Enable optional Vulkan experiment stubs" OFF)

add_executable(dlss_secondary
    src/main.cpp
    src/App.cpp
    src/Logger.cpp
    src/AdapterEnumerator.cpp
    src/Renderer.cpp
    src/CrossAdapterBridge.cpp
    src/Benchmark.cpp
    src/Diagnostics.cpp
)

target_include_directories(dlss_secondary PRIVATE include)

target_compile_definitions(dlss_secondary PRIVATE
    $<$<BOOL:${DLSS_SECONDARY_ENABLE_CROSS_ADAPTER}>:DLSS_SECONDARY_ENABLE_CROSS_ADAPTER=1>
    $<$<NOT:$<BOOL:${DLSS_SECONDARY_ENABLE_CROSS_ADAPTER}>>:DLSS_SECONDARY_ENABLE_CROSS_ADAPTER=0>
)

if(WIN32)
    target_compile_definitions(dlss_secondary PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
    target_link_libraries(dlss_secondary PRIVATE d3d12 dxgi dxguid)
endif()

install(TARGETS dlss_secondary RUNTIME DESTINATION bin)
