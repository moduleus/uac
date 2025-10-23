#include <ios>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/double_nan.h>
#include <urx/excitation.h>
#include <urx/utils/io/reader.h>
#include <urx/utils/io/writer.h>
#include <urx/utils/io/writer_options.h>

int main(int argc, char** argv) {
  // read the input file
  const std::shared_ptr<urx::Dataset> dataset =
      urx::utils::io::reader::loadFromFile(std::string(TUTORIAL_SOURCE_PATH) + "/../data/rca.urx");

  // print values
  std::cout << dataset->acquisition.description << "\n";
  std::cout << dataset->acquisition.excitations[0]->sampling_frequency << "\n";

  // change values
  dataset->acquisition.description = "Learning to modify values ...";

  urx::utils::io::writer::saveToFile("rca-cpp.urx", *dataset, {false, false, false});

  return 0;
}
