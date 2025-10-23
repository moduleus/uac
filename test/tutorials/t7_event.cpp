#include "helpers.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

int main() {
  // Transmit parameters
  const size_t n_theta = 4;                               // number of plane waves in the sequence
  const size_t d_theta = 4;                               // angular step between plane waves [°]
  const double theta_max = (n_theta - 1) / 2. * d_theta;  // maximum plane wave angle [°]
  std::vector<double> thetas;
  thetas.reserve(n_theta);
  for (size_t i = 0; i < n_theta; ++i) {
    thetas.push_back(-theta_max + i * d_theta);
  }
  const double transmit_frequency = 8.33e6;  // transmit frequency of the pulse [Hz]
  const double voltage = 25.0;               // voltage of the excitation Vpeak [V]
  const size_t n_periods = 2;                // number of transmitted periods
  const size_t n_frames = 1;                 // number of frames
  const double frame_rate = 30.0;            // frame rate after compound [Hz]
  const double prf = 10e3;                   // pulse repetition rate [Hz]
  const double sound_speed = 1540.0;         // celerity of the sound in the medium [m/s]
  // Receive parameters
  const double sampling_frequency = 33.3e6;  // sampling frequency of the backscattered signal [Hz]
  const double modulation_frequency =
      8.33e6;                            // modulation frequency of the backscattered signal [Hz]
  const int n_samples = 800;             // number of received samples for each transmit event
  const double rx_time_offset = 1.2e-6;  // time before the sampling of the received signal [s]

  // Probe parameters
  const std::string probe_description = "SL10-2 192 elements";  // description or name of the probe
  const size_t n_elements = 192;      // number of transducers in the ultrasound probe
  const double pitch = 300e-6;        // pitch (spatial step) of the array [m]
  const double width = 280e-6;        // width of a single transducer [m]
  const double height = 6e-3;         // height of a single transducer [m]
  const double kerf = pitch - width;  // space between adjacent transducer elements [m]
  std::vector<std::vector<uint32_t>> channel_mapping(n_elements);
  for (uint32_t i = 0; i < n_elements; ++i) {
    channel_mapping[i] = {i};
  }

  // Probe
  const auto probe = makeLinearArrayUac(n_elements, pitch, width, height, probe_description);

  // Sequence
  const auto [group, excitation] = makePlaneWavesGroupUac(
      thetas, voltage, n_periods, prf, n_frames, frame_rate, transmit_frequency, sound_speed,
      n_samples, rx_time_offset, sampling_frequency, modulation_frequency, channel_mapping, probe);
}
