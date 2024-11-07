#pragma once

#include <string>

#include <uac/dataset.h>
#include <uac/utils/export.h>

namespace uac::utils::io::writer {

UAC_UTILS_EXPORT void saveToFile(const std::string& filename, const Dataset& dataset);

}  // namespace uac::utils::io::writer
