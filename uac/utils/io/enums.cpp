#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include <urx/utils/io/enums.h>

#include <uac/enums.h>
#include <uac/utils/export.h>

namespace urx::utils::io::enums {

namespace {
std::unordered_map<uac::Edge, std::string> edge_to_string{{uac::Edge::RISING, "RISING"},
                                                          {uac::Edge::FALLING, "FALLING"},
                                                          {uac::Edge::HIGH, "RCA"},
                                                          {uac::Edge::LOW, "MATRIX"},
                                                          {uac::Edge::UNDEFINED, "UNDEFINED"}};
std::unordered_map<std::string, uac::Edge> string_to_edge =
    urx::utils::io::enums::invertMap(edge_to_string);

std::unordered_map<uac::Polarity, std::string> polarity_to_string{
    {uac::Polarity::POSITIVE, "POSITIVE"},
    {uac::Polarity::NEGATIVE, "NEGATIVE"},
    {uac::Polarity::UNDEFINED, "UNDEFINED"}};
std::unordered_map<std::string, uac::Polarity> string_to_polarity =
    urx::utils::io::enums::invertMap(polarity_to_string);
}  // namespace

template <>
UAC_UTILS_EXPORT uac::Edge stringToEnum(const std::string& enumeration) {
  return stringToEnumImpl(enumeration, string_to_edge);
}
template <>
UAC_UTILS_EXPORT uac::Polarity stringToEnum(const std::string& enumeration) {
  return stringToEnumImpl(enumeration, string_to_polarity);
}

template <>
UAC_UTILS_EXPORT std::string enumToString(uac::Edge enumeration) {
  return enumToStringImpl(enumeration, edge_to_string);
}
template <>
UAC_UTILS_EXPORT std::string enumToString(uac::Polarity enumeration) {
  return enumToStringImpl(enumeration, polarity_to_string);
}
}  // namespace urx::utils::io::enums
