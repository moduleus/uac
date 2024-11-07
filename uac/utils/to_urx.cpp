#include <algorithm>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/double_nan.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/event.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/receive_setup.h>
#include <urx/transform.h>
#include <urx/transmit_setup.h>
#include <urx/version.h>
#include <urx/wave.h>

#include <uac/acquisition.h>
#include <uac/dataset.h>
#include <uac/event.h>
#include <uac/group.h>
#include <uac/receive_setup.h>
#include <uac/transmit_setup.h>
#include <uac/utils/to_urx.h>
#include <uac/version.h>

namespace uac::utils {

namespace {

template <typename T>
size_t findVectorSharedPtrIndex(const std::vector<std::shared_ptr<T>>& container, T* pointer) {
  auto eg_it =
      std::find_if(container.begin(), container.end(),
                   [pointer](const std::shared_ptr<T>& eg_i) { return eg_i.get() == pointer; });
  if (eg_it != container.end()) {
    return std::distance(container.begin(), eg_it);
  }
  throw std::runtime_error("Failed to get index.");
}

template <typename Turx, typename Tuac>
std::shared_ptr<Turx> findSmartPointer(const std::vector<std::shared_ptr<Turx>>& container_urx,
                                       const std::vector<std::shared_ptr<Tuac>>& container_uac,
                                       const std::weak_ptr<Tuac>& pointer_uac) {
  std::shared_ptr<Turx> pointer_urx;
  const bool empty_ptr = !pointer_uac.owner_before(std::weak_ptr<Tuac>{}) &&
                         !std::weak_ptr<Tuac>{}.owner_before(pointer_uac);
  if (!empty_ptr) {
    const size_t index = findVectorSharedPtrIndex(container_uac, pointer_uac.lock().get());
    pointer_urx = container_urx[index];
  }

  return pointer_urx;
}
}  // namespace

std::shared_ptr<urx::Dataset> toUrx(const uac::Dataset& dataset) {
  std::shared_ptr<urx::Dataset> dataset_conv = std::make_shared<urx::Dataset>();

  dataset_conv->acquisition.authors = dataset.acquisition.authors;
  dataset_conv->acquisition.description = dataset.acquisition.description;
  dataset_conv->acquisition.local_time = dataset.acquisition.local_time;
  dataset_conv->acquisition.country_code = dataset.acquisition.country_code;
  dataset_conv->acquisition.system = dataset.acquisition.system;

  dataset_conv->acquisition.timestamp = dataset.acquisition.timestamp;

  for (const auto& probei : dataset.acquisition.probes) {
    std::shared_ptr<urx::Probe> probe_conv = std::make_shared<urx::Probe>();
    probe_conv->description = probei->description;
    probe_conv->type = probei->type;
    probe_conv->transform = probei->transform;

    for (const auto& element_geometry : probei->element_geometries) {
      probe_conv->element_geometries.push_back(
          std::make_shared<urx::ElementGeometry>(*element_geometry));
    }

    for (const auto& impulse_response : probei->impulse_responses) {
      probe_conv->impulse_responses.push_back(
          std::make_shared<urx::ImpulseResponse>(*impulse_response));
    }

    for (const auto& element : probei->elements) {
      urx::Element element_conv;

      element_conv.transform = element.transform;

      element_conv.element_geometry = findSmartPointer(
          probe_conv->element_geometries, probei->element_geometries, element.element_geometry);

      element_conv.impulse_response = findSmartPointer(
          probe_conv->impulse_responses, probei->impulse_responses, element.impulse_response);

      probe_conv->elements.push_back(std::move(element_conv));
    }

    dataset_conv->acquisition.probes.push_back(std::move(probe_conv));
  }

  for (const auto& excitation : dataset.acquisition.excitations) {
    dataset_conv->acquisition.excitations.push_back(std::make_shared<urx::Excitation>(*excitation));
  }

  for (const auto& group : dataset.acquisition.groups) {
    std::shared_ptr<urx::Group> group_conv = std::make_shared<urx::Group>();

    group_conv->sampling_type = group->sampling_type;
    group_conv->data_type = group->data_type;
    group_conv->description = group->description;
    group_conv->sound_speed = group->sound_speed;

    for (const auto& event : group->sequence) {
      urx::Event event_conv;

      event_conv.transmit_setup.probe = dataset_conv->acquisition.probes[findVectorSharedPtrIndex(
          dataset.acquisition.probes, event.transmit_setup.probe.lock().get())];

      event_conv.transmit_setup.wave = event.transmit_setup.wave;

      event_conv.transmit_setup.active_elements = event.transmit_setup.active_elements;

      for (const auto& excitation : event.transmit_setup.excitations) {
        event_conv.transmit_setup.excitations.push_back(
            dataset_conv->acquisition.excitations[findVectorSharedPtrIndex(
                dataset.acquisition.excitations, excitation.lock().get())]);
      }

      event_conv.transmit_setup.delays = event.transmit_setup.delays;

      event_conv.transmit_setup.probe_transform = event.transmit_setup.probe_transform;

      event_conv.transmit_setup.time_offset = event.transmit_setup.time_offset;

      event_conv.receive_setup.probe = dataset_conv->acquisition.probes[findVectorSharedPtrIndex(
          dataset.acquisition.probes, event.receive_setup.probe.lock().get())];

      event_conv.receive_setup.probe_transform = event.receive_setup.probe_transform;

      event_conv.receive_setup.sampling_frequency = event.receive_setup.sampling_frequency;

      event_conv.receive_setup.number_samples = event.receive_setup.number_samples;

      event_conv.receive_setup.active_elements = event.receive_setup.active_elements;

      event_conv.receive_setup.tgc_profile = event.receive_setup.tgc_profile;
      event_conv.receive_setup.tgc_sampling_frequency = event.receive_setup.tgc_sampling_frequency;

      event_conv.receive_setup.modulation_frequency = event.receive_setup.modulation_frequency;
      event_conv.receive_setup.time_offset = event.receive_setup.time_offset;

      group_conv->sequence.push_back(std::move(event_conv));
    }

    dataset_conv->acquisition.groups.push_back(std::move(group_conv));
  }

  dataset_conv->version = dataset.version;

  return dataset_conv;
}

}  // namespace uac::utils
