#include "helpers.h"

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

int main() {
  // Receive parameters
  const double sampling_frequency = 33.3e6;  // sampling frequency of the backscattered signal [Hz]
  const double modulation_frequency =
      8.33e6;                            // modulation frequency of the backscattered signal [Hz]
  const int n_samples = 800;             // number of received samples for each transmit event
  const double rx_time_offset = 1.2e-6;  // time before the sampling of the received signal [s]

  // Probe parameters
  const std::string probe_description = "SL10-2 192 elements";  // description or name of the probe
  const int n_elements = 192;         // number of transducers in the ultrasound probe
  const double pitch = 300e-6;        // pitch (spatial step) of the array [m]
  const double width = 280e-6;        // width of a single transducer [m]
  const double height = 6e-3;         // height of a single transducer [m]
  const double kerf = pitch - width;  // space between adjacent transducer elements [m]

  // Create channel mapping vector (equivalent to range(NElements))
  std::vector<std::vector<uint32_t>> channel_mapping(n_elements);
  for (uint32_t i = 0; i < n_elements; ++i) {
    channel_mapping[i] = {i};
  }

  // Probe
  auto probe =
      makeLinearArrayUac(n_elements, pitch, width, height, probe_description);  // build the probe

  // Receive
  auto receive_setup =
      makeReceiveSetupUac(n_samples, rx_time_offset, sampling_frequency, modulation_frequency,
                          channel_mapping, probe);  // build the receive setup
  return 0;
}