#include "helpers.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <ios>
#include <iostream>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <vector>

#include <urx/detail/double_nan.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/enums.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/transform.h>
#include <urx/vector.h>
#include <urx/wave.h>

#include <uac/element.h>
#include <uac/element_geometry.h>
#include <uac/enums.h>
#include <uac/event.h>
#include <uac/excitation.h>
#include <uac/group.h>
#include <uac/impulse_response.h>
#include <uac/probe.h>
#include <uac/receive_setup.h>
#include <uac/transmit_setup.h>
#include <uac/wave.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433
#endif

std::shared_ptr<uac::Probe> makeLinearArrayUac(size_t n_elements, double pitch, double width,
                                               double height, const std::string& description) {
  //makeLinearArrayUac.m Creates a linear array probe in uac format
  // Inputs:
  //   n_elements       : number of elements (transducers) in the probe
  //   pitch           : distance between the center of adjacent elements [m]
  //   width           : size of the element along the x-axis [m]
  //   height          : size of the element along the y-axis [m]
  //   description     : name or description of the probe
  //
  // Outputs:
  //   Probe (uac.Probe) : built as a linear array along the x-axis
  //
  // Drawing: Linear array (n_elements = 12)
  //                                          __ width
  //       ____ x    __ __ __ __ __ __ __ __ __ __ __ __
  //      /|        / // // // // // // // // // // // /    /
  //     / |       /_//_//_//_//_//_//_//_//_//_//_//_/    / height
  //    y   z                 ___ pitch
  //
  //      +--+       perimeter of and element with + as elements edges
  //     /  /       defined either clockwise or counterclockwise
  //    +--+
  //
  // For more information see : https://github.com/moduleus/uac/wiki
  //
  // Requirements:     urx 1.2.0 toolbox
  //                   uac 1.2.0 toolbox

  // Element's geometry : all the elements share the same geometry
  const std::array<std::array<double, 4>, 3> edges = {
      {{{-width / 2, -width / 2, width / 2, width / 2}},
       {{// x, y z coordinates (dimension 1) of the 4 edges (dimensions 2)
         -height / 2, height / 2, height / 2, -height / 2}},
       {{0, 0, 0, 0}}}};

  const std::shared_ptr<uac::ElementGeometry> element_geometry =
      std::make_shared<uac::ElementGeometry>();
  element_geometry->perimeter = {{edges[0][0], edges[1][0], edges[2][0]},
                                 // perimeter of one element
                                 {edges[0][1], edges[1][1], edges[2][1]},
                                 {edges[0][2], edges[1][2], edges[2][2]},
                                 {edges[0][3], edges[1][3], edges[2][3]}};

  // (Optional) Impulse response : all the elements share the same impulse response
  const std::shared_ptr<uac::ImpulseResponse> impulse_response =
      std::make_shared<uac::ImpulseResponse>();
  impulse_response->sampling_frequency =
      50e6;                           // sampling frequency of the impulse response data [Hz]
  impulse_response->time_offset = 0;  // delay before impulse reponse starts [s]
  impulse_response->units = "N/A";    // unit of the impulse response
  impulse_response->data = {0.0000, 0.0467, 0.1641, 0.2780, 0.2521, 0.0000,
                            // impulse response depending on time
                            -0.4160, -0.7869, -0.8756, -0.5759, -0.0000, 0.5759, 0.8756, 0.7869,
                            0.4160, 0.0000, -0.2521, -0.2780};

  // Build the elements' array
  std::vector<uac::Element> elements;
  elements.reserve(n_elements);
  const double xmin = -pitch * (n_elements - 1) / 2;  // first element center position
  // Loop on the other elements
  // then increments this element with a loop
  for (size_t i = 0; i < n_elements; i++) {
    const double x = xmin + i * pitch;  // computes all the position element
    uac::Element element;
    element.transform.translation = {x, 0, 0};  // elements are only along x-axis
    // element.transform.rotation = {};  // default value, rotation of the element
    element.element_geometry = element_geometry;  // shared geometry between elements
    element.impulse_response = impulse_response;  // shared impulse response between elements
    elements.push_back(element);                  // appends elements to the array
  }

  // Build the probe
  std::shared_ptr<uac::Probe> probe = std::make_shared<uac::Probe>();
  probe->description = description;
  // 0: LINEAR, 1: CURVILINEAR, 2: RCA, 3: MATRIX, 4: SPARSE, -1: UNDEFINED
  probe->type = uac::ProbeType::LINEAR;
  // probe->transform.rotation = {};        // default value,
  // probe->transform.translation = {};     // default value
  // could be an array in case of different impulse responses
  probe->impulse_responses.push_back(impulse_response);
  // could be an array in case of different elements geometries
  probe->element_geometries.push_back(element_geometry);
  // set all the elements once to avoid repetitive call to probe
  probe->elements = elements;

  return probe;
}

std::pair<uac::TransmitSetup, std::shared_ptr<uac::Excitation>> makePlaneWaveTransmitSetupUac(
    double theta, double voltage, size_t n_periods, double transmit_frequency, double max_delay,
    double sound_speed, const std::vector<std::vector<uint32_t>>& channel_mapping,
    const std::shared_ptr<uac::Probe>& probe,
    const std::shared_ptr<uac::Excitation>& excitation_input) {
  /*
    makePlaneWaveTransmitSetupUac creates a plane waves transmit setup with a linear array in uac format
    Inputs:
        theta                       : plane wave angle of the event [°]
        voltage                     : voltage of the excitation [Vpeak]
        n_periods                    : number of periods of transmitted signal
        transmit_frequency           : central frequency of the transmitted signal [Hz]
        max_delay                    : maximum delay of the plane wave sequence [s]
        sound_speed                  : celerity of the sound in the medium [m/s]
        probe (uac.Probe)           : ultrasound probe used for the transmit event
        excitation (uac.Excitation) : (optional) excitation reused from another transmit event
    Outputs:
        TransmitSetup (uac.TransmitSetup)   : built for plane wave transmit event with a linear array probe
        Excitation (uac.Excitation)         : excitation to be used in another transmit event
    Requirements:
        urx 1.2.0 toolbox
        uac 1.2.0 toolbox
    More information:
        see : https://github.com/moduleus/uac/wiki
    */
  // Checks
  if (probe->type != uac::ProbeType::LINEAR) {
    throw std::invalid_argument("Wrong type of probe");
  }

  // Excitation parameters
  const double excitation_post_clamp_duration = 900e-9;
  const double excitation_sampling_frequency = 200e6;

  // Delay calculation
  const uint32_t n_elements = static_cast<uint32_t>(probe->elements.size());
  const double pitch =
      probe->elements[1].transform.translation.x - probe->elements[0].transform.translation.x;

  std::vector<double> delays(n_elements);
  for (uint32_t i = 0; i < n_elements; ++i) {
    const double element_index = i - (n_elements - 1) / 2.0;
    delays[i] = std::sin(theta * M_PI / 180.0) * element_index * pitch / sound_speed;
  }

  // Wave
  uac::Wave wave;
  wave.type = uac::WaveType::PLANE_WAVE;
  wave.parameters = {std::sin(theta * M_PI / 180.0), 0.0, std::cos(theta * M_PI / 180.0)};
  wave.time_zero = max_delay + n_periods / transmit_frequency / 2.0;

  // Excitation
  std::shared_ptr<uac::Excitation> excitation;
  if (excitation_input == nullptr) {
    excitation = std::make_shared<uac::Excitation>();
    excitation->pulse_shape = "square wave";
    excitation->transmit_frequency = transmit_frequency;
    excitation->sampling_frequency = excitation_sampling_frequency;

    const size_t n_samples_excitation =
        static_cast<size_t>(std::lround(excitation_sampling_frequency / transmit_frequency / 2.0));
    const double real_freq = excitation_sampling_frequency / n_samples_excitation / 2.0;
    if (std::abs(real_freq - transmit_frequency) > 1e-6) {
      std::cout << "(Warning) Real transmit frequency: " << real_freq << " Hz instead of "
                << transmit_frequency << "Hz\n";
    }

    const size_t n_post_clamp = static_cast<size_t>(
        std::lround(excitation_sampling_frequency * excitation_post_clamp_duration));

    // Generate waveform
    std::vector<double> waveform;
    for (size_t period = 0; period < n_periods; ++period) {
      for (size_t i = 0; i < n_samples_excitation; ++i) {
        waveform.push_back(voltage);
      }
      for (size_t i = 0; i < n_samples_excitation; ++i) {
        waveform.push_back(-voltage);
      }
    }
    for (size_t i = 0; i < n_post_clamp; ++i) {
      waveform.push_back(0.0);
    }

    excitation->waveform = waveform;
  } else {
    excitation = excitation_input;
  }

  // TransmitSetup
  uac::TransmitSetup transmit_setup;
  transmit_setup.wave = wave;

  // Active elements
  transmit_setup.active_elements = channel_mapping;
  for (uint32_t i = 0; i < n_elements; ++i) {
    transmit_setup.excitations.push_back(excitation);
  }

  // Add max_delay to delays
  for (double& delay : delays) {
    delay += max_delay;
  }
  transmit_setup.delays = delays;
  transmit_setup.probe = probe;

  return {transmit_setup, excitation};
}

uac::ReceiveSetup makeReceiveSetupUac(uint32_t n_samples, double rx_time_offset,
                                      double sampling_frequency, double modulation_frequency,
                                      const std::vector<std::vector<uint32_t>>& channel_mapping,
                                      const std::shared_ptr<uac::Probe>& probe) {
  // ReceiveSetup
  uac::ReceiveSetup receive_setup;
  receive_setup.sampling_frequency = sampling_frequency;
  receive_setup.modulation_frequency = modulation_frequency;
  receive_setup.number_samples = n_samples;
  receive_setup.active_elements =
      channel_mapping;  // active elements (arrays dimension) per excitations (cells dimension)
  receive_setup.probe = probe;
  // receive_setup.probe_transform.translation = {};  // default value, position of the probe for the reception event
  // receive_setup.probe_transform.rotation = {};     // default value, rotation of the probe for the reception event
  // receive_setup.tgc_profile;            // Time Gain Compensation profile
  receive_setup.tgc_sampling_frequency = 0;    // sampling of the TGC profile [Hz]
  receive_setup.time_offset = rx_time_offset;  // delay before sampling [s]
  // receive_setup.hwConfig                                      // configuration of the hardware

  return receive_setup;
}

std::pair<std::shared_ptr<uac::Group>, std::shared_ptr<uac::Excitation>> makePlaneWavesGroupUac(
    const std::vector<double>& thetas, double voltage, size_t n_periods, double prf,
    uint32_t n_frames, double frame_rate, double transmit_frequency, double sound_speed,
    uint32_t n_samples, double rx_time_offset, double sampling_frequency,
    double modulation_frequency, const std::vector<std::vector<uint32_t>>& channel_mapping,
    const std::shared_ptr<uac::Probe>& probe) {
  // Maximum negative delay of the sequence
  const uint32_t n_elements = static_cast<uint32_t>(probe->elements.size());

  // Calculate pitch (assuming probe->elements has transform.translation.x)
  const double pitch = probe->elements[1].transform.translation.x -
                       probe->elements[0].transform.translation.x;  // pitch of the probe [m]

  // Find maximum absolute theta
  double theta_max = std::numeric_limits<double>::lowest();
  for (const double theta : thetas) {
    theta_max = std::max(theta_max, std::abs(theta));
  }

  // Calculate maximum negative delay
  double min_delay = std::numeric_limits<double>::max();
  for (uint32_t i = 0; i < n_elements; ++i) {
    const double element_pos = (i - (n_elements - 1) / 2.0);
    const double delay = std::sin(theta_max * M_PI / 180.0) * element_pos * pitch / sound_speed;
    min_delay = std::min(min_delay, delay);
  }
  const double max_delays = -min_delay;  // maximum negative delay of the plane wave sequence [s]

  // Build the sequence of events
  std::vector<uac::Event> sequence;

  // Define the first event and excitation of the sequence
  uac::Event event;
  auto [transmit_setup, excitation] =
      makePlaneWaveTransmitSetupUac(thetas[0], voltage, n_periods, transmit_frequency, max_delays,
                                    sound_speed, channel_mapping, probe);

  uac::ReceiveSetup receive_setup = makeReceiveSetupUac(
      n_samples, rx_time_offset, sampling_frequency, modulation_frequency, channel_mapping, probe);

  event.transmit_setup = transmit_setup;
  event.receive_setup = std::move(receive_setup);
  sequence.push_back(event);

  // Increment the events of this sequence with the loop and reuse the excitation
  for (size_t i = 1; i < thetas.size(); ++i) {
    uac::Event event;
    const auto [transmit_setup_i, excitation_i] =
        makePlaneWaveTransmitSetupUac(thetas[i], voltage, n_periods, transmit_frequency, max_delays,
                                      sound_speed, channel_mapping, probe, excitation);

    uac::ReceiveSetup receive_setup =
        makeReceiveSetupUac(n_samples, rx_time_offset, sampling_frequency, modulation_frequency,
                            channel_mapping, probe);

    event.transmit_setup = transmit_setup_i;
    event.receive_setup = std::move(receive_setup);
    event.time_offset = i / prf;
    sequence.push_back(event);
  }

  // Group
  const std::shared_ptr<uac::Group> group =
      std::make_shared<uac::Group>();           // defines the group of the sequence
  group->description = "Group of plane waves";  // group name or description
  group->sound_speed = sound_speed;
  group->sequence = sequence;  // set all the events of the sequence
  group->time_offset =
      0.0;  // test 0, normally must be bigger than the Rx Analog reconf (ex: 20e-6)
  group->repetition_count = n_frames;
  group->period = 1.0 / frame_rate;
  group->data_type = urx::DataType::INT16;  // 0: INT16, 1: INT32, 2:FLOAT, 3:DOUBLE, -1: UNDEFINED
  group->sampling_type = urx::SamplingType::RF;  // 0: RF, 1: IQ, -1: UNDEFINED

  return {group, excitation};
}
