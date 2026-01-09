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
  CHECK(!(acq != acq2));
  CHECK(*group == *group2);
  CHECK(!(*group != *group2));
  CHECK(group->destinations[0] == group2->destinations[0]);
  CHECK(!(group->destinations[0] != group2->destinations[0]));

  auto group3 = std::make_shared<Group>();
  DestinationLink destination_link3;
  destination_link3.destination = group3;
  group3->destinations.push_back(std::move(destination_link3));

  auto group4 = std::make_shared<SuperGroup>();
  DestinationLink destination_link4;
  destination_link4.destination = group4;
  group4->destinations.push_back(std::move(destination_link4));

  CHECK(group3->destinations[0] != group4->destinations[0]);
  CHECK(!(group3->destinations[0] == group4->destinations[0]));

  SuperGroup su;
  Group g;

  IGroup* isu = &su;
  IGroup* ig = &g;

  CHECK(*isu != *ig);
  CHECK(*ig != *isu);
  CHECK(!(*isu == *ig));
  CHECK(!(*ig == *isu));

  CHECK(su != *ig);
  CHECK(g != *isu);
  CHECK(!(su == *ig));
  CHECK(!(g == *isu));

  CHECK(*isu != g);
  CHECK(*ig != su);
  CHECK(!(*isu == g));
  CHECK(!(*ig == su));

  auto group5 = std::make_shared<Group>();
  auto group5_2 = std::make_shared<Group>();
  DestinationLink destination_link5;
  destination_link5.destination = group5_2;
  group5->destinations.push_back(std::move(destination_link5));

  auto group6 = std::make_shared<Group>();
  auto group6_2 = std::make_shared<SuperGroup>();
  DestinationLink destination_link6;
  destination_link6.destination = group6_2;
  group6->destinations.push_back(std::move(destination_link6));

  CHECK(group5->destinations[0] != group6->destinations[0]);
  CHECK(!(group5->destinations[0] == group6->destinations[0]));

  auto group7 = std::make_shared<Group>();
  auto group7_2 = std::make_shared<Group>();
  DestinationLink destination_link7;
  destination_link7.destination = group7_2;
  group7->destinations.push_back(std::move(destination_link7));

  auto group8 = std::make_shared<Group>();
  auto group8_2 = std::make_shared<Group>();
  DestinationLink destination_link8;
  destination_link8.destination = group8_2;
  group8->destinations.push_back(std::move(destination_link8));

  CHECK(group7->destinations[0] == group8->destinations[0]);
  CHECK(!(group7->destinations[0] != group8->destinations[0]));
}

}  // namespace uac::test
