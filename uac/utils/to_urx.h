#pragma once

#include <memory>

#include <urx/dataset.h>

#include <uac/dataset.h>
#include <uac/utils/export.h>

namespace uac::utils {

UAC_UTILS_EXPORT std::shared_ptr<urx::Dataset> toUrx(const uac::Dataset& dataset);

}  // namespace uac::utils
