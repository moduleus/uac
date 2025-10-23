#pragma once

#include <algorithm>
#include <iterator>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include <H5Cpp.h>

#include <urx/probe.h>
#include <urx/utils/cpp.h>
#include <urx/utils/io/writer_impl.h>

#include <uac/dataset.h>
#include <uac/excitation.h>
#include <uac/group.h>
#include <uac/hw_config.h>
#include <uac/igroup.h>
#include <uac/probe.h>
#include <uac/super_group.h>
#include <uac/utils/serialize_helper.h>

namespace uac::utils::io {

template <typename Dataset, typename AllTypeInVariant, typename Derived>
class WriterBase
    : public urx::utils::io::WriterBase<Dataset, AllTypeInVariant,
                                        WriterBase<Dataset, AllTypeInVariant, Derived>> {
 public:
  WriterBase()
      : urx::utils::io::WriterBase<Dataset, AllTypeInVariant,
                                   WriterBase<Dataset, AllTypeInVariant, Derived>>() {
    if constexpr (std::is_same_v<Dataset, uac::Dataset>) {
      this->_data_field = uac::utils::getMemberMap();
    }
  }

  using urx::utils::io::WriterBase<Dataset, AllTypeInVariant,
                                   WriterBase<Dataset, AllTypeInVariant, Derived>>::serializeAll;

  void init(const Dataset& dataset) {
    this->_map_to_shared_ptr[nameTypeid<Group>()] = &dataset.acquisition.groups;
    this->_map_to_shared_ptr[nameTypeid<Probe>()] = &dataset.acquisition.probes;
    this->_map_to_shared_ptr[nameTypeid<Excitation>()] = &dataset.acquisition.excitations;
    this->_map_to_shared_ptr[nameTypeid<SuperGroup>()] = &dataset.acquisition.super_groups;
  }

  template <typename T>
  typename std::enable_if_t<urx::utils::TypeContainer<T>::VALUE == urx::utils::ContainerType::RAW>
  serializeHdf5(const std::string& name, const T& field, const H5::Group& group) {
    if constexpr (std::is_same_v<T, uac::HwConfig>) {
      const H5::Group group_child = urx::utils::io::details::createOrOpenGroup(group, name);

      for (const auto& i : field.values) {
        std::visit(
            [this, name_i = i.first, &group_child](const auto& j) {
              this->serializeHdf5<std::remove_const_t<std::remove_reference_t<decltype(j)>>>(
                  name_i, j, group_child);
            },
            i.second);
      }
    } else {
      urx::utils::io::WriterBase<
          Dataset, AllTypeInVariant,
          WriterBase<Dataset, AllTypeInVariant, Derived>>::template serializeHdf5<T>(name, field,
                                                                                     group);
    }
  }

  template <typename T>
  typename std::enable_if_t<urx::utils::TypeContainer<T>::VALUE ==
                            urx::utils::ContainerType::SHARED_PTR>
  serializeHdf5(const std::string& name, const T& field, const H5::Group& group) {
    urx::utils::io::WriterBase<
        Dataset, AllTypeInVariant,
        WriterBase<Dataset, AllTypeInVariant, Derived>>::template serializeHdf5<T>(name, field,
                                                                                   group);
  }

  template <typename T>
  typename std::enable_if_t<urx::utils::TypeContainer<T>::VALUE ==
                            urx::utils::ContainerType::OPTIONAL>
  serializeHdf5(const std::string& name, const T& field, const H5::Group& group) {
    urx::utils::io::WriterBase<
        Dataset, AllTypeInVariant,
        WriterBase<Dataset, AllTypeInVariant, Derived>>::template serializeHdf5<T>(name, field,
                                                                                   group);
  }

  template <typename T>
  typename std::enable_if_t<urx::utils::TypeContainer<T>::VALUE ==
                            urx::utils::ContainerType::WEAK_PTR>
  serializeHdf5(const std::string& name, const T& field, const H5::Group& group) {
    if constexpr (std::is_same_v<T, std::weak_ptr<uac::IGroup>>) {
      // Never assigned
      if (!field.owner_before(std::weak_ptr<uac::IGroup>{}) &&
          !std::weak_ptr<uac::IGroup>{}.owner_before(field)) {
        return;
      }

      if (auto shared = field.lock()) {
        auto serialize_func = [this, &group, &name](const std::string& prefix, const auto& ptr) {
          if (!ptr) {
            return;
          }
          using T2 =
              typename std::remove_reference_t<std::remove_cv_t<decltype(ptr)>>::element_type;
          const auto& all_shared = urx::utils::getSharedPtr<T2>(this->_map_to_shared_ptr);
          auto idx = std::find_if(
              all_shared.begin(), all_shared.end(),
              [&ptr](const std::shared_ptr<T2>& data) { return ptr.get() == data.get(); });
          if (idx == all_shared.end()) {
            throw std::runtime_error(("Failed to found shared pointer assigned to weak pointer " +
                                      group.getObjName() + "/" + name)
                                         .c_str());
          }
          serializeHdf5<std::string>(
              name, prefix + std::to_string(std::distance(all_shared.begin(), idx)), group);
        };
        serialize_func("s", std::dynamic_pointer_cast<uac::SuperGroup>(shared));
        serialize_func("g", std::dynamic_pointer_cast<uac::Group>(shared));
      } else {
        throw std::runtime_error(
            ("Invalid weak field from " + group.getObjName() + "/" + name).c_str());
      }
    } else {
      urx::utils::io::WriterBase<
          Dataset, AllTypeInVariant,
          WriterBase<Dataset, AllTypeInVariant, Derived>>::template serializeHdf5<T>(name, field,
                                                                                     group);
    }
  }

  template <typename T>
  typename std::enable_if_t<urx::utils::TypeContainer<T>::VALUE ==
                            urx::utils::ContainerType::VECTOR>
  // NOLINTNEXTLINE(misc-no-recursion)
  serializeHdf5(const std::string& name, const T& field, const H5::Group& group) {
    urx::utils::io::WriterBase<
        Dataset, AllTypeInVariant,
        WriterBase<Dataset, AllTypeInVariant, Derived>>::template serializeHdf5<T>(name, field,
                                                                                   group);
  }
};

using WriterDataset =
    urx::utils::io::WriterDatasetBase<Dataset, AllTypeInVariant, uac::utils::io::WriterBase>;

}  // namespace uac::utils::io
