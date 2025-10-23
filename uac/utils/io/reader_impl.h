#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include <H5Cpp.h>

#include <urx/probe.h>
#include <urx/utils/common.h>
#include <urx/utils/cpp.h>
#include <urx/utils/io/reader_impl.h>
#include <urx/utils/io/serialize_helper.h>

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
class ReaderBase
    : public urx::utils::io::ReaderBase<Dataset, AllTypeInVariant,
                                        ReaderBase<Dataset, AllTypeInVariant, Derived>> {
 public:
  ReaderBase()
      : urx::utils::io::ReaderBase<Dataset, AllTypeInVariant,
                                   ReaderBase<Dataset, AllTypeInVariant, Derived>>() {
    if constexpr (std::is_same_v<Dataset, uac::Dataset>) {
      this->_data_field = getMemberMap();
    }
  }

  using urx::utils::io::ReaderBase<Dataset, AllTypeInVariant,
                                   ReaderBase<Dataset, AllTypeInVariant, Derived>>::deserializeAll;

  void init(const Dataset& dataset) {
    this->_map_to_shared_ptr[nameTypeid<Group>()] = &dataset.acquisition.groups;
    this->_map_to_shared_ptr[nameTypeid<Probe>()] = &dataset.acquisition.probes;
    this->_map_to_shared_ptr[nameTypeid<Excitation>()] = &dataset.acquisition.excitations;
    this->_map_to_shared_ptr[nameTypeid<SuperGroup>()] = &dataset.acquisition.super_groups;
  }

  uac::HwConfig::VecDataTypeVariant parseHdf5Object(const H5::AbstractDs& attr,
                                                    const std::string& obj_name,
                                                    const H5::Group& group_child) {
    const H5::DataSpace dataspace = attr.getSpace();

    const H5::DataType datatype = attr.getDataType();

    // Value
    if (dataspace.getSimpleExtentNdims() == 0) {
      if (datatype == H5::PredType::NATIVE_INT8) {
        int8_t value;
        this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
        return value;
      }
      if (datatype == H5::PredType::NATIVE_UINT8) {
        uint8_t value;
        this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
        return value;
      }
      if (datatype == H5::PredType::NATIVE_INT16) {
        int16_t value;
        this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
        return value;
      }
      if (datatype == H5::PredType::NATIVE_UINT16) {
        uint16_t value;
        this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
        return value;
      }
      if (datatype == H5::PredType::NATIVE_INT32) {
        int32_t value;
        this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
        return value;
      }
      if (datatype == H5::PredType::NATIVE_UINT32) {
        uint32_t value;
        this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
        return value;
      }
      if (datatype == H5::PredType::NATIVE_INT64) {
        int64_t value;
        this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
        return value;
      }
      if (datatype == H5::PredType::NATIVE_UINT64) {
        uint64_t value;
        this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
        return value;
      }
      if (datatype == H5::PredType::NATIVE_FLOAT) {
        float value;
        this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
        return value;
      }
      if (datatype == H5::PredType::NATIVE_DOUBLE) {
        double value;
        this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
        return value;
      }
      std::string value;
      this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
      return value;
    }
    // Vector.
    if (datatype == H5::PredType::NATIVE_INT8) {
      std::vector<int8_t> value;
      this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
      return value;
    }
    if (datatype == H5::PredType::NATIVE_UINT8) {
      std::vector<uint8_t> value;
      this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
      return value;
    }
    if (datatype == H5::PredType::NATIVE_INT16) {
      std::vector<int16_t> value;
      this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
      return value;
    }
    if (datatype == H5::PredType::NATIVE_UINT16) {
      std::vector<uint16_t> value;
      this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
      return value;
    }
    if (datatype == H5::PredType::NATIVE_INT32) {
      std::vector<int32_t> value;
      this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
      return value;
    }
    if (datatype == H5::PredType::NATIVE_UINT32) {
      std::vector<uint32_t> value;
      this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
      return value;
    }
    if (datatype == H5::PredType::NATIVE_INT64) {
      std::vector<int64_t> value;
      this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
      return value;
    }
    if (datatype == H5::PredType::NATIVE_UINT64) {
      std::vector<uint64_t> value;
      this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
      return value;
    }
    if (datatype == H5::PredType::NATIVE_FLOAT) {
      std::vector<float> value;
      this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
      return value;
    }
    if (datatype == H5::PredType::NATIVE_DOUBLE) {
      std::vector<double> value;
      this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
      return value;
    }
    std::vector<std::string> value;
    this->deserializeHdf5<decltype(value)>(obj_name, value, group_child);
    return value;
  }

  template <typename T>
  typename std::enable_if_t<urx::utils::TypeContainer<T>::VALUE == urx::utils::ContainerType::RAW>
  // NOLINTNEXTLINE(misc-no-recursion)
  deserializeHdf5(const std::string& name, T& field, const H5::Group& group) {
    if constexpr (std::is_same_v<T, uac::HwConfig>) {
      const H5::Group group_child(group.openGroup(name));

      const hsize_t num_objs = group_child.getNumObjs();

      for (hsize_t i = 0; i < num_objs; ++i) {
        const std::string obj_name = group_child.getObjnameByIdx(i);
        const H5G_obj_t obj_type = group_child.getObjTypeByIdx(i);

        if (obj_type == H5G_DATASET) {
          field.values[obj_name] =
              parseHdf5Object(group_child.openDataSet(obj_name), obj_name, group_child);
        }
        // HwConfig
        else if (obj_type == H5G_GROUP) {
          const H5::Group group_child2(group_child.openGroup(obj_name));
          if (group_child2.nameExists(urx::utils::common::formatIndexWithLeadingZeros(
                  0, urx::utils::io::ITER_LENGTH))) {
            std::vector<uac::HwConfig> hw_config;
            this->deserializeHdf5<std::vector<uac::HwConfig>>(obj_name, hw_config, group_child);
            field.values[obj_name] = hw_config;
          } else {
            uac::HwConfig hw_config;
            this->deserializeHdf5<uac::HwConfig>(obj_name, hw_config, group_child);
            field.values[obj_name] = hw_config;
          }
        }
      }

      const int num_attrs = group_child.getNumAttrs();

      for (int i = 0; i < num_attrs; ++i) {
        const H5::Attribute attr = group_child.openAttribute(i);
        const std::string obj_name = attr.getName();

        field.values[obj_name] = parseHdf5Object(attr, obj_name, group_child);
      }
    } else {
      urx::utils::io::ReaderBase<
          Dataset, AllTypeInVariant,
          ReaderBase<Dataset, AllTypeInVariant, Derived>>::template deserializeHdf5<T>(name, field,
                                                                                       group);
    }
  }

  template <typename T>
  typename std::enable_if_t<urx::utils::TypeContainer<T>::VALUE ==
                            urx::utils::ContainerType::SHARED_PTR>
  deserializeHdf5(const std::string& name, T& field, const H5::Group& group) {
    urx::utils::io::ReaderBase<
        Dataset, AllTypeInVariant,
        ReaderBase<Dataset, AllTypeInVariant, Derived>>::template deserializeHdf5<T>(name, field,
                                                                                     group);
  }

  template <typename T>
  typename std::enable_if_t<urx::utils::TypeContainer<T>::VALUE ==
                            urx::utils::ContainerType::OPTIONAL>
  deserializeHdf5(const std::string& name, T& field, const H5::Group& group) {
    urx::utils::io::ReaderBase<
        Dataset, AllTypeInVariant,
        ReaderBase<Dataset, AllTypeInVariant, Derived>>::template deserializeHdf5<T>(name, field,
                                                                                     group);
  }

  template <typename T>
  typename std::enable_if_t<urx::utils::TypeContainer<T>::VALUE ==
                            urx::utils::ContainerType::WEAK_PTR>
  deserializeHdf5(const std::string& name, T& field, const H5::Group& group) {
    if constexpr (std::is_same_v<T, std::weak_ptr<uac::IGroup>>) {
      if (group.nameExists(name) || group.attrExists(name)) {
        std::string idx;

        this->deserializeHdf5<std::string>(name, idx, group);

        if (idx[0] == 's') {
          const auto& map_i = urx::utils::getSharedPtr<uac::SuperGroup>(this->_map_to_shared_ptr);
          const size_t index = std::stoul(idx.substr(1));

          if (map_i.size() > index && map_i[index]) {
            field = map_i[index];
          } else {
            this->_async_weak_assign.push_back([&field, &map_i, index]() { field = map_i[index]; });
          }
        } else if (idx[0] == 'g') {
          const auto& map_i = urx::utils::getSharedPtr<uac::Group>(this->_map_to_shared_ptr);
          const size_t index = std::stoul(idx.substr(1));

          if (map_i.size() > index && map_i[index]) {
            field = map_i[index];
          } else {
            this->_async_weak_assign.push_back([&field, &map_i, index]() { field = map_i[index]; });
          }
        } else {
          throw std::runtime_error(("Failed to read data from " + name).c_str());
        }
      }
    } else {
      urx::utils::io::ReaderBase<
          Dataset, AllTypeInVariant,
          ReaderBase<Dataset, AllTypeInVariant, Derived>>::template deserializeHdf5<T>(name, field,
                                                                                       group);
    }
  }

  template <typename T>
  typename std::enable_if_t<urx::utils::TypeContainer<T>::VALUE ==
                            urx::utils::ContainerType::VECTOR>
  // NOLINTNEXTLINE(misc-no-recursion)
  deserializeHdf5(const std::string& name, T& field, const H5::Group& group) {
    urx::utils::io::ReaderBase<
        Dataset, AllTypeInVariant,
        ReaderBase<Dataset, AllTypeInVariant, Derived>>::template deserializeHdf5<T>(name, field,
                                                                                     group);
  }
};

using ReaderDataset =
    urx::utils::io::ReaderDatasetBase<Dataset, AllTypeInVariant, uac::utils::io::ReaderBase>;

}  // namespace uac::utils::io
