#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <urx/detail/double_nan.h>
#include <urx/utils/io/test/io.h>

#include <uac/destination_link.h>
#include <uac/enums.h>
#include <uac/event.h>
#include <uac/excitation.h>
#include <uac/group.h>
#include <uac/hw_config.h>
#include <uac/receive_setup.h>
#include <uac/super_group.h>
#include <uac/transmit_setup.h>
#include <uac/trigger.h>

namespace uac::utils::io::test {

template <typename T>
std::shared_ptr<T> generateFakeDataset() {
  auto dataset = urx::utils::io::test::generateFakeDataset<T>();

  TriggerIn trigger_in;
  TriggerOut trigger_out;

  auto super_group = std::make_shared<SuperGroup>();
  super_group->time_offset = 1.23;
  trigger_in.channel = "Channel1";
  trigger_in.edge = uac::Edge::FALLING;
  super_group->trigger_in = trigger_in;
  trigger_out.channel = "Channel5";
  trigger_out.pulse_duration = 7.41;
  trigger_out.time_offset = 92;
  super_group->trigger_out = trigger_out;
  super_group->repetition_count = 45;

  DestinationLink destination_link;
  destination_link.destination = dataset->acquisition.groups[1];
  trigger_in.channel = "Channel6";
  trigger_in.edge = uac::Edge::LOW;
  destination_link.trigger = trigger_in;
  super_group->destinations.push_back(destination_link);

  destination_link.destination = dataset->acquisition.groups[0];
  trigger_in.channel = "Channel8";
  trigger_in.edge = uac::Edge::HIGH;
  destination_link.trigger = trigger_in;
  super_group->destinations.push_back(destination_link);

  super_group->period = 11;

  super_group->hw_config = {{{"str", std::vector<std::string>{"coucoud", "coucoud2"}},
                             {"numb", 125},
                             {"vect", std::vector<double>{10, 11, 12, 13, 14}}}};

  super_group->initial_group = dataset->acquisition.groups[0];
  super_group->description = "Hello";

  dataset->acquisition.super_groups.push_back(super_group);
  dataset->acquisition.initial_group = super_group;
  dataset->acquisition.time_offset = 11;
  trigger_in.channel = "Channel2";
  trigger_in.edge = uac::Edge::RISING;
  dataset->acquisition.trigger_in = trigger_in;
  trigger_out.channel = "Channel4";
  trigger_out.pulse_duration = 741;
  trigger_out.time_offset = 9.2;
  trigger_out.polarity = uac::Polarity::NEGATIVE;
  dataset->acquisition.trigger_out = trigger_out;

  dataset->acquisition.hw_config = {{{"str", std::string{"coucou"}},
                                     {"numb", 12.5},
                                     {"vect", std::vector<int>{10, 11, 12, 13, 14}},
                                     {"data2", std::vector<uac::HwConfig>{uac::HwConfig{
                                                   {{"str2", std::string{"coucou2"}},
                                                    {"vect2", std::vector<int>{9, 8, 7, 6}}}}}}}};

  const std::shared_ptr<Group> group = dataset->acquisition.groups[0];

  group->time_offset = -450;

  trigger_in.channel = "Channel15";
  trigger_in.edge = uac::Edge::FALLING;
  group->trigger_in = trigger_in;
  trigger_out.channel = "Channel24";
  trigger_out.pulse_duration = 7541;
  trigger_out.time_offset = 9.32;
  trigger_out.polarity = uac::Polarity::POSITIVE;
  group->trigger_out = trigger_out;
  group->repetition_count = 777;
  destination_link.destination = dataset->acquisition.groups[0];
  trigger_in.channel = "Channel8";
  trigger_in.edge = uac::Edge::HIGH;
  destination_link.trigger = trigger_in;
  super_group->destinations.push_back(destination_link);
  super_group->destinations.push_back(destination_link);
  super_group->destinations.push_back(destination_link);
  super_group->destinations.push_back(destination_link);
  super_group->destinations.push_back(destination_link);
  super_group->period = 448.3;
  super_group->hw_config = {{{"str", std::string{"coucou"}}}};

  Event& event = group->sequence[0];
  event.time_offset = 741;
  trigger_in.channel = "Channel10";
  trigger_in.edge = uac::Edge::UNDEFINED;
  event.trigger_in = trigger_in;
  trigger_out.channel = "Channel11";
  trigger_out.pulse_duration = 354;
  trigger_out.time_offset = 95.12;
  trigger_out.polarity = uac::Polarity::UNDEFINED;
  event.trigger_out = trigger_out;
  event.hw_config = {
      {{"num", static_cast<short>(9874)}, {"vect", std::vector<int64_t>{10LL, 9994LL}}}};
  event.receive_setup.hw_config = {{{"num", static_cast<uint64_t>(9874)}}};
  event.transmit_setup.hw_config = {{{"num", static_cast<unsigned int>(9874)}}};

  const std::shared_ptr<Excitation> excitation = dataset->acquisition.excitations[0];
  excitation->hw_config = {
      {{"num", static_cast<unsigned short>(9874)}, {"vect", std::vector<short>{741, -752}}}};

  return dataset;
}

}  // namespace uac::utils::io::test
