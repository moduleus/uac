#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <urx/config.h>
#include <urx/dataset.h>

#include <uac/dataset.h>
#include <uac/utils/test/dataset_gen.h>
#include <uac/utils/to_urx.h>

#ifdef URX_WITH_HDF5
#include <urx/utils/io/reader.h>
#include <urx/utils/io/writer_options.h>

#include <uac/utils/io/writer.h>
#else
#include <iostream>
#endif

namespace uac::utils::test {

TEST_CASE("Uac to Urx", "[utils]") {
  auto dataset_uac = uac::utils::test::generateFakeDataset<uac::Dataset>();

  auto dataset_urx = uac::utils::toUrx(*dataset_uac);

#ifdef URX_WITH_HDF5
  urx::utils::io::WriterOptions options;
  options.setCheckData(false);
  uac::utils::io::writer::saveToFile("dataset_to_urx.uac", *dataset_uac, options);

  auto dataset_loaded = urx::utils::io::reader::loadFromFile("dataset_to_urx.uac");

  REQUIRE(*dataset_urx == *dataset_loaded);
#else
  std::cout << "Enable HDF5 to check if uac::utils::toUrx and uac::utils::io::writer::saveToFile / "
               "urx::utils::io::reader::loadFromFile have the same behavior.\n";
#endif
}

}  // namespace uac::utils::test
