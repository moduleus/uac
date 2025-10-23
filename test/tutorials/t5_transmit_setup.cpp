#include "helpers.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <urx/probe.h>

#include <uac/probe.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433
#endif

int main() {
  // Transmit parameters
  const double theta = 4.0;
  const double theta_max = 6.0;
  const double transmit_frequency = 8.33e6;
  const double voltage = 25.0;
  const int n_periods = 2;
  const double sound_speed = 1540.0;

  // Probe parameters
  const std::string probe_description = "SL10-2 192 elements";
  const int n_elements = 192;
  const double pitch = 300e-6;
  const double width = 280e-6;
  const double height = 6e-3;
  const double kerf = pitch - width;
  std::vector<std::vector<uint32_t>> channel_mapping(n_elements);
  for (uint32_t i = 0; i < n_elements; ++i) {
    channel_mapping[i] = {i};
  }
  // Create probe
  const std::shared_ptr<uac::Probe> probe =
      makeLinearArrayUac(n_elements, pitch, width, height, probe_description);

  // Calculate max delays
  double max_delays = 0.0;
  for (int i = 0; i < n_elements; ++i) {
    const double element_index = i - (n_elements - 1) / 2.0;
    const double delay = std::sin(theta_max * M_PI / 180.0) * element_index * pitch / sound_speed;
    max_delays = std::min(max_delays, delay);
  }
  max_delays = -max_delays;

  // Create transmit setup
  auto [transmit_setup, excitation] =
      makePlaneWaveTransmitSetupUac(theta, voltage, n_periods, transmit_frequency, max_delays,
                                    sound_speed, channel_mapping, probe);

  return 0;
}