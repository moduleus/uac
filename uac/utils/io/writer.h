#pragma once

#include <string>

#include <urx/utils/io/writer_options.h>

#include <uac/dataset.h>
#include <uac/utils/export.h>

namespace uac::utils::io::writer {

UAC_UTILS_EXPORT void saveToFile(const std::string& filename, const Dataset& dataset,
                                 const urx::utils::io::WriterOptions& options = {});

}  // namespace uac::utils::io::writer
