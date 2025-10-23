#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <urx/detail/double_nan.h>
#include <urx/utils/validator.h>

#include <uac/acquisition.h>
#include <uac/dataset.h>
#include <uac/destination_link.h>
#include <uac/enums.h>
#include <uac/event.h>
#include <uac/group.h>
#include <uac/igroup.h>
#include <uac/trigger.h>
#include <uac/utils/export.h>

#define UNUSED(x) (void)(x)

namespace uac::utils {

UAC_UTILS_EXPORT bool isIGroupPtrInTargetAcq(const std::weak_ptr<IGroup>& ptr,
                                             const Acquisition& acq);

template <typename Derived, typename Dataset>
class ValidatorBase : public urx::utils::ValidatorBase<Derived, Dataset> {
 public:
  ~ValidatorBase() override = default;

  void check(Dataset& dataset) override {
    urx::utils::ValidatorBase<Derived, Dataset>::check(dataset);

    for (size_t i = 0; i < dataset.acquisition.super_groups.size(); ++i) {
      if (dataset.acquisition.super_groups.at(i)) {
        check(*dataset.acquisition.super_groups.at(i), "SuperGroup " + std::to_string(i) + ": ");

        if (!isIGroupPtrInTargetAcq(dataset.acquisition.super_groups.at(i)->initial_group,
                                    dataset.acquisition)) {
          static_cast<Derived*>(this)->wrongPointer(
              dataset.acquisition.super_groups.at(i)->initial_group,
              "SuperGroup " + std::to_string(i) + ": Wrong initial group pointer.");
        }
        check(dataset.acquisition.super_groups.at(i)->destinations, dataset.acquisition,
              "SuperGroup " + std::to_string(i) + ": ");
      } else {
        static_cast<Derived*>(this)->nullPointer(
            dataset.acquisition.super_groups.at(i),
            "SuperGroup " + std::to_string(i) + ": SuperGroup pointer is null.");
      }
    }

    for (size_t i = 0; i < dataset.acquisition.groups.size(); ++i) {
      if (dataset.acquisition.groups.at(i)) {
        // Do not ask to check the group
        // It's already done in urx::utils::ValidatorBase<Derived, Dataset>::check
        check(dataset.acquisition.groups.at(i)->destinations, dataset.acquisition,
              "Group " + std::to_string(i) + ": ");
      } else {
        // Do not emit nullPointer error
        // It's already done in urx::utils::ValidatorBase<Derived, Dataset>::check
      }
    }
  }

  void check(decltype(IGroup::destinations)& destinations, decltype(Dataset::acquisition)& acq,
             const std::string& identifier_message_prefix = "") {
    for (size_t i = 0; i < destinations.size(); ++i) {
      if (!isIGroupPtrInTargetAcq(destinations.at(i).destination, acq)) {
        static_cast<Derived*>(this)->wrongPointer(
            destinations.at(i).destination, identifier_message_prefix + "DestinationLink " +
                                                std::to_string(i) + ": Wrong destination pointer.");
      }
      check(destinations.at(i).trigger,
            identifier_message_prefix + "DestinationLink " + std::to_string(i) + ": ");
    }
  }

  void check(decltype(Dataset::acquisition)& acq) override {
    urx::utils::ValidatorBase<Derived, Dataset>::check(acq);

    if (!isIGroupPtrInTargetAcq(acq.initial_group, acq)) {
      static_cast<Derived*>(this)->wrongPointer(
          acq.initial_group, "Acquisition initial group: Wrong initial group pointer.");
    }

    check(acq.trigger_in, "Acquisition TriggerIn :");
    check(acq.trigger_out, "Acquisition TriggerOut :");

    if (std::isnan(acq.time_offset)) {
      static_cast<Derived*>(this)->valueNotSet(acq.time_offset,
                                               "Acquisition Time offset: Value is not set.");
    }

    if (acq.time_offset < 0) {
      static_cast<Derived*>(this)->valueNotPositive(
          acq.time_offset,
          "Acquisition Time offset: Value is not set correctly. Value must be positive or zero.");
    }
  }

  void check(Group& group, const std::string& identifier_message_prefix = "") override {
    urx::utils::ValidatorBase<Derived, Dataset>::check(group, identifier_message_prefix);
    check(*static_cast<IGroup*>(&group), identifier_message_prefix);
  }

  void check(std::optional<TriggerIn>& trigger, const std::string& identifier_message_prefix = "") {
    if (trigger.has_value() && trigger->edge == uac::Edge::UNDEFINED) {
      static_cast<Derived*>(this)->valueNotSet(
          trigger->edge, identifier_message_prefix + "TriggerIn Edge not set.");
    }
  }

  void check(std::optional<TriggerOut>& trigger,
             const std::string& identifier_message_prefix = "") {
    if (trigger.has_value()) {
      if (trigger->polarity == uac::Polarity::UNDEFINED) {
        static_cast<Derived*>(this)->valueNotSet(
            trigger->polarity, identifier_message_prefix + "TriggerOut Polarity not set.");
      }

      if (std::isnan(trigger->time_offset)) {
        static_cast<Derived*>(this)->valueNotSet(trigger->time_offset,
                                                 "Time offset: Value is not set.");
      }

      if (trigger->time_offset < 0) {
        static_cast<Derived*>(this)->valueNotPositive(
            trigger->time_offset,
            "Time offset: Value is not set correctly. Value must be positive or zero.");
      }

      if (std::isnan(trigger->pulse_duration)) {
        static_cast<Derived*>(this)->valueNotSet(trigger->pulse_duration,
                                                 "Pulse duration: Value is not set.");
      }

      if (trigger->pulse_duration < 0) {
        static_cast<Derived*>(this)->valueNotPositive(
            trigger->pulse_duration,
            "Pulse duration: Value is not set correctly. Value must be positive or zero.");
      }
    }
  }

  void check(IGroup& igroup, const std::string& identifier_message_prefix = "") {
    check(igroup.trigger_in, identifier_message_prefix + "TriggerIn :");
    check(igroup.trigger_out, identifier_message_prefix + "TriggerOut :");

    if (std::isnan(igroup.time_offset)) {
      static_cast<Derived*>(this)->valueNotSet(igroup.time_offset,
                                               "Time offset: Value is not set.");
    }

    if (igroup.time_offset < 0) {
      static_cast<Derived*>(this)->valueNotPositive(
          igroup.time_offset,
          "Time offset: Value is not set correctly. Value must be positive or zero.");
    }

    if (std::isnan(igroup.period)) {
      static_cast<Derived*>(this)->valueNotSet(igroup.period, "Period: Value is not set.");
    }

    if (igroup.period < 0) {
      static_cast<Derived*>(this)->valueNotPositive(
          igroup.period, "Period: Value is not set correctly. Value must be positive or zero.");
    }
  }

  void check(Event& event, const SamplingType& type,
             const std::string& identifier_message_prefix = "") override {
    urx::utils::ValidatorBase<Derived, Dataset>::check(event, type, identifier_message_prefix);

    check(event.trigger_in, identifier_message_prefix + "TriggerIn :");
    check(event.trigger_out, identifier_message_prefix + "TriggerOut :");

    if (std::isnan(event.time_offset)) {
      static_cast<Derived*>(this)->valueNotSet(event.time_offset, "Time offset: Value is not set.");
    }

    if (event.time_offset < 0) {
      static_cast<Derived*>(this)->valueNotPositive(
          event.time_offset,
          "Time offset: Value is not set correctly. Value must be positive or zero.");
    }
  }
};

using ValidatorReport = urx::utils::ValidatorReportBase<ValidatorBase, Dataset>;

}  // namespace uac::utils
