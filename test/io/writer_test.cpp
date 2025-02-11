
#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "io.h"
#include <urx/dataset.h>
#include <urx/utils/exception.h>

#include <uac/acquisition.h>
#include <uac/dataset.h>
#include <uac/destination_link.h>
#include <uac/group.h>
#include <uac/super_group.h>
#include <uac/uac.h>
#include <uac/utils/io/reader.h>
#include <uac/utils/io/writer.h>
#include <uac/version.h>

namespace uac::utils::io::test {

TEST_CASE("Write HDF5 file", "[hdf5_writer][hdf5_reader]") {
  auto dataset = uac::utils::io::test::generateFakeDataset<Dataset>();
  dataset->version.major = uac::UAC_VERSION_MAJOR;
  dataset->version.minor = uac::UAC_VERSION_MINOR;
  dataset->version.patch = uac::UAC_VERSION_PATCH;

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

TEST_CASE("Read failure HDF5 file", "[hdf5_reader]") {
  REQUIRE_THROWS_AS(reader::loadFromFile("missing_file.uac"), urx::utils::ReadFileException);
}

TEST_CASE("Write failure HDF5 file", "[hdf5_writer]") {
  const uac::Dataset dataset;
#ifdef _WIN32
  REQUIRE_THROWS_AS(writer::saveToFile("aux", dataset), urx::utils::WriteFileException);
#else
  REQUIRE_THROWS_AS(writer::saveToFile("/", dataset), urx::utils::WriteFileException);
#endif
}

}  // namespace uac::utils::io::test
