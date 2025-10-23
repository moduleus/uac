#pragma once

#include <algorithm>
#include <cstring>
#include <memory>
#include <optional>
#include <vector>

#include <urx/probe.h>
#include <urx/utils/common.h>
#include <urx/utils/detail/clone.h>

#include <uac/acquisition.h>
#include <uac/dataset.h>
#include <uac/destination_link.h>
#include <uac/event.h>
#include <uac/excitation.h>
#include <uac/group.h>
#include <uac/igroup.h>
#include <uac/probe.h>
#include <uac/receive_setup.h>
#include <uac/super_group.h>
#include <uac/transmit_setup.h>

namespace uac::utils::detail {
template <typename T, typename U = T>
inline U clone(const T& value) {
  return urx::utils::detail::clone(value);
}
}  // namespace uac::utils::detail
namespace urx::utils::detail {

inline void assignNewIGroupPtr(const uac::Acquisition& acq, const uac::Acquisition& acq_cloned,
                               std::weak_ptr<uac::IGroup>& ptr) {
  if (!ptr.expired()) {
    if (const std::shared_ptr<uac::SuperGroup> sg_ptr =
            std::dynamic_pointer_cast<uac::SuperGroup>(ptr.lock())) {
      const std::optional<size_t> sg_id =
          urx::utils::common::getElementIndex(acq.super_groups, sg_ptr, false);
      if (sg_id.has_value()) {
        ptr = acq.super_groups.at(*sg_id);
      } else {
        ptr = std::weak_ptr<uac::IGroup>();
      }
    } else if (const std::shared_ptr<uac::Group> g_ptr =
                   std::dynamic_pointer_cast<uac::Group>(ptr.lock())) {
      const std::optional<size_t> g_id =
          urx::utils::common::getElementIndex(acq.groups, g_ptr, false);
      if (g_id.has_value()) {
        ptr = acq.groups.at(*g_id);
      } else {
        ptr = std::weak_ptr<uac::IGroup>();
      }
    } else {
      ptr = std::weak_ptr<uac::IGroup>();
    }
  }
}

template <>
inline uac::Acquisition clone(const uac::Acquisition& value) {
  uac::Acquisition acq_cloned(value);

  for (size_t p_id = 0; p_id < value.probes.size(); ++p_id) {
    acq_cloned.probes[p_id] = clone(value.probes.at(p_id));
  }
  for (size_t ex_id = 0; ex_id < value.excitations.size(); ++ex_id) {
    acq_cloned.excitations[ex_id] = clone(value.excitations.at(ex_id));
  }
  for (size_t g_id = 0; g_id < value.groups.size(); ++g_id) {
    acq_cloned.groups[g_id] = clone(value.groups.at(g_id));
  }
  for (size_t sg_id = 0; sg_id < value.super_groups.size(); ++sg_id) {
    acq_cloned.super_groups[sg_id] = clone(value.super_groups.at(sg_id));
  }

  assignNewIGroupPtr(value, acq_cloned, acq_cloned.initial_group);

  for (auto& group_cloned : acq_cloned.groups) {
    if (group_cloned) {
      for (auto& event_cloned : group_cloned->sequence) {
        auto& receive_setup_cloned = event_cloned.receive_setup;
        auto& transmit_setup_cloned = event_cloned.transmit_setup;

        std::optional<size_t> p_id =
            common::getElementIndex(value.probes, receive_setup_cloned.probe, true);
        if (!p_id.has_value()) {
          receive_setup_cloned.probe = std::weak_ptr<uac::Probe>();
        } else {
          receive_setup_cloned.probe = acq_cloned.probes.at(*p_id);
        }

        p_id = common::getElementIndex(value.probes, transmit_setup_cloned.probe, true);
        if (!p_id.has_value()) {
          transmit_setup_cloned.probe = std::weak_ptr<uac::Probe>();
        } else {
          transmit_setup_cloned.probe = acq_cloned.probes.at(*p_id);
        }
        for (auto& excitation_cloned : transmit_setup_cloned.excitations) {
          std::optional<size_t> const acq_ex_id =
              common::getElementIndex(value.excitations, excitation_cloned, true);
          if (!acq_ex_id.has_value()) {
            excitation_cloned = std::weak_ptr<uac::Excitation>();
          } else {
            excitation_cloned = acq_cloned.excitations.at(*acq_ex_id);
          }
        }
      }

      for (auto& destination_cloned : group_cloned->destinations) {
        assignNewIGroupPtr(value, acq_cloned, destination_cloned.destination);
      }
    }
  }

  for (auto& supergroup_cloned : acq_cloned.super_groups) {
    if (supergroup_cloned) {
      for (auto& destination_cloned : supergroup_cloned->destinations) {
        assignNewIGroupPtr(value, acq_cloned, destination_cloned.destination);
      }
      assignNewIGroupPtr(value, acq_cloned, supergroup_cloned->initial_group);
    }
  }

  return acq_cloned;
}

template <>
inline uac::Dataset clone(const uac::Dataset& value) {
  uac::Dataset dataset_cloned(value);
  dataset_cloned.acquisition = clone(value.acquisition);

  return dataset_cloned;
}

}  // namespace urx::utils::detail
