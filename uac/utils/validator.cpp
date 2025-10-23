#include <algorithm>
#include <memory>
#include <optional>

#include <urx/utils/common.h>

#include <uac/acquisition.h>
#include <uac/group.h>
#include <uac/igroup.h>
#include <uac/super_group.h>
#include <uac/utils/validator.h>

namespace uac::utils {

bool isIGroupPtrInTargetAcq(const std::weak_ptr<IGroup>& ptr, const Acquisition& acq) {
  if (!ptr.expired()) {
    if (const std::shared_ptr<uac::SuperGroup> sg_ptr =
            std::dynamic_pointer_cast<uac::SuperGroup>(ptr.lock())) {
      const std::optional<size_t> sg_id =
          urx::utils::common::getElementIndex(acq.super_groups, sg_ptr, false);
      if (sg_id.has_value()) {
        return true;
      }
    } else if (const std::shared_ptr<uac::Group> g_ptr =
                   std::dynamic_pointer_cast<uac::Group>(ptr.lock())) {
      const std::optional<size_t> g_id =
          urx::utils::common::getElementIndex(acq.groups, g_ptr, false);
      if (g_id.has_value()) {
        return true;
      }
    }
  }
  return false;
}

}  // namespace uac::utils
