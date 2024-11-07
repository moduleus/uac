#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include <H5Cpp.h>

#include <urx/utils/common.h>
#include <urx/utils/io/reader_impl.h>
#include <urx/utils/io/serialize_helper.h>

#include <uac/group.h>
#include <uac/hw_config.h>
#include <uac/igroup.h>
#include <uac/super_group.h>

namespace urx::utils::io::reader {

template <typename U>
uac::HwConfig::VecDataTypeVariant parseHdf5Object(
    const H5::AbstractDs& attr, const std::string& obj_name, const H5::Group& group_child,
    MapToSharedPtr& map,
    const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>& data_field) {
  const H5::DataSpace dataspace = attr.getSpace();

  const H5::DataType datatype = attr.getDataType();

  // Value
  if (dataspace.getSimpleExtentNdims() == 0) {
    if (datatype == H5::PredType::NATIVE_INT8) {
      int8_t value;
      DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
      return value;
    }
    if (datatype == H5::PredType::NATIVE_UINT8) {
      uint8_t value;
      DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
      return value;
    }
    if (datatype == H5::PredType::NATIVE_INT16) {
      int16_t value;
      DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
      return value;
    }
    if (datatype == H5::PredType::NATIVE_UINT16) {
      uint16_t value;
      DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
      return value;
    }
    if (datatype == H5::PredType::NATIVE_INT32) {
      int32_t value;
      DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
      return value;
    }
    if (datatype == H5::PredType::NATIVE_UINT32) {
      uint32_t value;
      DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
      return value;
    }
    if (datatype == H5::PredType::NATIVE_INT64) {
      int64_t value;
      DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
      return value;
    }
    if (datatype == H5::PredType::NATIVE_UINT64) {
      uint64_t value;
      DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
      return value;
    }
    if (datatype == H5::PredType::NATIVE_FLOAT) {
      float value;
      DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
      return value;
    }
    if (datatype == H5::PredType::NATIVE_DOUBLE) {
      double value;
      DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
      return value;
    }
    std::string value;
    DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
    return value;
  }
  // Vector.
  if (datatype == H5::PredType::NATIVE_INT8) {
    std::vector<int8_t> value;
    DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
    return value;
  }
  if (datatype == H5::PredType::NATIVE_UINT8) {
    std::vector<uint8_t> value;
    DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
    return value;
  }
  if (datatype == H5::PredType::NATIVE_INT16) {
    std::vector<int16_t> value;
    DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
    return value;
  }
  if (datatype == H5::PredType::NATIVE_UINT16) {
    std::vector<uint16_t> value;
    DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
    return value;
  }
  if (datatype == H5::PredType::NATIVE_INT32) {
    std::vector<int32_t> value;
    DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
    return value;
  }
  if (datatype == H5::PredType::NATIVE_UINT32) {
    std::vector<uint32_t> value;
    DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
    return value;
  }
  if (datatype == H5::PredType::NATIVE_INT64) {
    std::vector<int64_t> value;
    DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
    return value;
  }
  if (datatype == H5::PredType::NATIVE_UINT64) {
    std::vector<uint64_t> value;
    DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
    return value;
  }
  if (datatype == H5::PredType::NATIVE_FLOAT) {
    std::vector<float> value;
    DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
    return value;
  }
  if (datatype == H5::PredType::NATIVE_DOUBLE) {
    std::vector<double> value;
    DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
    return value;
  }
  std::vector<std::string> value;
  DeserializeHdf5<decltype(value), U>::f(obj_name, value, group_child, map, data_field);
  return value;
}

template <typename U>
struct DeserializeHdf5<uac::HwConfig, U, ContainerType::RAW> {
  static void
  // NOLINTNEXTLINE(misc-no-recursion)
  f(const std::string& name, uac::HwConfig& field, const H5::Group& group, MapToSharedPtr& map,
    const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>& data_field) {
    const H5::Group group_child(group.openGroup(name));

    const hsize_t num_objs = group_child.getNumObjs();

    for (hsize_t i = 0; i < num_objs; ++i) {
      const std::string obj_name = group_child.getObjnameByIdx(i);
      const H5G_obj_t obj_type = group_child.getObjTypeByIdx(i);

      if (obj_type == H5G_DATASET) {
        field.values[obj_name] = parseHdf5Object(group_child.openDataSet(obj_name), obj_name,
                                                 group_child, map, data_field);
      }
      // HwConfig
      else if (obj_type == H5G_GROUP) {
        const H5::Group group_child2(group_child.openGroup(obj_name));
        if (group_child2.nameExists(common::formatIndexWithLeadingZeros(0, ITER_LENGTH))) {
          std::vector<uac::HwConfig> hw_config;
          DeserializeHdf5<std::vector<uac::HwConfig>, U>::f(obj_name, hw_config, group_child, map,
                                                            data_field);
          field.values[obj_name] = hw_config;
        } else {
          uac::HwConfig hw_config;
          DeserializeHdf5<uac::HwConfig, U>::f(obj_name, hw_config, group_child, map, data_field);
          field.values[obj_name] = hw_config;
        }
      }
    }

    const int num_attrs = group_child.getNumAttrs();

    for (int i = 0; i < num_attrs; ++i) {
      const H5::Attribute attr = group_child.openAttribute(i);
      const std::string obj_name = attr.getName();

      field.values[obj_name] = parseHdf5Object(attr, obj_name, group_child, map, data_field);
    }
  }
};

template <typename U>
struct DeserializeHdf5<std::weak_ptr<uac::IGroup>, U, ContainerType::WEAK_PTR> {
  static void
  f(const std::string& name, std::weak_ptr<uac::IGroup>& field, const H5::Group& group,
    MapToSharedPtr& map,
    const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>& data_field) {
    if (group.nameExists(name) || group.attrExists(name)) {
      std::string idx;

      DeserializeHdf5<std::string, U>::f(name, idx, group, map, data_field);

      if (idx[0] == 's') {
        const auto& map_i = getSharedPtr<uac::SuperGroup>(map);
        const size_t index = std::stoul(idx.substr(1));

        if (map_i.size() > index) {
          field = map_i[index];
        } else {
          // NOLINTNEXTLINE
          const_cast<std::vector<std::function<void()>>*>(
              static_cast<const std::vector<std::function<void()>>*>(
                  map.at(nameTypeid<std::function<void()>>())))
              ->push_back([&field, &map_i, index]() { field = map_i[index]; });
        }
      } else if (idx[0] == 'g') {
        const auto& map_i = getSharedPtr<uac::Group>(map);
        const size_t index = std::stoul(idx.substr(1));

        if (map_i.size() > index) {
          field = map_i[index];
        } else {
          // NOLINTNEXTLINE
          const_cast<std::vector<std::function<void()>>*>(
              static_cast<const std::vector<std::function<void()>>*>(
                  map.at(nameTypeid<std::function<void()>>())))
              ->push_back([&field, &map_i, index]() { field = map_i[index]; });
        }
      } else {
        throw std::runtime_error(("Failed to read data from " + name).c_str());
      }
    }
  }
};

}  // namespace urx::utils::io::reader
