#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <urx/dataset.h>
#include <urx/probe.h>
#include <urx/utils/validator.h>

#include <uac/acquisition.h>
#include <uac/dataset.h>
#include <uac/probe.h>
#include <uac/utils/test/dataset_gen.h>
#include <uac/utils/validator.h>

namespace uac::utils::validator::test {

using UrxValidatorReport = urx::utils::ValidatorReportBase<urx::utils::ValidatorBase, Dataset>;

TEST_CASE("Test Dataset check with urx Validator", "[dataset][helper]") {
  std::shared_ptr<Dataset> dataset = std::make_shared<Dataset>();
  dataset->acquisition.probes = {std::make_shared<Probe>(), std::make_shared<Probe>()};

  UrxValidatorReport validator;
  validator.check(*dataset);

  REQUIRE(validator.getOutput().size() == 9);

  dataset = utils::test::generateFakeDataset<Dataset>();
  validator.check(*dataset);

  REQUIRE(validator.getOutput().size() == 67);

  dataset = utils::test::generateWrongDataset<Dataset>();
  validator.check(*dataset);

  REQUIRE(validator.getOutput().size() == 150);
}

TEST_CASE("Test Dataset check with uac Validator", "[dataset][helper]") {
  std::shared_ptr<Dataset> dataset = std::make_shared<Dataset>();
  dataset->acquisition.probes = {std::make_shared<Probe>(), std::make_shared<Probe>()};

  ValidatorReport validator;
  validator.check(*dataset);

  REQUIRE(validator.getOutput().size() == 10);

  dataset = utils::test::generateFakeDataset<Dataset>();
  validator.check(*dataset);

  REQUIRE(validator.getOutput().size() == 81);

  dataset = utils::test::generateWrongDataset<Dataset>();
  validator.check(*dataset);

  REQUIRE(validator.getOutput().size() == 164);
}

}  // namespace uac::utils::validator::test
