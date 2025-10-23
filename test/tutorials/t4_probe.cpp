#include "helpers.h"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <urx/utils/io/writer_options.h>

#include <uac/acquisition.h>
#include <uac/dataset.h>
#include <uac/utils/io/writer.h>

int main(int argc, char** argv) {
  // Probe parameters
  const std::string probe_description = "SL10-2 192 elements";  // description or name of the probe
  const size_t n_elements = 192;  // number of transducers in the ultrasound probe
  const double pitch = 300e-6;    // pitch (spatial step) of the array [m]
  const double width = 280e-6;    // width of a single transducer [m]
  const double height = 6e-3;     // height of a single transducer [m]

  // Probe definition
  const auto probe =
      makeLinearArrayUac(n_elements, pitch, width, height, probe_description);  // build the probe

  // Saves to file
  uac::Dataset dataset;
  dataset.acquisition.probes.push_back(probe);
  // saves the probe in a dataset
  uac::utils::io::writer::saveToFile("./t4-cpp.uac", dataset, {false, false, false});
}
