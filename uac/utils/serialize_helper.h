#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/enums.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/transform.h>
#include <urx/utils/cpp.h>
#include <urx/vector.h>
#include <urx/wave.h>

#include <uac/acquisition.h>
#include <uac/destination_link.h>
#include <uac/element.h>
#include <uac/element_geometry.h>
#include <uac/enums.h>
#include <uac/event.h>
#include <uac/excitation.h>
#include <uac/group.h>
#include <uac/hw_config.h>
#include <uac/igroup.h>
#include <uac/impulse_response.h>
#include <uac/probe.h>
#include <uac/receive_setup.h>
#include <uac/super_group.h>
#include <uac/transform.h>
#include <uac/transmit_setup.h>
#include <uac/trigger.h>
#include <uac/vector.h>  // IWYU pragma: keep
#include <uac/version.h>
#include <uac/wave.h>

namespace uac::utils {

using urx::utils::MapToSharedPtr;
using urx::utils::nameTypeid;

using AllTypeInVariant = std::variant<
    Acquisition*, urx::DataType*, urx::DoubleNan*, Edge*, Polarity*, HwConfig*, urx::ProbeType*,
    ReceiveSetup*, urx::SamplingType*, Transform*, TransmitSetup*, std::optional<TriggerIn>*,
    std::optional<TriggerOut>*, Vector3D<double>*, Version*, Wave*, urx::WaveType*, double*, float*,
    int16_t*, int32_t*, int64_t*, int8_t*, std::shared_ptr<urx::RawData>*, std::string*,
    std::vector<DestinationLink>*, std::vector<Element>*, std::vector<Event>*,
    std::vector<Vector3D<double>>*, std::vector<double>*, std::vector<float>*,
    std::vector<int16_t>*, std::vector<int32_t>*, std::vector<int64_t>*, std::vector<int8_t>*,
    std::vector<std::shared_ptr<ElementGeometry>>*, std::vector<std::shared_ptr<Excitation>>*,
    std::vector<std::shared_ptr<Group>>*, std::vector<std::shared_ptr<ImpulseResponse>>*,
    std::vector<std::shared_ptr<Probe>>*, std::vector<std::vector<double>>*,
    std::vector<std::vector<uint32_t>>*, std::vector<std::weak_ptr<Excitation>>*,
    std::vector<std::shared_ptr<SuperGroup>>*, std::vector<uint16_t>*, std::vector<uint32_t>*,
    std::vector<uint64_t>*, std::vector<uint8_t>*, std::weak_ptr<ElementGeometry>*,
    std::weak_ptr<Group>*, std::weak_ptr<IGroup>*, std::weak_ptr<ImpulseResponse>*,
    std::weak_ptr<Probe>*, uint16_t*, uint32_t*, uint64_t*, uint8_t*>;

const std::unordered_map<std::type_index, std::vector<std::pair<AllTypeInVariant, std::string>>>&
getMemberMap();

}  // namespace uac::utils
