#pragma once

#include <memory>
#include <string>

#include <urx/utils/io/reader_options.h>

#include <uac/dataset.h>
#include <uac/utils/export.h>

namespace uac::utils::io::reader {

UAC_UTILS_EXPORT std::shared_ptr<Dataset> loadFromFile(
    const std::string& filename, const urx::utils::io::ReaderOptions& options = {});

}  // namespace uac::utils::io::reader
