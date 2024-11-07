
#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "io.h"
#include <urx/dataset.h>

#include <uac/acquisition.h>
#include <uac/dataset.h>
#include <uac/destination_link.h>
#include <uac/group.h>
#include <uac/super_group.h>
#include <uac/utils/io/reader.h>
#include <uac/utils/io/writer.h>

namespace uac::utils::io::test {

TEST_CASE("Write HDF5 file", "[hdf5_writer]") {
  auto dataset = uac::utils::io::test::generateFakeDataset<Dataset>();

  writer::saveToFile("writer康🐜.uac", *dataset);

  auto dataset_loaded = reader::loadFromFile("writer康🐜.uac");

  REQUIRE(*dataset_loaded == *dataset);
}

TEST_CASE("Destination points to futur group / super group", "[hdf5_reader]") {
  auto dataset = std::make_shared<uac::Dataset>();

  dataset->acquisition.groups.push_back(std::make_shared<uac::Group>());
  dataset->acquisition.groups.push_back(std::make_shared<uac::Group>());

  dataset->acquisition.groups[0]->destinations.emplace_back();
  dataset->acquisition.groups[0]->destinations[0].destination = dataset->acquisition.groups[1];

  dataset->acquisition.super_groups.push_back(std::make_shared<uac::SuperGroup>());

  dataset->acquisition.groups[1]->destinations.emplace_back();
  dataset->acquisition.groups[1]->destinations[0].destination =
      dataset->acquisition.super_groups[0];

  writer::saveToFile("futur_group.uac", *dataset);

  auto dataset_loaded = reader::loadFromFile("futur_group.uac");

  REQUIRE(*dataset_loaded == *dataset);
}

}  // namespace uac::utils::io::test
