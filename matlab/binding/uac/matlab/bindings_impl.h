#ifndef UAC_LIB_BINDING_IMPL
#define UAC_LIB_BINDING_IMPL

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include <urx/matlab/bindings_decl.h>

#include <uac/hw_config.h>
#include <uac/matlab/bindings.h>

// NOLINTBEGIN(bugprone-macro-parentheses)

#define _HW_CONFIG_IMPL(snake, type)                                                               \
  void *CONCAT2(snake, new)(void) {                                                                \
    urxIncAllocCount();                                                                            \
    auto retval = new std::shared_ptr<type>(new type());                                           \
    urxGetLog() << reinterpret_cast<size_t>(retval) << " " << __FUNCTION__ << "\n" << std::flush;  \
    return retval;                                                                                 \
  }                                                                                                \
  void CONCAT2(snake, delete)(void *this_ptr) {                                                    \
    urxDecAllocCount();                                                                            \
    urxGetLog() << reinterpret_cast<size_t>(this_ptr) << " "                                       \
                << reinterpret_cast<size_t>(static_cast<std::shared_ptr<type> *>(this_ptr)->get()) \
                << " " << static_cast<std::shared_ptr<type> *>(this_ptr)->use_count() << " "       \
                << __FUNCTION__ << "\n"                                                            \
                << std::flush;                                                                     \
    delete static_cast<std::shared_ptr<type> *>(this_ptr);                                         \
  }                                                                                                \
  void CONCAT3(snake, clear, raw)(void *this_ptr) {                                                \
    static_cast<type *>(this_ptr)->values.clear();                                                 \
  }                                                                                                \
  void CONCAT3(snake, clear, shared)(void *this_ptr) {                                             \
    (*static_cast<std::shared_ptr<type> *>(this_ptr))->values.clear();                             \
  }                                                                                                \
  uint64_t CONCAT3(snake, size, raw)(void *this_ptr) {                                             \
    return static_cast<type *>(this_ptr)->values.size();                                           \
  }                                                                                                \
  uint64_t CONCAT3(snake, size, shared)(void *this_ptr) {                                          \
    return (*static_cast<std::shared_ptr<type> *>(this_ptr))->values.size();                       \
  }                                                                                                \
  void CONCAT4(snake, assign, raw, raw)(void *this_ptr, void *other_ptr) {                         \
    *static_cast<type *>(this_ptr) = *static_cast<type *>(other_ptr);                              \
  }                                                                                                \
  void CONCAT4(snake, assign, raw, shared)(void *this_ptr, void *other_ptr) {                      \
    *static_cast<type *>(this_ptr) = **static_cast<std::shared_ptr<type> *>(other_ptr);            \
  }                                                                                                \
  void CONCAT4(snake, assign, weak, shared)(void *this_ptr, void *other_ptr) {                     \
    *static_cast<std::weak_ptr<type> *>(this_ptr) =                                                \
        *static_cast<std::shared_ptr<type> *>(other_ptr);                                          \
  }                                                                                                \
  void CONCAT4(snake, assign, shared, shared)(void *this_ptr, void *other_ptr) {                   \
    *static_cast<std::shared_ptr<type> *>(this_ptr) =                                              \
        *static_cast<std::shared_ptr<type> *>(other_ptr);                                          \
  }                                                                                                \
  const char *CONCAT4(snake, get, key, raw)(void *this_ptr, uint64_t i) {                          \
    auto it = static_cast<type *>(this_ptr) -> values.begin();                                     \
    std::advance(it, i);                                                                           \
    return it->first.c_str();                                                                      \
  }                                                                                                \
  const char *CONCAT4(snake, get, key, shared)(void *this_ptr, uint64_t i) {                       \
    auto it = (*static_cast<std::shared_ptr<type> *>(this_ptr)) -> values.begin();                 \
    std::advance(it, i);                                                                           \
    return it->first.c_str();                                                                      \
  }                                                                                                \
  const void *CONCAT4(snake, get, value, raw)(void *this_ptr, uint64_t i) {                        \
    auto it = static_cast<type *>(this_ptr) -> values.begin();                                     \
    std::advance(it, i);                                                                           \
    return variant_to_void(it->second);                                                            \
  }                                                                                                \
  const void *CONCAT4(snake, get, value, shared)(void *this_ptr, uint64_t i) {                     \
    auto it = (*static_cast<std::shared_ptr<type> *>(this_ptr)) -> values.begin();                 \
    std::advance(it, i);                                                                           \
    return variant_to_void(it->second);                                                            \
  }                                                                                                \
  const char *CONCAT5(snake, get, value, string, raw)(void *this_ptr, uint64_t i) {                \
    auto it = static_cast<type *>(this_ptr) -> values.begin();                                     \
    std::advance(it, i);                                                                           \
    return std::get<std::string>(it->second).c_str();                                              \
  }                                                                                                \
  const char *CONCAT5(snake, get, value, string, shared)(void *this_ptr, uint64_t i) {             \
    auto it = (*static_cast<std::shared_ptr<type> *>(this_ptr)) -> values.begin();                 \
    std::advance(it, i);                                                                           \
    return std::get<std::string>(it->second).c_str();                                              \
  }                                                                                                \
  uint64_t CONCAT5(snake, get, size, vec_string, raw)(void *this_ptr, uint64_t i) {                \
    auto it = static_cast<type *>(this_ptr) -> values.begin();                                     \
    std::advance(it, i);                                                                           \
    return std::get<std::vector<std::string>>(it->second).size();                                  \
  }                                                                                                \
  uint64_t CONCAT5(snake, get, size, vec_string, shared)(void *this_ptr, uint64_t i) {             \
    auto it = (*static_cast<std::shared_ptr<type> *>(this_ptr)) -> values.begin();                 \
    std::advance(it, i);                                                                           \
    return std::get<std::vector<std::string>>(it->second).size();                                  \
  }                                                                                                \
  const char *CONCAT5(snake, get, value, vec_string, raw)(void *this_ptr, uint64_t i,              \
                                                          uint64_t j) {                            \
    auto it = static_cast<type *>(this_ptr) -> values.begin();                                     \
    std::advance(it, i);                                                                           \
    return std::get<std::vector<std::string>>(it->second)[j].c_str();                              \
  }                                                                                                \
  const char *CONCAT5(snake, get, value, vec_string, shared)(void *this_ptr, uint64_t i,           \
                                                             uint64_t j) {                         \
    auto it = (*static_cast<std::shared_ptr<type> *>(this_ptr)) -> values.begin();                 \
    std::advance(it, i);                                                                           \
    return std::get<std::vector<std::string>>(it->second)[j].c_str();                              \
  }                                                                                                \
  uint64_t CONCAT5(snake, get, size, vec_struct, raw)(void *this_ptr, uint64_t i) {                \
    auto it = static_cast<type *>(this_ptr) -> values.begin();                                     \
    std::advance(it, i);                                                                           \
    return std::get<std::vector<type>>(it->second).size();                                         \
  }                                                                                                \
  uint64_t CONCAT5(snake, get, size, vec_struct, shared)(void *this_ptr, uint64_t i) {             \
    auto it = (*static_cast<std::shared_ptr<type> *>(this_ptr)) -> values.begin();                 \
    std::advance(it, i);                                                                           \
    return std::get<std::vector<type>>(it->second).size();                                         \
  }                                                                                                \
  void *CONCAT5(snake, get, value, vec_struct, raw)(void *this_ptr, uint64_t i, uint64_t j) {      \
    auto it = static_cast<type *>(this_ptr) -> values.begin();                                     \
    std::advance(it, i);                                                                           \
    return &std::get<std::vector<type>>(it->second)[j];                                            \
  }                                                                                                \
  void *CONCAT5(snake, get, value, vec_struct, shared)(void *this_ptr, uint64_t i, uint64_t j) {   \
    auto it = (*static_cast<std::shared_ptr<type> *>(this_ptr)) -> values.begin();                 \
    std::advance(it, i);                                                                           \
    return &std::get<std::vector<type>>(it->second)[j];                                            \
  }                                                                                                \
  uint32_t CONCAT5(snake, get, value, size, raw)(void *this_ptr, uint64_t i) {                     \
    auto it = static_cast<type *>(this_ptr) -> values.begin();                                     \
    std::advance(it, i);                                                                           \
    return variant_to_size(it->second);                                                            \
  }                                                                                                \
  uint32_t CONCAT5(snake, get, value, size, shared)(void *this_ptr, uint64_t i) {                  \
    auto it = (*static_cast<std::shared_ptr<type> *>(this_ptr)) -> values.begin();                 \
    std::advance(it, i);                                                                           \
    return variant_to_size(it->second);                                                            \
  }                                                                                                \
  uint32_t CONCAT3(snake, get_type, raw)(void *this_ptr, uint64_t i) {                             \
    auto it = static_cast<type *>(this_ptr) -> values.begin();                                     \
    std::advance(it, i);                                                                           \
    return variant_to_type(it->second);                                                            \
  }                                                                                                \
  uint32_t CONCAT3(snake, get_type, shared)(void *this_ptr, uint64_t i) {                          \
    auto it = (*static_cast<std::shared_ptr<type> *>(this_ptr)) -> values.begin();                 \
    std::advance(it, i);                                                                           \
    return variant_to_type(it->second);                                                            \
  }                                                                                                \
  void CONCAT4(snake, set, raw, raw)(void *hw_config, const char *name, uint32_t type_val,         \
                                     const void *value, uint32_t size) {                           \
    static_cast<type *>(hw_config)->values[name] =                                                 \
        void_to_variant(static_cast<TypeHwConfig>(type_val), value, size);                         \
  }                                                                                                \
  void CONCAT4(snake, set, raw, shared)(void *hw_config, const char *name, uint32_t type_val,      \
                                        const void *value, uint32_t) {                             \
    static_cast<type *>(hw_config)->values[name] =                                                 \
        shared_void_to_variant(static_cast<TypeHwConfig>(type_val), value);                        \
  }                                                                                                \
  void CONCAT4(snake, set, shared, raw)(void *hw_config, const char *name, uint32_t type_val,      \
                                        const void *value, uint32_t size) {                        \
    (*static_cast<std::shared_ptr<type> *>(hw_config))->values[name] =                             \
        void_to_variant(static_cast<TypeHwConfig>(type_val), value, size);                         \
  }                                                                                                \
  void CONCAT4(snake, set, shared, shared)(void *hw_config, const char *name, uint32_t type_val,   \
                                           const void *value, uint32_t) {                          \
    (*static_cast<std::shared_ptr<type> *>(hw_config))->values[name] =                             \
        shared_void_to_variant(static_cast<TypeHwConfig>(type_val), value);                        \
  }                                                                                                \
  void CONCAT5(snake, set, vec_string, raw, raw)(void *hw_config, const char *name,                \
                                                 uint32_t type_val, const char *const *value,      \
                                                 uint32_t size) {                                  \
    static_cast<type *>(hw_config)->values[name] =                                                 \
        void_to_variant(static_cast<TypeHwConfig>(type_val), value, size);                         \
  }                                                                                                \
  void CONCAT5(snake, set, vec_string, shared, raw)(void *hw_config, const char *name,             \
                                                    uint32_t type_val, const char *const *value,   \
                                                    uint32_t size) {                               \
    (*static_cast<std::shared_ptr<type> *>(hw_config))->values[name] =                             \
        void_to_variant(static_cast<TypeHwConfig>(type_val), value, size);                         \
  }                                                                                                \
  void CONCAT5(snake, set, vec_struct, raw, raw)(void *hw_config, const char *name,                \
                                                 uint32_t type_val, const char *const *value,      \
                                                 uint32_t size) {                                  \
    static_cast<type *>(hw_config)->values[name] =                                                 \
        void_to_variant(static_cast<TypeHwConfig>(type_val), value, size);                         \
  }                                                                                                \
  void CONCAT5(snake, set, vec_struct, shared, raw)(void *hw_config, const char *name,             \
                                                    uint32_t type_val, const char *const *value,   \
                                                    uint32_t size) {                               \
    (*static_cast<std::shared_ptr<type> *>(hw_config))->values[name] =                             \
        void_to_variant(static_cast<TypeHwConfig>(type_val), value, size);                         \
  }                                                                                                \
  void CONCAT5(snake, append, hwconfig, raw, raw)(void *hw_config, const char *name,               \
                                                  const void *value) {                             \
    auto &dict = static_cast<type *>(hw_config) -> values;                                         \
    if (dict.find(name) == dict.end()) {                                                           \
      dict[name] = std::vector<type>();                                                            \
    }                                                                                              \
    std::get<std::vector<type>>(dict[name]).push_back(*static_cast<const type *>(value));          \
  }                                                                                                \
  void CONCAT5(snake, append, hwconfig, raw, shared)(void *hw_config, const char *name,            \
                                                     const void *value) {                          \
    auto &dict = static_cast<type *>(hw_config) -> values;                                         \
    if (dict.find(name) == dict.end()) {                                                           \
      dict[name] = std::vector<type>();                                                            \
    }                                                                                              \
    std::get<std::vector<type>>(dict[name])                                                        \
        .push_back(**static_cast<const std::shared_ptr<type> *>(value));                           \
  }                                                                                                \
  void CONCAT5(snake, append, hwconfig, shared, raw)(void *hw_config, const char *name,            \
                                                     const void *value) {                          \
    auto &dict = (*static_cast<std::shared_ptr<type> *>(hw_config)) -> values;                     \
    if (dict.find(name) == dict.end()) {                                                           \
      dict[name] = std::vector<type>();                                                            \
    }                                                                                              \
    std::get<std::vector<type>>(dict[name]).push_back(*static_cast<const type *>(value));          \
  }                                                                                                \
  void CONCAT5(snake, append, hwconfig, shared, shared)(void *hw_config, const char *name,         \
                                                        const void *value) {                       \
    auto &dict = (*static_cast<std::shared_ptr<type> *>(hw_config)) -> values;                     \
    if (dict.find(name) == dict.end()) {                                                           \
      dict[name] = std::vector<type>();                                                            \
    }                                                                                              \
    std::get<std::vector<type>>(dict[name])                                                        \
        .push_back(**static_cast<const std::shared_ptr<type> *>(value));                           \
  }                                                                                                \
  FORCE_SEMICOLON

#define HW_CONFIG_NS_IMPL(ns, name) _HW_CONFIG_IMPL(CONCAT2(ns, name), CONCAT_NS(ns, name))
#define HW_CONFIG_NS2_IMPL(ns, name_snake, name_real) \
  _HW_CONFIG_IMPL(CONCAT2(ns, name_snake), CONCAT_NS(ns, name_real))
#define HW_CONFIG_IMPL(name) _HW_CONFIG_IMPL(name, name)

// NOLINTEND(bugprone-macro-parentheses)

#endif  // #define UAC_LIB_BINDING_IMPL
