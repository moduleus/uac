#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <urx/dataset.h>
#include <urx/detail/compare.h>
#include <urx/probe.h>
#include <urx/transform.h>
#include <urx/utils/exception.h>
#include <urx/utils/io/writer_options.h>

#include <uac/acquisition.h>
#include <uac/dataset.h>
#include <uac/destination_link.h>
#include <uac/event.h>
#include <uac/excitation.h>
#include <uac/group.h>
#include <uac/receive_setup.h>
#include <uac/super_group.h>
#include <uac/transmit_setup.h>
#include <uac/utils/io/reader.h>
#include <uac/utils/io/writer.h>
#include <uac/utils/test/dataset_gen.h>

namespace uac::utils::io::test {

TEST_CASE("Write HDF5 file", "[hdf5_writer][hdf5_reader]") {
  auto dataset = uac::utils::test::generateFakeDataset<Dataset>();

  const bool clean_unusable_data = GENERATE(true, false);
  const bool check_data = GENERATE(true, false);

  urx::utils::io::WriterOptions options;

  options.setCleanUnusableData(clean_unusable_data);
  options.setCheckData(check_data);

  const std::string filename = "writer康🐜.uac";

  if (check_data) {
    REQUIRE_THROWS_AS(writer::saveToFile(filename, *dataset, options), std::runtime_error);
    return;
  }
  writer::saveToFile(filename, *dataset, options);

  auto dataset_loaded = reader::loadFromFile(filename);

  REQUIRE(dataset_loaded->acquisition.probes != dataset->acquisition.probes);
  REQUIRE(dataset_loaded->acquisition.probes.size() == dataset->acquisition.probes.size());
  for (size_t i = 0; i < dataset_loaded->acquisition.probes.size(); ++i) {
    REQUIRE(*dataset_loaded->acquisition.probes.at(i) == *dataset->acquisition.probes.at(i));
  }
  REQUIRE(urx::valueComparison(dataset_loaded->acquisition.probes, dataset->acquisition.probes));

  REQUIRE(dataset_loaded->acquisition.excitations != dataset->acquisition.excitations);
  REQUIRE(dataset_loaded->acquisition.excitations.size() ==
          dataset->acquisition.excitations.size());
  for (size_t i = 0; i < dataset_loaded->acquisition.excitations.size(); ++i) {
    REQUIRE(*dataset_loaded->acquisition.excitations.at(i) ==
            *dataset->acquisition.excitations.at(i));
  }
  REQUIRE(urx::valueComparison(dataset_loaded->acquisition.excitations,
                               dataset->acquisition.excitations));

  REQUIRE(dataset_loaded->acquisition.groups != dataset->acquisition.groups);
  REQUIRE(dataset_loaded->acquisition.groups.size() == dataset->acquisition.groups.size());
  for (size_t i = 0; i < dataset_loaded->acquisition.groups.size(); ++i) {
    REQUIRE(*dataset_loaded->acquisition.groups.at(i) == *dataset->acquisition.groups.at(i));
  }
  REQUIRE(urx::valueComparison(dataset_loaded->acquisition.groups, dataset->acquisition.groups));

  REQUIRE(*dataset_loaded == *dataset);
}

TEST_CASE("Write HDF5 file containing unusable data", "[hdf5_writer][hdf5_reader]") {
  auto dataset = utils::test::generateFakeDataset<Dataset>();

  auto clean_unusable_data = GENERATE(true, false);
  auto check_data = GENERATE(true, false);

  urx::utils::io::WriterOptions options;

  options.setCleanUnusableData(clean_unusable_data);
  options.setCheckData(check_data);

  const std::string filename = "writer康🐜.uac";

  REQUIRE(dataset->acquisition.groups.back()->sequence.back().transmit_setup.probe.expired());
  REQUIRE(dataset->acquisition.groups.back()->sequence.back().receive_setup.probe.expired());

  dataset->acquisition.groups.back()->sequence.back().transmit_setup.excitations = {
      dataset->acquisition.excitations[0]};
  dataset->acquisition.groups.back()->sequence.back().transmit_setup.active_elements = {{1}};
  dataset->acquisition.groups.back()->sequence.back().transmit_setup.delays = {1.6};
  dataset->acquisition.groups.back()->sequence.back().transmit_setup.probe_transform = {
      {5.1e-120, -8, 7.}, {5.2, 4.3, 8.2e10}};

  dataset->acquisition.groups.back()->sequence.back().receive_setup.active_elements = {{1}};
  dataset->acquisition.groups.back()->sequence.back().receive_setup.probe_transform = {
      {5.1e-120, -8, 7.}, {5.2, 4.3, 8.2e10}};

  if (check_data) {
    REQUIRE_THROWS_AS(writer::saveToFile(filename, *dataset, options), std::runtime_error);
    return;
  }
  writer::saveToFile(filename, *dataset, options);

  const std::shared_ptr<Dataset> dataset_loaded = reader::loadFromFile(filename);

  REQUIRE(dataset_loaded->acquisition.probes != dataset->acquisition.probes);
  REQUIRE(dataset_loaded->acquisition.probes.size() == dataset->acquisition.probes.size());
  for (size_t i = 0; i < dataset_loaded->acquisition.probes.size(); ++i) {
    REQUIRE(*dataset_loaded->acquisition.probes.at(i) == *dataset->acquisition.probes.at(i));
  }
  REQUIRE(urx::valueComparison(dataset_loaded->acquisition.probes, dataset->acquisition.probes));

  REQUIRE(dataset_loaded->acquisition.excitations != dataset->acquisition.excitations);
  REQUIRE(dataset_loaded->acquisition.excitations.size() ==
          dataset->acquisition.excitations.size());
  for (size_t i = 0; i < dataset_loaded->acquisition.excitations.size(); ++i) {
    REQUIRE(*dataset_loaded->acquisition.excitations.at(i) ==
            *dataset->acquisition.excitations.at(i));
  }
  REQUIRE(urx::valueComparison(dataset_loaded->acquisition.excitations,
                               dataset->acquisition.excitations));

  if (clean_unusable_data) {
    REQUIRE(*dataset_loaded != *dataset);

    const auto& ts = dataset_loaded->acquisition.groups.back()->sequence.back().transmit_setup;
    REQUIRE(ts.excitations.empty());
    REQUIRE(ts.active_elements.empty());
    REQUIRE(ts.delays.empty());
    REQUIRE(ts.probe_transform == decltype(ts.probe_transform)());

    const auto& rs = dataset_loaded->acquisition.groups.back()->sequence.back().receive_setup;
    REQUIRE(rs.active_elements.empty());
    REQUIRE(rs.probe_transform == decltype(rs.probe_transform)());
  } else {
    REQUIRE(*dataset_loaded == *dataset);

    REQUIRE(dataset_loaded->acquisition.groups.size() == dataset->acquisition.groups.size());
    for (size_t i = 0; i < dataset_loaded->acquisition.groups.size(); ++i) {
      REQUIRE(*dataset_loaded->acquisition.groups.at(i) == *dataset->acquisition.groups.at(i));
    }
  }
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

  urx::utils::io::WriterOptions options;

  options.setCleanUnusableData(false);
  options.setCheckData(false);

  writer::saveToFile("futur_group.uac", *dataset, options);

  auto dataset_loaded = reader::loadFromFile("futur_group.uac");

  REQUIRE(*dataset_loaded == *dataset);
}

TEST_CASE("Read failure HDF5 file", "[hdf5_reader]") {
  REQUIRE_THROWS_AS(reader::loadFromFile("missing_file.uac"), urx::utils::ReadFileException);
}

TEST_CASE("Write failure HDF5 file", "[hdf5_writer]") {
  const uac::Dataset dataset;
#ifdef _WIN32
  REQUIRE_THROWS_AS(writer::saveToFile("aux", dataset), std::runtime_error);
  urx::utils::io::WriterOptions options;
  options.setCheckData(false);
  REQUIRE_THROWS_AS(writer::saveToFile("aux", dataset, options), urx::utils::WriteFileException);
#else
  REQUIRE_THROWS_AS(writer::saveToFile("/", dataset), std::runtime_error);
  urx::utils::io::WriterOptions options;
  options.setCheckData(false);
  REQUIRE_THROWS_AS(writer::saveToFile("/", dataset, options), urx::utils::WriteFileException);
#endif
}

}  // namespace uac::utils::io::test
