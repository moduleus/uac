#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include <urx/config.h>
#include <urx/dataset.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/impulse_response.h>
#include <urx/matlab/bindings_impl.h>
#include <urx/probe.h>
#include <urx/transform.h>
#include <urx/utils/type_container.h>
#include <urx/vector.h>
#include <urx/wave.h>

#include <uac/acquisition.h>
#include <uac/dataset.h>  // IWYU pragma: keep
#include <uac/destination_link.h>
#include <uac/element.h>           // IWYU pragma: keep
#include <uac/element_geometry.h>  // IWYU pragma: keep
#include <uac/event.h>
#include <uac/excitation.h>
#include <uac/group.h>
#include <uac/hw_config.h>
#include <uac/igroup.h>
#include <uac/impulse_response.h>  // IWYU pragma: keep
#include <uac/matlab/bindings.h>
#include <uac/matlab/bindings_impl.h>
#include <uac/probe.h>  // IWYU pragma: keep
#include <uac/receive_setup.h>
#include <uac/super_group.h>
#include <uac/transform.h>  // IWYU pragma: keep
#include <uac/transmit_setup.h>
#include <uac/trigger.h>
#include <uac/utils/to_urx.h>
#include <uac/vector.h>  // IWYU pragma: keep
#include <uac/version.h>
#include <uac/wave.h>  // IWYU pragma: keep

#ifdef URX_WITH_HDF5
#include <uac/utils/io/reader.h>
#include <uac/utils/io/writer.h>
#endif

namespace {

template <typename Container>
uac::IGroup *getIGroupPtr(const Container &container) {
  if constexpr (urx::utils::TypeContainer<Container>::VALUE ==
                urx::utils::ContainerType::SHARED_PTR) {
    return container.get();
  } else if constexpr (urx::utils::TypeContainer<Container>::VALUE ==
                       urx::utils::ContainerType::WEAK_PTR) {
    return container.lock().get();
  } else if constexpr (urx::utils::TypeContainer<Container>::VALUE ==
                       urx::utils::ContainerType::OPTIONAL) {
    return container ? &(*container) : nullptr;
  } else {
    throw std::runtime_error(__FUNCTION__);
  }
}

}  // namespace

// NOLINTBEGIN(cppcoreguidelines-owning-memory)

URX_MATLAB_ACQUISITION_IMPL(uac);
OBJECT_ACCESSOR_NS_IMPL(uac, Acquisition, super_groups);
OBJECT_ACCESSOR_NS_IMPL(uac, Acquisition, initial_group);
IGROUP_TRUE_TYPE_NS_IMPL(uac, Acquisition, initial_group);
OBJECT_ACCESSOR_NS_IMPL(uac, Acquisition, time_offset);
OBJECT_ACCESSOR_NS_IMPL(uac, Acquisition, trigger_in);
OBJECT_ACCESSOR_NS_IMPL(uac, Acquisition, trigger_out);
OBJECT_ACCESSOR_NS_IMPL(uac, Acquisition, hw_config);

URX_MATLAB_DATASET_IMPL(uac);

OBJECT_NS_IMPL(uac, DestinationLink);
OBJECT_ACCESSOR_NS_IMPL(uac, DestinationLink, trigger);
OBJECT_ACCESSOR_NS_IMPL(uac, DestinationLink, destination);
IGROUP_TRUE_TYPE_NS_IMPL(uac, DestinationLink, destination);

URX_MATLAB_ELEMENT_GEOMETRY_IMPL(uac);

URX_MATLAB_ELEMENT_IMPL(uac);

URX_MATLAB_EVENT_IMPL(uac);
OBJECT_ACCESSOR_NS_IMPL(uac, Event, time_offset);
OBJECT_ACCESSOR_NS_IMPL(uac, Event, trigger_in);
OBJECT_ACCESSOR_NS_IMPL(uac, Event, trigger_out);
OBJECT_ACCESSOR_NS_IMPL(uac, Event, hw_config);

URX_MATLAB_EXCITATION_IMPL(uac);
OBJECT_ACCESSOR_NS_IMPL(uac, Excitation, hw_config);

URX_MATLAB_GROUP_IMPL(uac);
OBJECT_ACCESSOR_NS_IMPL(uac, Group, time_offset);
OBJECT_ACCESSOR_NS_IMPL(uac, Group, trigger_in);
OBJECT_ACCESSOR_NS_IMPL(uac, Group, trigger_out);
OBJECT_ACCESSOR_NS_IMPL(uac, Group, repetition_count);
OBJECT_ACCESSOR_NS_IMPL(uac, Group, destinations);
OBJECT_ACCESSOR_NS_IMPL(uac, Group, period);
OBJECT_ACCESSOR_NS_IMPL(uac, Group, hw_config);

URX_MATLAB_IMPULSE_RESPONSE_IMPL(uac);

URX_MATLAB_PROBE_IMPL(uac);

URX_MATLAB_RECEIVE_SETUP_IMPL(uac);
OBJECT_ACCESSOR_NS_IMPL(uac, ReceiveSetup, hw_config);

OBJECT_NS_IMPL(uac, SuperGroup);
OBJECT_ACCESSOR_NS_IMPL(uac, SuperGroup, time_offset);
OBJECT_ACCESSOR_NS_IMPL(uac, SuperGroup, trigger_in);
OBJECT_ACCESSOR_NS_IMPL(uac, SuperGroup, trigger_out);
OBJECT_ACCESSOR_NS_IMPL(uac, SuperGroup, repetition_count);
OBJECT_ACCESSOR_NS_IMPL(uac, SuperGroup, destinations);
OBJECT_ACCESSOR_NS_IMPL(uac, SuperGroup, period);
OBJECT_ACCESSOR_NS_IMPL(uac, SuperGroup, hw_config);
OBJECT_ACCESSOR_NS_IMPL(uac, SuperGroup, initial_group);
IGROUP_TRUE_TYPE_NS_IMPL(uac, SuperGroup, initial_group);
OBJECT_ACCESSOR_NS_IMPL(uac, SuperGroup, description);

URX_MATLAB_TRANSFORM_IMPL(uac);

URX_MATLAB_TRANSMIT_SETUP_IMPL(uac);
OBJECT_ACCESSOR_NS_IMPL(uac, TransmitSetup, hw_config);

OBJECT_NS_IMPL(uac, TriggerIn);
OBJECT_ACCESSOR_NS_IMPL(uac, TriggerIn, channel);
OBJECT_ACCESSOR_NS_IMPL(uac, TriggerIn, edge);

OBJECT_NS_IMPL(uac, TriggerOut);
OBJECT_ACCESSOR_NS_IMPL(uac, TriggerOut, channel);
OBJECT_ACCESSOR_NS_IMPL(uac, TriggerOut, time_offset);
OBJECT_ACCESSOR_NS_IMPL(uac, TriggerOut, pulse_duration);
OBJECT_ACCESSOR_NS_IMPL(uac, TriggerOut, polarity);

URX_MATLAB_VECTOR3D_IMPL(uac);
URX_MATLAB_VECTOR2D_IMPL(uac);

URX_MATLAB_VERSION_IMPL(uac);

URX_MATLAB_WAVE_IMPL(uac);

VECTOR_RAW_NS_IMPL(uac, DestinationLink);
VECTOR_RAW_NS_IMPL(uac, Element);
VECTOR_RAW_NS_IMPL(uac, Event);
VECTOR_RAW_NS2_IMPL(uac, Vector3D, Vector3D<double>);

VECTOR_SHARED_NS_IMPL(uac, ElementGeometry);
VECTOR_SHARED_NS_IMPL(uac, Excitation);
VECTOR_SHARED_NS_IMPL(uac, Group);
VECTOR_SHARED_NS_IMPL(uac, ImpulseResponse);
VECTOR_SHARED_NS_IMPL(uac, Probe);
VECTOR_SHARED_NS_IMPL(uac, SuperGroup);

VECTOR_WEAK_NS_IMPL(uac, Excitation);

// NOLINTEND(cppcoreguidelines-owning-memory)

namespace {

template <class... Ts>
struct Overloaded : Ts... {
  using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

uac::HwConfig::VecDataTypeVariant void_to_variant(TypeHwConfig type, const void *value,
                                                  uint32_t size) {
  switch (type) {
    case HC_UINT8: {
      return *static_cast<const uint8_t *>(value);
    }
    case HC_INT8: {
      return *static_cast<const int8_t *>(value);
    }
    case HC_UINT16: {
      return *static_cast<const uint16_t *>(value);
    }
    case HC_INT16: {
      return *static_cast<const int16_t *>(value);
    }
    case HC_UINT32: {
      return *static_cast<const uint32_t *>(value);
    }
    case HC_INT32: {
      return *static_cast<const int32_t *>(value);
    }
    case HC_UINT64: {
      return *static_cast<const uint64_t *>(value);
    }
    case HC_INT64: {
      return *static_cast<const int64_t *>(value);
    }
    case HC_FLOAT: {
      return *static_cast<const float *>(value);
    }
    case HC_DOUBLE: {
      return *static_cast<const double *>(value);
    }
    case HC_STRING: {
      return std::string{static_cast<const char *>(value)};
    }
    case HC_STRUCT: {
      return *static_cast<const uac::HwConfig *>(value);
    }
    case HC_VEC_UINT8: {
      const auto *val_ptr = static_cast<const uint8_t *>(value);
      return std::vector<uint8_t>{val_ptr, val_ptr + size};
    }
    case HC_VEC_INT8: {
      const auto *val_ptr = static_cast<const int8_t *>(value);
      return std::vector<int8_t>{val_ptr, val_ptr + size};
    }
    case HC_VEC_UINT16: {
      const auto *val_ptr = static_cast<const uint16_t *>(value);
      return std::vector<uint16_t>{val_ptr, val_ptr + size};
    }
    case HC_VEC_INT16: {
      const auto *val_ptr = static_cast<const int16_t *>(value);
      return std::vector<int16_t>{val_ptr, val_ptr + size};
    }
    case HC_VEC_UINT32: {
      const auto *val_ptr = static_cast<const uint32_t *>(value);
      return std::vector<uint32_t>{val_ptr, val_ptr + size};
    }
    case HC_VEC_INT32: {
      const auto *val_ptr = static_cast<const int32_t *>(value);
      return std::vector<int32_t>{val_ptr, val_ptr + size};
    }
    case HC_VEC_UINT64: {
      const auto *val_ptr = static_cast<const uint64_t *>(value);
      return std::vector<uint64_t>{val_ptr, val_ptr + size};
    }
    case HC_VEC_INT64: {
      const auto *val_ptr = static_cast<const int64_t *>(value);
      return std::vector<int64_t>{val_ptr, val_ptr + size};
    }
    case HC_VEC_FLOAT: {
      const auto *val_ptr = static_cast<const float *>(value);
      return std::vector<float>{val_ptr, val_ptr + size};
    }
    case HC_VEC_DOUBLE: {
      const auto *val_ptr = static_cast<const double *>(value);
      return std::vector<double>{val_ptr, val_ptr + size};
    }
    case HC_VEC_STRING: {
      const auto *val_ptr = static_cast<const char *const *>(value);
      return std::vector<std::string>{val_ptr, val_ptr + size};
    }
    case HC_VEC_STRUCT: {
      const auto *val_ptr = static_cast<const uac::HwConfig *>(value);
      return std::vector<uac::HwConfig>{val_ptr, val_ptr + size};
    }
  }

  throw std::runtime_error("Unsupported type.");
}

uac::HwConfig::VecDataTypeVariant shared_void_to_variant(TypeHwConfig type, const void *value) {
  switch (type) {
    case HC_STRUCT: {
      return **static_cast<const std::shared_ptr<uac::HwConfig> *>(value);
    }
    case HC_VEC_UINT8: {
      return **static_cast<const std::shared_ptr<std::vector<uint8_t>> *>(value);
    }
    case HC_VEC_INT8: {
      return **static_cast<const std::shared_ptr<std::vector<int8_t>> *>(value);
    }
    case HC_VEC_UINT16: {
      return **static_cast<const std::shared_ptr<std::vector<uint16_t>> *>(value);
    }
    case HC_VEC_INT16: {
      return **static_cast<const std::shared_ptr<std::vector<int16_t>> *>(value);
    }
    case HC_VEC_UINT32: {
      return **static_cast<const std::shared_ptr<std::vector<uint32_t>> *>(value);
    }
    case HC_VEC_INT32: {
      return **static_cast<const std::shared_ptr<std::vector<int32_t>> *>(value);
    }
    case HC_VEC_UINT64: {
      return **static_cast<const std::shared_ptr<std::vector<uint64_t>> *>(value);
    }
    case HC_VEC_INT64: {
      return **static_cast<const std::shared_ptr<std::vector<int64_t>> *>(value);
    }
    case HC_VEC_FLOAT: {
      return **static_cast<const std::shared_ptr<std::vector<float>> *>(value);
    }
    case HC_VEC_DOUBLE: {
      return **static_cast<const std::shared_ptr<std::vector<double>> *>(value);
    }
    case HC_VEC_STRING: {
      return **static_cast<const std::shared_ptr<std::vector<std::string>> *>(value);
    }
    case HC_VEC_STRUCT: {
      return **static_cast<const std::shared_ptr<std::vector<uac::HwConfig>> *>(value);
    }
    default: {
      throw std::runtime_error("Unsupported type.");
    }
  }
}

const void *variant_to_void(const uac::HwConfig::VecDataTypeVariant &value) {
  return std::visit(
      Overloaded{[](const uint8_t &v) { return static_cast<const void *>(&v); },
                 [](const int8_t &v) { return static_cast<const void *>(&v); },
                 [](const uint16_t &v) { return static_cast<const void *>(&v); },
                 [](const int16_t &v) { return static_cast<const void *>(&v); },
                 [](const uint32_t &v) { return static_cast<const void *>(&v); },
                 [](const int32_t &v) { return static_cast<const void *>(&v); },
                 [](const uint64_t &v) { return static_cast<const void *>(&v); },
                 [](const int64_t &v) { return static_cast<const void *>(&v); },
                 [](const float &v) { return static_cast<const void *>(&v); },
                 [](const double &v) { return static_cast<const void *>(&v); },
                 [](const std::string &v) { return static_cast<const void *>(v.c_str()); },
                 [](const uac::HwConfig &v) { return static_cast<const void *>(&v); },
                 [](const std::vector<uint8_t> &v) { return static_cast<const void *>(v.data()); },
                 [](const std::vector<int8_t> &v) { return static_cast<const void *>(v.data()); },
                 [](const std::vector<uint16_t> &v) { return static_cast<const void *>(v.data()); },
                 [](const std::vector<int16_t> &v) { return static_cast<const void *>(v.data()); },
                 [](const std::vector<uint32_t> &v) { return static_cast<const void *>(v.data()); },
                 [](const std::vector<int32_t> &v) { return static_cast<const void *>(v.data()); },
                 [](const std::vector<uint64_t> &v) { return static_cast<const void *>(v.data()); },
                 [](const std::vector<int64_t> &v) { return static_cast<const void *>(v.data()); },
                 [](const std::vector<float> &v) { return static_cast<const void *>(v.data()); },
                 [](const std::vector<double> &v) { return static_cast<const void *>(v.data()); },
                 [](const std::vector<std::string> &v) { return static_cast<const void *>(&v); },
                 [](const std::vector<uac::HwConfig> &v) { return static_cast<const void *>(&v); }},
      value);
}

uint32_t variant_to_type(const uac::HwConfig::VecDataTypeVariant &value) {
  return std::visit(
      Overloaded{
          [](const uint8_t &) { return static_cast<uint32_t>(HC_UINT8); },
          [](const int8_t &) { return static_cast<uint32_t>(HC_INT8); },
          [](const uint16_t &) { return static_cast<uint32_t>(HC_UINT16); },
          [](const int16_t &) { return static_cast<uint32_t>(HC_INT16); },
          [](const uint32_t &) { return static_cast<uint32_t>(HC_UINT32); },
          [](const int32_t &) { return static_cast<uint32_t>(HC_INT32); },
          [](const uint64_t &) { return static_cast<uint32_t>(HC_UINT64); },
          [](const int64_t &) { return static_cast<uint32_t>(HC_INT64); },
          [](const float &) { return static_cast<uint32_t>(HC_FLOAT); },
          [](const double &) { return static_cast<uint32_t>(HC_DOUBLE); },
          [](const std::string &) { return static_cast<uint32_t>(HC_STRING); },
          [](const uac::HwConfig &) { return static_cast<uint32_t>(HC_STRUCT); },
          [](const std::vector<uint8_t> &) { return static_cast<uint32_t>(HC_VEC_UINT8); },
          [](const std::vector<int8_t> &) { return static_cast<uint32_t>(HC_VEC_INT8); },
          [](const std::vector<uint16_t> &) { return static_cast<uint32_t>(HC_VEC_UINT16); },
          [](const std::vector<int16_t> &) { return static_cast<uint32_t>(HC_VEC_INT16); },
          [](const std::vector<uint32_t> &) { return static_cast<uint32_t>(HC_VEC_UINT32); },
          [](const std::vector<int32_t> &) { return static_cast<uint32_t>(HC_VEC_INT32); },
          [](const std::vector<uint64_t> &) { return static_cast<uint32_t>(HC_VEC_UINT64); },
          [](const std::vector<int64_t> &) { return static_cast<uint32_t>(HC_VEC_INT64); },
          [](const std::vector<float> &) { return static_cast<uint32_t>(HC_VEC_FLOAT); },
          [](const std::vector<double> &) { return static_cast<uint32_t>(HC_VEC_DOUBLE); },
          [](const std::vector<std::string> &) { return static_cast<uint32_t>(HC_VEC_STRING); },
          [](const std::vector<uac::HwConfig> &) { return static_cast<uint32_t>(HC_VEC_STRUCT); }},
      value);
}

uint32_t variant_to_size(const uac::HwConfig::VecDataTypeVariant &value) {
  return std::visit(
      Overloaded{
          [](const uint8_t &) { return 1U; },
          [](const int8_t &) { return 1U; },
          [](const uint16_t &) { return 1U; },
          [](const int16_t &) { return 1U; },
          [](const uint32_t &) { return 1U; },
          [](const int32_t &) { return 1U; },
          [](const uint64_t &) { return 1U; },
          [](const int64_t &) { return 1U; },
          [](const float &) { return 1U; },
          [](const double &) { return 1U; },
          [](const std::string &) { return 1U; },
          [](const uac::HwConfig &) { return 1U; },
          [](const std::vector<uint8_t> &v) { return static_cast<uint32_t>(v.size()); },
          [](const std::vector<int8_t> &v) { return static_cast<uint32_t>(v.size()); },
          [](const std::vector<uint16_t> &v) { return static_cast<uint32_t>(v.size()); },
          [](const std::vector<int16_t> &v) { return static_cast<uint32_t>(v.size()); },
          [](const std::vector<uint32_t> &v) { return static_cast<uint32_t>(v.size()); },
          [](const std::vector<int32_t> &v) { return static_cast<uint32_t>(v.size()); },
          [](const std::vector<uint64_t> &v) { return static_cast<uint32_t>(v.size()); },
          [](const std::vector<int64_t> &v) { return static_cast<uint32_t>(v.size()); },
          [](const std::vector<float> &v) { return static_cast<uint32_t>(v.size()); },
          [](const std::vector<double> &v) { return static_cast<uint32_t>(v.size()); },
          [](const std::vector<std::string> &v) { return static_cast<uint32_t>(v.size()); },
          [](const std::vector<uac::HwConfig> &v) { return static_cast<uint32_t>(v.size()); }},
      value);
}

}  // namespace

// NOLINTBEGIN(cppcoreguidelines-owning-memory)

VECTOR_RAW_NS_IMPL(uac, HwConfig);

HW_CONFIG_NS_IMPL(uac, HwConfig);

// NOLINTEND(cppcoreguidelines-owning-memory)

TypeHwConfig hw_config_get_type(void *hw_config, void *name) {
  uac::HwConfig *hw = static_cast<uac::HwConfig *>(hw_config);
  std::string *n = static_cast<std::string *>(name);

  static std::unordered_map<std::type_index, TypeHwConfig> type_id_type{
      {std::type_index(typeid(uint8_t)), HC_UINT8},
      {std::type_index(typeid(uint16_t)), HC_UINT16},
      {std::type_index(typeid(uint32_t)), HC_UINT32},
      {std::type_index(typeid(uint64_t)), HC_UINT64},
      {std::type_index(typeid(int8_t)), HC_INT8},
      {std::type_index(typeid(int16_t)), HC_INT16},
      {std::type_index(typeid(int32_t)), HC_INT32},
      {std::type_index(typeid(int64_t)), HC_INT64},
      {std::type_index(typeid(float)), HC_FLOAT},
      {std::type_index(typeid(double)), HC_DOUBLE},
      {std::type_index(typeid(std::string)), HC_STRING},
      {std::type_index(typeid(uac::HwConfig)), HC_STRUCT},
      {std::type_index(typeid(std::vector<uint8_t>)), HC_VEC_UINT8},
      {std::type_index(typeid(std::vector<uint16_t>)), HC_VEC_UINT16},
      {std::type_index(typeid(std::vector<uint32_t>)), HC_VEC_UINT32},
      {std::type_index(typeid(std::vector<uint64_t>)), HC_VEC_UINT64},
      {std::type_index(typeid(std::vector<int8_t>)), HC_VEC_INT8},
      {std::type_index(typeid(std::vector<int16_t>)), HC_VEC_INT16},
      {std::type_index(typeid(std::vector<int32_t>)), HC_VEC_INT32},
      {std::type_index(typeid(std::vector<int64_t>)), HC_VEC_INT64},
      {std::type_index(typeid(std::vector<float>)), HC_VEC_FLOAT},
      {std::type_index(typeid(std::vector<double>)), HC_VEC_DOUBLE},
      {std::type_index(typeid(std::vector<std::string>)), HC_VEC_STRING},
      {std::type_index(typeid(std::vector<uac::HwConfig>)), HC_VEC_STRUCT},
  };

  const uac::HwConfig::VecDataTypeVariant &value = hw->values.at(*n);
  return std::visit([](const auto &v) { return type_id_type.at(typeid(decltype(v))); }, value);
}

void *hw_config_get_value(void *hw_config, void *name) {
  uac::HwConfig *hw = static_cast<uac::HwConfig *>(hw_config);
  std::string *n = static_cast<std::string *>(name);

  return &hw->values.at(*n);
}

void hw_config_set_value(void *hw_config, void *name, TypeHwConfig type, const void *value,
                         uint32_t size) {
  uac::HwConfig *hw = static_cast<uac::HwConfig *>(hw_config);
  std::string *n = static_cast<std::string *>(name);

  hw->values[*n] = void_to_variant(type, value, size);
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)

void *uac_load_from_file([[maybe_unused]] const char *filename) {
#ifdef URX_WITH_HDF5
  return new std::shared_ptr<uac::Dataset>(uac::utils::io::reader::loadFromFile(filename));
#else
  return nullptr;
#endif
}

void uac_save_to_file([[maybe_unused]] const char *filename, [[maybe_unused]] void *dataset) {
#ifdef URX_WITH_HDF5
  uac::utils::io::writer::saveToFile(filename,
                                     **static_cast<std::shared_ptr<uac::Dataset> *>(dataset));
#endif
}

void *uac_to_urx(void *dataset) {
  return new std::shared_ptr<urx::Dataset>(
      uac::utils::toUrx(**static_cast<std::shared_ptr<uac::Dataset> *>(dataset)));
}

// NOLINTEND(cppcoreguidelines-owning-memory)
