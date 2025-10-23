#include "clone.h"

#include <uac/dataset.h>
#include <uac/utils/detail/clone.h>

namespace uac::utils {

Dataset clone(const Dataset& value) { return detail::clone<Dataset>(value); }

std::shared_ptr<Dataset> clone(const std::shared_ptr<Dataset>& value) {
  return detail::clone<std::shared_ptr<Dataset>>(value);
}

}  // namespace uac::utils
