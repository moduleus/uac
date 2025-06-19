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

  dataset->acquisition.hw_config = {{{"int8_t", static_cast<int8_t>(1)},
                                     {"int16_t", static_cast<int16_t>(1)},
                                     {"int32_t", static_cast<int32_t>(1)},
                                     {"int64_t", static_cast<int64_t>(1)},
                                     {"uint8_t", static_cast<uint8_t>(1)},
                                     {"uint16_t", static_cast<uint16_t>(1)},
                                     {"uint32_t", static_cast<uint32_t>(1)},
                                     {"uint64_t", static_cast<uint64_t>(1)},
                                     {"float", 1.f},
                                     {"double", 1.},
                                     {"string", std::string{"coucou"}},
                                     {"v_int8_t", std::vector<int8_t>{1}},
                                     {"v_int16_t", std::vector<int16_t>{1}},
                                     {"v_int32_t", std::vector<int32_t>{1}},
                                     {"v_int64_t", std::vector<int64_t>{1}},
                                     {"v_uint8_t", std::vector<uint8_t>{1}},
                                     {"v_uint16_t", std::vector<uint16_t>{1}},
                                     {"v_uint32_t", std::vector<uint32_t>{1}},
                                     {"v_uint64_t", std::vector<uint64_t>{1}},
                                     {"v_float", std::vector<float>{1.f}},
                                     {"v_double", std::vector<double>{1.}},
                                     {"v_string", std::vector<std::string>{{"coucou"}}},
                                     {"v_int8_empty", std::vector<int8_t>()},
                                     {"v_int16_empty", std::vector<int16_t>()},
                                     {"v_int32_empty", std::vector<int32_t>()},
                                     {"v_int64_empty", std::vector<int64_t>()},
                                     {"v_uint8_empty", std::vector<uint8_t>()},
                                     {"v_uint16_empty", std::vector<uint16_t>()},
                                     {"v_uint32_empty", std::vector<uint32_t>()},
                                     {"v_uint64_empty", std::vector<uint64_t>()},
                                     {"v_float_empty", std::vector<float>()},
                                     {"v_double_empty", std::vector<double>()},
                                     {"v_string_empty", std::vector<std::string>()},
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
