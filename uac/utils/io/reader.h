#pragma once

#include <memory>
#include <string>

#include <uac/dataset.h>
#include <uac/utils/export.h>

namespace uac::utils::io::reader {

UAC_UTILS_EXPORT std::shared_ptr<Dataset> loadFromFile(const std::string& filename);

}  // namespace uac::utils::io::reader
