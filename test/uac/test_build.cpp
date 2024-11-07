#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/enums.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/transform.h>
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
#include <uac/vector.h>  // IWYU pragma: keep
#include <uac/version.h>
#include <uac/wave.h>

namespace uac::test {

TEST_CASE("Instantiate all classes to check build", "[test_build]") {
  const Acquisition acquisition;
  const Dataset dataset;
  const DestinationLink destination_link;
  const ElementGeometry element_geometry;
  const Element element;
  const Event event;
  const Excitation excitation;
  const Group group;
  const HwConfig hw_config;
  // const IGroup igroup;
  const ImpulseResponse impulse_response;
  const Probe probe;
  const ReceiveSetup receive_setup;
  const SuperGroup super_group;
  const Transform transform;
  const TransmitSetup transmit_setup;
  const TriggerIn trigger_in;
  const TriggerOut trigger_out;
  const Vector2D<double> vector_2d;
  const Vector3D<double> vector_3d;
  const Version version;
  const Wave wave;
}

TEST_CASE("Access UAC properties from Dataset", "[test_build]") {
  const Dataset dataset;

  auto hw_config = dataset.acquisition.hw_config;
}

TEST_CASE("Access UAC properties from Acquisition", "[test_build]") {
  Acquisition acquisition;

  std::shared_ptr<IGroup> group = std::make_shared<Group>();
  acquisition.initial_group = group;

  group = std::make_shared<SuperGroup>();
  acquisition.initial_group = group;

  acquisition.groups.push_back(std::make_shared<Group>());

  const std::shared_ptr<Excitation> excitation = std::make_shared<Excitation>();
  acquisition.excitations = {excitation};

  auto hw_config = acquisition.excitations[0]->hw_config;
  hw_config = acquisition.groups[0]->hw_config;
}

TEST_CASE("Access UAC properties from Group", "[test_build]") {
  const std::shared_ptr<Group> group = std::make_shared<Group>();
  const Event event;
  group->sequence = {event};
  group->sampling_type = urx::SamplingType::IQ;

  HwConfig hw_conf = group->sequence[0].hw_config;

  const std::shared_ptr<uac::SuperGroup> super_group = std::make_shared<uac::SuperGroup>();
  super_group->initial_group = group;
  hw_conf = super_group->initial_group.lock()->hw_config;
}

TEST_CASE("Access UAC properties from Event", "[test_build]") {
  const std::shared_ptr<Event> event = std::make_shared<Event>();

  const TransmitSetup ts;
  const ReceiveSetup rs;
  event->transmit_setup = ts;
  event->receive_setup = rs;
  event->hw_config = HwConfig();
  HwConfig hw_conf = event->transmit_setup.hw_config;
  hw_conf = event->receive_setup.hw_config;
}

}  // namespace uac::test
