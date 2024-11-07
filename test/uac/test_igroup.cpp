#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <uac/acquisition.h>
#include <uac/destination_link.h>
#include <uac/group.h>
#include <uac/igroup.h>
#include <uac/super_group.h>

namespace uac::test {

TEST_CASE("Test IGroup", "[test_igroup]") {
  Acquisition acq;
  auto group = std::make_shared<Group>();
  DestinationLink destination_link;
  destination_link.destination = group;
  group->destinations.push_back(std::move(destination_link));
  acq.groups.push_back(group);

  Acquisition acq2;
  auto group2 = std::make_shared<Group>();
  DestinationLink destination_link2;
  destination_link2.destination = group2;
  group2->destinations.push_back(std::move(destination_link2));
  acq2.groups.push_back(group2);

  CHECK(acq == acq2);

  SuperGroup su;
  Group g;

  IGroup* isu = &su;
  IGroup* ig = &g;

  CHECK(*isu != *ig);
  CHECK(*ig != *isu);
}

}  // namespace uac::test
