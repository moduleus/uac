#pragma once

#include <memory>

#include <uac/dataset.h>
#include <uac/utils/export.h>

namespace uac::utils {

UAC_UTILS_EXPORT Dataset clone(const Dataset& value);
UAC_UTILS_EXPORT std::shared_ptr<Dataset> clone(const std::shared_ptr<Dataset>& value);

}  // namespace uac::utils
