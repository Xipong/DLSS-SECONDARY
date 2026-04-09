#pragma once

#include "AdapterEnumerator.hpp"
#include "Common.hpp"

namespace dlss_secondary {

class Logger;

class Diagnostics {
public:
    explicit Diagnostics(Logger& logger);
    void PrintAdapterTable(const EnumeratedAdapters& adapters);
    void PrintPipelineExpectations();

private:
    Logger& logger_;
};

} // namespace dlss_secondary
