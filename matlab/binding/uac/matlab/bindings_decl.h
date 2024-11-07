#ifndef UAC_LIB_BINDING_DECL
#define UAC_LIB_BINDING_DECL

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>  // IWYU pragma: keep
#else
#include <stddef.h>  // IWYU pragma: keep
#include <stdint.h>  // IWYU pragma: keep
#endif

#include <urx/matlab/bindings_decl.h>
#include <urx/matlab/export.h>

// NOLINTBEGIN(bugprone-macro-parentheses)

#define _HW_CONFIG_DECL(name)                                                                     \
  URX_MATLAB_EXPORT void *CONCAT2(name, new)(void);                                               \
  URX_MATLAB_EXPORT void CONCAT2(name, delete)(void *this_ptr);                                   \
  URX_MATLAB_EXPORT void CONCAT3(name, clear, raw)(void *this_ptr);                               \
  URX_MATLAB_EXPORT void CONCAT3(name, clear, shared)(void *this_ptr);                            \
  URX_MATLAB_EXPORT uint64_t CONCAT3(name, size, raw)(void *this_ptr);                            \
  URX_MATLAB_EXPORT uint64_t CONCAT3(name, size, shared)(void *this_ptr);                         \
  URX_MATLAB_EXPORT void CONCAT4(name, assign, raw, raw)(void *this_ptr, void *other_ptr);        \
  URX_MATLAB_EXPORT void CONCAT4(name, assign, raw, shared)(void *this_ptr, void *other_ptr);     \
  URX_MATLAB_EXPORT void CONCAT4(name, assign, weak, shared)(void *this_ptr, void *other_ptr);    \
  URX_MATLAB_EXPORT void CONCAT4(name, assign, shared, shared)(void *this_ptr, void *other_ptr);  \
  URX_MATLAB_EXPORT const char *CONCAT4(name, get, key, raw)(void *this_ptr, uint64_t i);         \
  URX_MATLAB_EXPORT const char *CONCAT4(name, get, key, shared)(void *this_ptr, uint64_t i);      \
  URX_MATLAB_EXPORT const void *CONCAT4(name, get, value, raw)(void *this_ptr, uint64_t i);       \
  URX_MATLAB_EXPORT const void *CONCAT4(name, get, value, shared)(void *this_ptr, uint64_t i);    \
  URX_MATLAB_EXPORT const char *CONCAT5(name, get, value, string, raw)(void *this_ptr,            \
                                                                       uint64_t i);               \
  URX_MATLAB_EXPORT const char *CONCAT5(name, get, value, string, shared)(void *this_ptr,         \
                                                                          uint64_t i);            \
  URX_MATLAB_EXPORT uint64_t CONCAT5(name, get, size, vec_string, raw)(void *this_ptr,            \
                                                                       uint64_t i);               \
  URX_MATLAB_EXPORT uint64_t CONCAT5(name, get, size, vec_string, shared)(void *this_ptr,         \
                                                                          uint64_t i);            \
  URX_MATLAB_EXPORT const char *CONCAT5(name, get, value, vec_string, raw)(                       \
      void *this_ptr, uint64_t i, uint64_t j);                                                    \
  URX_MATLAB_EXPORT const char *CONCAT5(name, get, value, vec_string, shared)(                    \
      void *this_ptr, uint64_t i, uint64_t j);                                                    \
  URX_MATLAB_EXPORT uint64_t CONCAT5(name, get, size, vec_struct, raw)(void *this_ptr,            \
                                                                       uint64_t i);               \
  URX_MATLAB_EXPORT uint64_t CONCAT5(name, get, size, vec_struct, shared)(void *this_ptr,         \
                                                                          uint64_t i);            \
  URX_MATLAB_EXPORT void *CONCAT5(name, get, value, vec_struct, raw)(void *this_ptr, uint64_t i,  \
                                                                     uint64_t j);                 \
  URX_MATLAB_EXPORT void *CONCAT5(name, get, value, vec_struct, shared)(void *this_ptr,           \
                                                                        uint64_t i, uint64_t j);  \
  URX_MATLAB_EXPORT uint32_t CONCAT5(name, get, value, size, raw)(void *this_ptr, uint64_t i);    \
  URX_MATLAB_EXPORT uint32_t CONCAT5(name, get, value, size, shared)(void *this_ptr, uint64_t i); \
  URX_MATLAB_EXPORT uint32_t CONCAT4(name, get, type, raw)(void *this_ptr, uint64_t i);           \
  URX_MATLAB_EXPORT uint32_t CONCAT4(name, get, type, shared)(void *this_ptr, uint64_t i);        \
  URX_MATLAB_EXPORT void CONCAT4(name, set, raw, raw)(                                            \
      void *hw_config, const char *name, uint32_t type_val, const void *value, uint32_t size);    \
  URX_MATLAB_EXPORT void CONCAT4(name, set, raw, shared)(                                         \
      void *hw_config, const char *name, uint32_t type_val, const void *value, uint32_t size);    \
  URX_MATLAB_EXPORT void CONCAT4(name, set, shared, raw)(                                         \
      void *hw_config, const char *name, uint32_t type_val, const void *value, uint32_t size);    \
  URX_MATLAB_EXPORT void CONCAT4(name, set, shared, shared)(                                      \
      void *hw_config, const char *name, uint32_t type_val, const void *value, uint32_t size);    \
  URX_MATLAB_EXPORT void CONCAT5(name, set, vec_string, raw, raw)(                                \
      void *hw_config, const char *name, uint32_t type_val, const char *const *value,             \
      uint32_t size);                                                                             \
  URX_MATLAB_EXPORT void CONCAT5(name, set, vec_string, shared, raw)(                             \
      void *hw_config, const char *name, uint32_t type_val, const char *const *value,             \
      uint32_t size);                                                                             \
  URX_MATLAB_EXPORT void CONCAT5(name, set, vec_struct, raw, raw)(                                \
      void *hw_config, const char *name, uint32_t type_val, const char *const *value,             \
      uint32_t size);                                                                             \
  URX_MATLAB_EXPORT void CONCAT5(name, set, vec_struct, shared, raw)(                             \
      void *hw_config, const char *name, uint32_t type_val, const char *const *value,             \
      uint32_t size);                                                                             \
  URX_MATLAB_EXPORT void CONCAT5(name, append, hwconfig, raw, raw)(                               \
      void *hw_config, const char *name, const void *value);                                      \
  URX_MATLAB_EXPORT void CONCAT5(name, append, hwconfig, shared, raw)(                            \
      void *hw_config, const char *name, const void *value);                                      \
  URX_MATLAB_EXPORT void CONCAT5(name, append, hwconfig, raw, shared)(                            \
      void *hw_config, const char *name, const void *value);                                      \
  URX_MATLAB_EXPORT void CONCAT5(name, append, hwconfig, shared, shared)(                         \
      void *hw_config, const char *name, const void *value)

#define HW_CONFIG_NS_DECL(ns, type) _HW_CONFIG_DECL(CONCAT2(ns, type))
#define HW_CONFIG_DECL(type) _HW_CONFIG_DECL(type)

// NOLINTEND(bugprone-macro-parentheses)

#endif  // #define UAC_LIB_BINDING_DECL
