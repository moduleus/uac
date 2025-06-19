#ifndef UAC_LIB_BINDING
#define UAC_LIB_BINDING

// MATLAB parser doesn't support __attribute__((visibility("default"))).
#ifndef UrxMatlabBinding_EXPORTS
#define URX_MATLAB_STATIC_DEFINE
#endif

#include <urx/matlab/bindings_decl.h>
#include <urx/matlab/export.h>

#include <uac/matlab/bindings_decl.h>

// NOLINTBEGIN(readability-identifier-naming)

#ifdef __cplusplus
#include <cstddef>  // IWYU pragma: keep
#include <cstdint>  // IWYU pragma: keep
#include <iosfwd>

URX_MATLAB_EXPORT std::ostream &urxGetLog();
#else
#include <stddef.h>  // IWYU pragma: keep
#include <stdint.h>  // IWYU pragma: keep
#endif

#ifdef __cplusplus
extern "C" {
#endif

URX_MATLAB_EXPORT void urxIncAllocCount();
URX_MATLAB_EXPORT void urxDecAllocCount();

URX_MATLAB_EXPORT void std_string_set(void *this_ptr, const char *v);
URX_MATLAB_EXPORT const char *std_string_get(void *this_ptr);

URX_MATLAB_EXPORT uint64_t get_pointer(void *ptr);

URX_MATLAB_ACQUISITION_DECL(uac);
OBJECT_ACCESSOR_NS_DECL(uac, Acquisition, super_groups);
OBJECT_ACCESSOR_NS_DECL(uac, Acquisition, initial_group);
IGROUP_TRUE_TYPE_NS_DECL(uac, Acquisition, initial_group);
OBJECT_ACCESSOR_NS_DECL(uac, Acquisition, time_offset);
OBJECT_ACCESSOR_NS_DECL(uac, Acquisition, trigger_in);
OBJECT_ACCESSOR_NS_DECL(uac, Acquisition, trigger_out);
OBJECT_ACCESSOR_NS_DECL(uac, Acquisition, hw_config);

URX_MATLAB_DATASET_DECL(uac);

OBJECT_NS_DECL(uac, DestinationLink);
OBJECT_ACCESSOR_NS_DECL(uac, DestinationLink, trigger);
OBJECT_ACCESSOR_NS_DECL(uac, DestinationLink, destination);
IGROUP_TRUE_TYPE_NS_DECL(uac, DestinationLink, destination);

URX_MATLAB_ELEMENT_GEOMETRY_DECL(uac);

URX_MATLAB_ELEMENT_DECL(uac);

URX_MATLAB_EVENT_DECL(uac);
OBJECT_ACCESSOR_NS_DECL(uac, Event, time_offset);
OBJECT_ACCESSOR_NS_DECL(uac, Event, trigger_in);
OBJECT_ACCESSOR_NS_DECL(uac, Event, trigger_out);
OBJECT_ACCESSOR_NS_DECL(uac, Event, hw_config);

URX_MATLAB_EXCITATION_DECL(uac);
OBJECT_ACCESSOR_NS_DECL(uac, Excitation, hw_config);

URX_MATLAB_GROUP_DECL(uac);
OBJECT_ACCESSOR_NS_DECL(uac, Group, time_offset);
OBJECT_ACCESSOR_NS_DECL(uac, Group, trigger_in);
OBJECT_ACCESSOR_NS_DECL(uac, Group, trigger_out);
OBJECT_ACCESSOR_NS_DECL(uac, Group, repetition_count);
OBJECT_ACCESSOR_NS_DECL(uac, Group, destinations);
OBJECT_ACCESSOR_NS_DECL(uac, Group, period);
OBJECT_ACCESSOR_NS_DECL(uac, Group, hw_config);

IGROUP_TRUE_TYPE_NS_DECL(uac, SuperGroup, initial_group);

URX_MATLAB_IMPULSE_RESPONSE_DECL(uac);

URX_MATLAB_PROBE_DECL(uac);

URX_MATLAB_RECEIVE_SETUP_DECL(uac);
OBJECT_ACCESSOR_NS_DECL(uac, ReceiveSetup, hw_config);

OBJECT_NS_DECL(uac, SuperGroup);
OBJECT_ACCESSOR_NS_DECL(uac, SuperGroup, time_offset);
OBJECT_ACCESSOR_NS_DECL(uac, SuperGroup, trigger_in);
OBJECT_ACCESSOR_NS_DECL(uac, SuperGroup, trigger_out);
OBJECT_ACCESSOR_NS_DECL(uac, SuperGroup, repetition_count);
OBJECT_ACCESSOR_NS_DECL(uac, SuperGroup, destinations);
OBJECT_ACCESSOR_NS_DECL(uac, SuperGroup, period);
OBJECT_ACCESSOR_NS_DECL(uac, SuperGroup, hw_config);
OBJECT_ACCESSOR_NS_DECL(uac, SuperGroup, initial_group);
IGROUP_TRUE_TYPE_NS_DECL(uac, SuperGroup, initial_group);
OBJECT_ACCESSOR_NS_DECL(uac, SuperGroup, description);

URX_MATLAB_TRANSFORM_DECL(uac);

URX_MATLAB_TRANSMIT_SETUP_DECL(uac);
OBJECT_ACCESSOR_NS_DECL(uac, TransmitSetup, hw_config);

OBJECT_NS_DECL(uac, TriggerIn);
OBJECT_ACCESSOR_NS_DECL(uac, TriggerIn, channel);
OBJECT_ACCESSOR_NS_DECL(uac, TriggerIn, edge);

OBJECT_NS_DECL(uac, TriggerOut);
OBJECT_ACCESSOR_NS_DECL(uac, TriggerOut, channel);
OBJECT_ACCESSOR_NS_DECL(uac, TriggerOut, time_offset);
OBJECT_ACCESSOR_NS_DECL(uac, TriggerOut, pulse_duration);
OBJECT_ACCESSOR_NS_DECL(uac, TriggerOut, polarity);

URX_MATLAB_VECTOR3D_DECL(uac);
URX_MATLAB_VECTOR2D_DECL(uac);

URX_MATLAB_VERSION_DECL(uac);

URX_MATLAB_WAVE_DECL(uac);

HW_CONFIG_NS_DECL(uac, HwConfig);

VECTOR_RAW_NS_DECL(uac, DestinationLink);
VECTOR_RAW_NS_DECL(uac, Element);
VECTOR_RAW_NS_DECL(uac, Event);
VECTOR_RAW_NS_DECL(uac, Vector3D);

VECTOR_SHARED_NS_DECL(uac, ElementGeometry);
VECTOR_SHARED_NS_DECL(uac, Excitation);
VECTOR_SHARED_NS_DECL(uac, Group);
VECTOR_SHARED_NS_DECL(uac, ImpulseResponse);
VECTOR_SHARED_NS_DECL(uac, Probe);
VECTOR_SHARED_NS_DECL(uac, SuperGroup);

VECTOR_WEAK_NS_DECL(uac, Excitation);

enum TypeHwConfig {
  HC_UINT8,
  HC_INT8,
  HC_UINT16,
  HC_INT16,
  HC_UINT32,
  HC_INT32,
  HC_UINT64,
  HC_INT64,
  HC_FLOAT,
  HC_DOUBLE,
  HC_STRING,
  HC_STRUCT,
  HC_VEC_UINT8,
  HC_VEC_INT8,
  HC_VEC_UINT16,
  HC_VEC_INT16,
  HC_VEC_UINT32,
  HC_VEC_INT32,
  HC_VEC_UINT64,
  HC_VEC_INT64,
  HC_VEC_FLOAT,
  HC_VEC_DOUBLE,
  HC_VEC_STRING,
  HC_VEC_STRUCT
};

// NOLINTNEXTLINE(modernize-use-using)
typedef enum TypeHwConfig TypeHwConfig;

URX_MATLAB_EXPORT TypeHwConfig hw_config_get_type(void *hw_config, void *name);
URX_MATLAB_EXPORT void *hw_config_get_value(void *hw_config, void *name);
URX_MATLAB_EXPORT void hw_config_set_value(void *hw_config, void *name, TypeHwConfig type,
                                           const void *value, uint32_t size);

URX_MATLAB_EXPORT void *uac_load_from_file(const char *filename);
URX_MATLAB_EXPORT void uac_save_to_file(const char *filename, void *dataset);

URX_MATLAB_EXPORT void *uac_to_urx(void *dataset);

VECTOR_RAW_DECL_RAW(uint8_t);
VECTOR_RAW_DECL_RAW(uint16_t);
VECTOR_RAW_DECL_RAW(uint32_t);
VECTOR_RAW_DECL_RAW(uint64_t);
VECTOR_RAW_DECL_RAW(int8_t);
VECTOR_RAW_DECL_RAW(int16_t);
VECTOR_RAW_DECL_RAW(int32_t);
VECTOR_RAW_DECL_RAW(int64_t);
VECTOR_RAW_DECL_RAW(float);
VECTOR_RAW_DECL_RAW(double);
VECTOR_RAW_NS_DECL(std, string);

VECTOR_2D_RAW_DECL(uint8_t);
VECTOR_2D_RAW_DECL(uint16_t);
VECTOR_2D_RAW_DECL(uint32_t);
VECTOR_2D_RAW_DECL(uint64_t);
VECTOR_2D_RAW_DECL(int8_t);
VECTOR_2D_RAW_DECL(int16_t);
VECTOR_2D_RAW_DECL(int32_t);
VECTOR_2D_RAW_DECL(int64_t);
VECTOR_2D_RAW_DECL(float);
VECTOR_2D_RAW_DECL(double);
VECTOR_2D_RAW_NS_DECL(std, string);

#ifdef __cplusplus
}
#endif

// NOLINTEND(readability-identifier-naming)

#endif  // #define UAC_LIB_BINDING
