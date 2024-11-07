#pragma once

#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include <H5Cpp.h>

#include <urx/utils/io/serialize_helper.h>
#include <urx/utils/io/writer_impl.h>

#include <uac/group.h>
#include <uac/hw_config.h>
#include <uac/igroup.h>
#include <uac/super_group.h>

namespace urx::utils::io::writer {

template <typename U>
struct SerializeHdf5<uac::HwConfig, U, ContainerType::RAW> {
  static void
  f(const std::string& name, const uac::HwConfig& field, const H5::Group& group,
    MapToSharedPtr& map,
    const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>& data_field) {
    const H5::Group group_child(group.createGroup(name));
    for (const auto& i : field.values) {
      std::visit(
          [&map, name_i = i.first, &group_child, &data_field](const auto& j) {
            SerializeHdf5<std::remove_const_t<std::remove_reference_t<decltype(j)>>, U>::f(
                name_i, j, group_child, map, data_field);
          },
          i.second);
    }
  }
};

template <typename U>
struct SerializeHdf5<std::weak_ptr<uac::IGroup>, U, ContainerType::WEAK_PTR> {
  static void
  f(const std::string& name, const std::weak_ptr<uac::IGroup>& field, const H5::Group& group,
    MapToSharedPtr& map,
    const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>& data_field) {
    // Never assigned
    if (!field.owner_before(std::weak_ptr<uac::IGroup>{}) &&
        !std::weak_ptr<uac::IGroup>{}.owner_before(field)) {
      return;
    }

    if (auto shared = field.lock()) {
      auto serialize_func = [&map, &group, &data_field, &name](const std::string& prefix,
                                                               const auto& ptr) {
        if (!ptr) {
          return;
        }
        using T = typename std::remove_reference_t<std::remove_cv_t<decltype(ptr)>>::element_type;
        const std::vector<std::shared_ptr<T>>& all_shared =
            *reinterpret_cast<const std::vector<std::shared_ptr<T>>*>(map.at(nameTypeid<T>()));
        auto idx = std::find_if(
            all_shared.begin(), all_shared.end(),
            [&ptr](const std::shared_ptr<T>& data) { return ptr.get() == data.get(); });
        if (idx == all_shared.end()) {
          throw std::runtime_error(("Failed to read data from " + name).c_str());
        }
        SerializeHdf5<std::string, U>::f(
            name, prefix + std::to_string(std::distance(all_shared.begin(), idx)), group, map,
            data_field);
      };
      serialize_func("s", std::dynamic_pointer_cast<uac::SuperGroup>(shared));
      serialize_func("g", std::dynamic_pointer_cast<uac::Group>(shared));
    } else {
      throw std::runtime_error(("Failed to read data from " + name).c_str());
    }
  }
};

}  // namespace urx::utils::io::writer
