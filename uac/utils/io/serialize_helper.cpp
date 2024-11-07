#include <memory>

#include <urx/detail/double_nan.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/transform.h>
#include <urx/utils/io/serialize_helper.h>
#include <urx/vector.h>
#include <urx/wave.h>

#include <uac/acquisition.h>
#include <uac/dataset.h>
#include <uac/destination_link.h>
#include <uac/element.h>
#include <uac/element_geometry.h>
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
#include <uac/utils/io/serialize_helper.h>
#include <uac/version.h>
#include <uac/wave.h>

namespace uac::utils::io {

#define str(s) #s

// NOLINTNEXTLINE(bugprone-macro-parentheses)
#define INDEXOF(c, member) reinterpret_cast<decltype(c::member)*>(&reinterpret_cast<c*>(0)->member)

#define MAP_ROW(c, member) std::make_pair(INDEXOF(c, member), std::string{str(member)})

#if defined __has_attribute
#if __has_attribute(no_sanitize)
__attribute__((no_sanitize("undefined")))
#endif
#endif
const std::unordered_map<std::type_index, std::vector<std::pair<AllTypeInVariant, std::string>>>&
getMemberMap() {
  static const std::unordered_map<std::type_index,
                                  std::vector<std::pair<AllTypeInVariant, std::string>>>
      member_map{
          {nameTypeid<Acquisition>(),
           {MAP_ROW(Acquisition, authors), MAP_ROW(Acquisition, description),
            MAP_ROW(Acquisition, local_time), MAP_ROW(Acquisition, country_code),
            MAP_ROW(Acquisition, system), MAP_ROW(Acquisition, timestamp),
            MAP_ROW(Acquisition, probes), MAP_ROW(Acquisition, excitations),
            MAP_ROW(Acquisition, groups), MAP_ROW(Acquisition, super_groups),
            MAP_ROW(Acquisition, initial_group), MAP_ROW(Acquisition, time_offset),
            MAP_ROW(Acquisition, trigger_in), MAP_ROW(Acquisition, trigger_out),
            MAP_ROW(Acquisition, hw_config)}},
          {nameTypeid<Dataset>(), {MAP_ROW(Dataset, acquisition), MAP_ROW(Dataset, version)}},
          {nameTypeid<DestinationLink>(),
           {MAP_ROW(DestinationLink, trigger), MAP_ROW(DestinationLink, destination)}},
          {nameTypeid<Element>(),
           {MAP_ROW(Element, transform), MAP_ROW(Element, element_geometry),
            MAP_ROW(Element, impulse_response)}},
          {nameTypeid<ElementGeometry>(), {MAP_ROW(ElementGeometry, perimeter)}},
          {nameTypeid<Event>(),
           {MAP_ROW(Event, transmit_setup), MAP_ROW(Event, receive_setup),
            MAP_ROW(Event, time_offset), MAP_ROW(Event, trigger_in), MAP_ROW(Event, trigger_out),
            MAP_ROW(Event, hw_config)}},
          {nameTypeid<Excitation>(),
           {MAP_ROW(Excitation, pulse_shape), MAP_ROW(Excitation, transmit_frequency),
            MAP_ROW(Excitation, sampling_frequency), MAP_ROW(Excitation, waveform),
            MAP_ROW(Excitation, hw_config)}},
          {nameTypeid<Group>(),
           {MAP_ROW(Group, sampling_type), MAP_ROW(Group, data_type), MAP_ROW(Group, description),
            MAP_ROW(Group, sound_speed), MAP_ROW(Group, sequence), MAP_ROW(IGroup, time_offset),
            MAP_ROW(IGroup, trigger_in), MAP_ROW(IGroup, trigger_out),
            MAP_ROW(IGroup, repetition_count), MAP_ROW(IGroup, destinations),
            MAP_ROW(IGroup, period), MAP_ROW(IGroup, hw_config)}},
          {nameTypeid<IGroup>(),
           {MAP_ROW(IGroup, time_offset), MAP_ROW(IGroup, trigger_in), MAP_ROW(IGroup, trigger_out),
            MAP_ROW(IGroup, repetition_count), MAP_ROW(IGroup, destinations),
            MAP_ROW(IGroup, period), MAP_ROW(IGroup, hw_config)}},
          {nameTypeid<ImpulseResponse>(),
           {MAP_ROW(ImpulseResponse, sampling_frequency), MAP_ROW(ImpulseResponse, time_offset),
            MAP_ROW(ImpulseResponse, units), MAP_ROW(ImpulseResponse, data)}},
          {nameTypeid<Probe>(),
           {MAP_ROW(Probe, description), MAP_ROW(Probe, type), MAP_ROW(Probe, transform),
            MAP_ROW(Probe, element_geometries), MAP_ROW(Probe, impulse_responses),
            MAP_ROW(Probe, elements)}},
          {nameTypeid<ReceiveSetup>(),
           {MAP_ROW(ReceiveSetup, probe), MAP_ROW(ReceiveSetup, probe_transform),
            MAP_ROW(ReceiveSetup, sampling_frequency), MAP_ROW(ReceiveSetup, number_samples),
            MAP_ROW(ReceiveSetup, active_elements), MAP_ROW(ReceiveSetup, tgc_profile),
            MAP_ROW(ReceiveSetup, tgc_sampling_frequency),
            MAP_ROW(ReceiveSetup, modulation_frequency), MAP_ROW(ReceiveSetup, time_offset),
            MAP_ROW(ReceiveSetup, hw_config)}},
          {nameTypeid<SuperGroup>(),
           {MAP_ROW(SuperGroup, time_offset), MAP_ROW(SuperGroup, trigger_in),
            MAP_ROW(SuperGroup, trigger_out), MAP_ROW(SuperGroup, repetition_count),
            MAP_ROW(SuperGroup, destinations), MAP_ROW(SuperGroup, period),
            MAP_ROW(SuperGroup, hw_config), MAP_ROW(SuperGroup, initial_group),
            MAP_ROW(SuperGroup, description)}},
          {nameTypeid<Transform>(),
           {MAP_ROW(Transform, rotation), MAP_ROW(Transform, translation)}},
          {nameTypeid<TransmitSetup>(),
           {MAP_ROW(TransmitSetup, probe), MAP_ROW(TransmitSetup, wave),
            MAP_ROW(TransmitSetup, active_elements), MAP_ROW(TransmitSetup, excitations),
            MAP_ROW(TransmitSetup, delays), MAP_ROW(TransmitSetup, probe_transform),
            MAP_ROW(TransmitSetup, time_offset), MAP_ROW(TransmitSetup, hw_config)}},
          {nameTypeid<TriggerIn>(), {MAP_ROW(TriggerIn, channel), MAP_ROW(TriggerIn, edge)}},
          {nameTypeid<TriggerOut>(),
           {MAP_ROW(TriggerOut, channel), MAP_ROW(TriggerOut, time_offset),
            MAP_ROW(TriggerOut, pulse_duration), MAP_ROW(TriggerOut, polarity)}},
          {nameTypeid<Vector3D<double>>(),
           {MAP_ROW(Vector3D<double>, x), MAP_ROW(Vector3D<double>, y),
            MAP_ROW(Vector3D<double>, z)}},
          {nameTypeid<Version>(),
           {MAP_ROW(Version, major), MAP_ROW(Version, minor), MAP_ROW(Version, patch)}},
          {nameTypeid<Wave>(),
           {MAP_ROW(Wave, type), MAP_ROW(Wave, time_zero), MAP_ROW(Wave, time_zero_reference_point),
            MAP_ROW(Wave, parameters)}},
      };
  return member_map;
}

}  // namespace uac::utils::io
