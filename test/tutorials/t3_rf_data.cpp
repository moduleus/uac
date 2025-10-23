#include <cstddef>
#include <ios>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/group_data.h>
#include <urx/utils/group_data_reader.h>
#include <urx/utils/io/reader.h>

int main(int argc, char** argv) {
  // read the input file
  const std::shared_ptr<urx::Dataset> dataset = urx::utils::io::reader::loadFromFile(
      std::string(TUTORIAL_SOURCE_PATH) + "/../data/single_int16_rf.urx");

  const size_t gid = 0;

  const auto group_data_reader = urx::utils::GroupDataReader{dataset->acquisition.groups_data[gid]};

  std::cout << "Samples count: " << group_data_reader.samplesCount(0) << "\n";
  std::cout << "Channels count: " << group_data_reader.channelsCount(0) << "\n";
  std::cout << "Events count: " << group_data_reader.eventsCount() << "\n";
  std::cout << "Sequences count: " << group_data_reader.sequencesCount() << "\n";

  return 0;
}
