#include "clone_generic_test.h"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <urx/dataset.h>
#include <urx/detail/compare.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/transform.h>
#include <urx/utils/common.h>
#include <urx/vector.h>
#include <urx/wave.h>

#include <uac/acquisition.h>
#include <uac/dataset.h>
#include <uac/destination_link.h>
#include <uac/element.h>
#include <uac/element_geometry.h>
#include <uac/event.h>
#include <uac/excitation.h>
#include <uac/group.h>
#include <uac/igroup.h>
#include <uac/impulse_response.h>
#include <uac/probe.h>
#include <uac/receive_setup.h>
#include <uac/super_group.h>
#include <uac/transform.h>
#include <uac/transmit_setup.h>
#include <uac/trigger.h>
#include <uac/utils/detail/clone.h>
#include <uac/utils/test/dataset_gen.h>
#include <uac/vector.h>  // IWYU pragma: keep
#include <uac/version.h>
#include <uac/wave.h>

namespace uac::utils::test {

TEST_CASE("Clone std::weak_ptr<IGroup>", "[Clone]") {
  const std::shared_ptr<Group> ptr = std::make_shared<Group>();
  const std::weak_ptr<IGroup> ig_ptr = ptr;
  const std::weak_ptr<IGroup> ig_ptr_cloned = uac::utils::detail::clone(ig_ptr);
  REQUIRE(urx::valueComparison(ig_ptr_cloned, ig_ptr));
  REQUIRE(urx::valueComparison(ig_ptr_cloned.lock(), ig_ptr.lock()));
  REQUIRE(urx::valueComparison(ig_ptr_cloned.lock().get(), ig_ptr.lock().get()));
  REQUIRE(*ig_ptr_cloned.lock() == *ig_ptr.lock());
}

TEST_CASE("Clone Vector2D", "[Clone]") {
  const uac::Vector2D<double> d_vec = {1.23, 4.56};
  genericCloneTest(d_vec, false);
}

TEST_CASE("Clone Vector3D", "[Clone]") {
  const uac::Vector3D<double> d_vec = {1.23, 4.56, -7.89};
  genericCloneTest(d_vec, false);
}

TEST_CASE("Clone Version", "[Clone]") {
  const uac::Version vers = {1, 2, 3};
  genericCloneTest(vers, false);
}

TEST_CASE("Clone Transform", "[Clone]") {
  const Transform t = {{1, 2, 3}, {4, 5, 6}};
  genericCloneTest(t, false);
}

TEST_CASE("Clone Wave", "[Clone]") {
  genericCloneTest(Wave(), true);
  const auto d = uac::utils::test::generateWrongDataset<uac::Dataset>();

  for (const auto& group : d->acquisition.groups) {
    if (group != nullptr) {
      for (auto& e : group->sequence) {
        const Wave& w = e.transmit_setup.wave;
        genericCloneTest(w, true);
      }
    }
  }
}

TEST_CASE("Clone Element", "[Clone]") {
  genericCloneTest(Element(), false);
  const auto d = utils::test::generateWrongDataset<uac::Dataset>();

  for (const auto& probe : d->acquisition.probes) {
    if (probe != nullptr) {
      for (auto& e : probe->elements) {
        genericCloneTest(e, false);
      }
    }
  }
}

TEST_CASE("Clone ElementGeometry", "[Clone]") {
  genericCloneTest(ElementGeometry(), true);
  const auto d = utils::test::generateWrongDataset<uac::Dataset>();

  for (const auto& probe : d->acquisition.probes) {
    if (probe != nullptr) {
      for (const auto& eg : probe->element_geometries) {
        if (eg) {
          genericCloneTest(*eg, true);
        }
      }
    }
  }
}

TEST_CASE("Clone ImpulseResponse", "[Clone]") {
  genericCloneTest(ImpulseResponse(), true);
  const auto d = utils::test::generateWrongDataset<uac::Dataset>();

  for (const auto& probe : d->acquisition.probes) {
    if (probe != nullptr) {
      for (const auto& ir : probe->impulse_responses) {
        if (ir) {
          genericCloneTest(*ir, true);
        }
      }
    }
  }
}

TEST_CASE("Clone TransmitSetup", "[Clone]") {
  genericCloneTest(TransmitSetup(), false);
  const auto d = utils::test::generateWrongDataset<uac::Dataset>();

  for (const auto& group : d->acquisition.groups) {
    if (group != nullptr) {
      for (const auto& e : group->sequence) {
        const TransmitSetup& ts = e.transmit_setup;
        genericCloneTest(ts, false);
      }
    }
  }
}

TEST_CASE("Clone ReceiveSetup", "[Clone]") {
  genericCloneTest(ReceiveSetup(), false);
  const auto d = utils::test::generateWrongDataset<uac::Dataset>();

  for (const auto& group : d->acquisition.groups) {
    if (group != nullptr) {
      for (const auto& e : group->sequence) {
        const ReceiveSetup& rs = e.receive_setup;
        genericCloneTest(rs, false);
      }
    }
  }
}

TEST_CASE("Clone Event", "[Clone]") {
  genericCloneTest(Event(), false);
  auto d = utils::test::generateWrongDataset<uac::Dataset>();

  for (auto& group : d->acquisition.groups) {
    if (group != nullptr) {
      for (auto& e : group->sequence) {
        genericCloneTest(e, false);
      }
    }
  }
}

TEST_CASE("Clone Excitation", "[Clone]") {
  genericCloneTest(Excitation(), true);
  auto d = utils::test::generateWrongDataset<uac::Dataset>();

  for (auto& ex : d->acquisition.excitations) {
    if (ex != nullptr) {
      genericCloneTest(*ex, true);
    }
  }
}

TEST_CASE("Clone Group", "[Clone]") {
  genericCloneTest(Group(), true);
  auto d = utils::test::generateWrongDataset<uac::Dataset>();

  for (auto& g : d->acquisition.groups) {
    if (g != nullptr) {
      genericCloneTest(*g, true);
    }
  }
}

TEST_CASE("Clone TriggerIn", "[Clone]") {
  genericCloneTest(TriggerIn(), true);
  auto d = utils::test::generateWrongDataset<uac::Dataset>();

  genericCloneTest(d->acquisition.trigger_in, true);
}

TEST_CASE("Clone TriggerOut", "[Clone]") {
  genericCloneTest(TriggerOut(), true);
  auto d = utils::test::generateWrongDataset<uac::Dataset>();

  genericCloneTest(d->acquisition.trigger_out, true);
}

TEST_CASE("Clone DestinationLink", "[Clone]") {
  genericCloneTest(DestinationLink(), true);
  auto d = utils::test::generateWrongDataset<uac::Dataset>();

  for (auto& g : d->acquisition.groups) {
    if (g != nullptr) {
      genericCloneTest(g->destinations, true);
      for (auto& dest : g->destinations) {
        genericCloneTest(dest, true);
      }
    }
  }
}

TEST_CASE("Clone SuperGroup", "[Clone]") {
  genericCloneTest(SuperGroup(), true);
  auto d = utils::test::generateWrongDataset<uac::Dataset>();

  for (auto& g : d->acquisition.groups) {
    if (g != nullptr) {
      genericCloneTest(*g, true);
    }
  }
}

TEST_CASE("Clone Probe", "[Clone]") {
  genericCloneTest(Probe(), true);
  const std::shared_ptr<uac::Dataset> d = utils::test::generateWrongDataset<uac::Dataset>();

  for (const std::shared_ptr<uac::Probe>& p : d->acquisition.probes) {
    if (p != nullptr) {
      if (p->description != "Probe with wrong ptr") {
        genericCloneTest(*p, true);
      }
      const std::shared_ptr<uac::Probe> p_cloned = utils::detail::clone(p);

      REQUIRE(p_cloned->element_geometries.size() == p->element_geometries.size());
      REQUIRE(p_cloned->impulse_responses.size() == p->impulse_responses.size());
      REQUIRE(p_cloned->elements.size() == p->elements.size());

      for (size_t eg_id = 0; eg_id < p->element_geometries.size(); ++eg_id) {
        REQUIRE(urx::valueComparison(p_cloned->element_geometries[eg_id],
                                     p->element_geometries.at(eg_id)));
        if (p->element_geometries.at(eg_id)) {
          REQUIRE(*p_cloned->element_geometries[eg_id] == *p->element_geometries.at(eg_id));
          REQUIRE(p_cloned->element_geometries[eg_id].get() !=
                  p->element_geometries.at(eg_id).get());
        }
      }
      for (size_t ir_id = 0; ir_id < p->impulse_responses.size(); ++ir_id) {
        REQUIRE(urx::valueComparison(p_cloned->impulse_responses[ir_id],
                                     p->impulse_responses.at(ir_id)));
        if (p->impulse_responses.at(ir_id)) {
          REQUIRE(*p_cloned->impulse_responses[ir_id] == *p->impulse_responses.at(ir_id));
          REQUIRE(p_cloned->impulse_responses[ir_id].get() != p->impulse_responses.at(ir_id).get());
        }
      }
      for (size_t e_id = 0; e_id < p_cloned->elements.size(); ++e_id) {
        std::optional<size_t> const eg_id = urx::utils::common::getElementIndex(
            p_cloned->element_geometries, p_cloned->elements.at(e_id).element_geometry, false);
        if (!eg_id.has_value()) {
          REQUIRE(p_cloned->elements.at(e_id).element_geometry.expired());
        } else {
          REQUIRE(urx::valueComparison(p_cloned->elements.at(e_id).element_geometry,
                                       p_cloned->element_geometries.at(eg_id.value())));
          REQUIRE(urx::valueComparison(p_cloned->elements.at(e_id).element_geometry,
                                       p->element_geometries.at(eg_id.value())));
          REQUIRE(std::nullopt !=
                  urx::utils::common::getElementIndex(
                      p->element_geometries, p_cloned->elements.at(e_id).element_geometry, true));
          REQUIRE(std::nullopt != urx::utils::common::getElementIndex(
                                      p->element_geometries,
                                      p_cloned->elements.at(e_id).element_geometry.lock(), true));
          REQUIRE(std::nullopt ==
                  urx::utils::common::getElementIndex(
                      p->element_geometries, p_cloned->elements.at(e_id).element_geometry, false));
          REQUIRE(std::nullopt == urx::utils::common::getElementIndex(
                                      p->element_geometries,
                                      p_cloned->elements.at(e_id).element_geometry.lock(), false));
        }

        const std::optional<size_t> ir_id = urx::utils::common::getElementIndex(
            p_cloned->impulse_responses, p_cloned->elements.at(e_id).impulse_response, true);
        if (!ir_id.has_value()) {
          REQUIRE(p_cloned->elements.at(e_id).impulse_response.expired());
        } else {
          REQUIRE(urx::valueComparison(p_cloned->elements.at(e_id).impulse_response,
                                       p_cloned->impulse_responses.at(ir_id.value())));
          REQUIRE(urx::valueComparison(p_cloned->elements.at(e_id).impulse_response,
                                       p->impulse_responses.at(ir_id.value())));

          REQUIRE(std::nullopt !=
                  urx::utils::common::getElementIndex(
                      p->impulse_responses, p_cloned->elements.at(e_id).impulse_response, true));
          REQUIRE(std::nullopt != urx::utils::common::getElementIndex(
                                      p->impulse_responses,
                                      p_cloned->elements.at(e_id).impulse_response.lock(), true));
          REQUIRE(std::nullopt ==
                  urx::utils::common::getElementIndex(
                      p->impulse_responses, p_cloned->elements.at(e_id).impulse_response, false));
          REQUIRE(std::nullopt == urx::utils::common::getElementIndex(
                                      p->impulse_responses,
                                      p_cloned->elements.at(e_id).impulse_response.lock(), false));
        }
      }
    }
  }
}

void testAcquisitionClone(const Acquisition& acq, const Acquisition& acq_cloned) {
  // Check Acquisition::probes
  REQUIRE(urx::valueComparison(acq_cloned.probes, acq.probes));
  for (size_t p_id = 0; p_id < acq.probes.size(); ++p_id) {
    REQUIRE(urx::valueComparison(acq_cloned.probes.at(p_id), acq.probes.at(p_id)));
    if (acq.probes.at(p_id).get()) {
      REQUIRE(acq_cloned.probes.at(p_id).get() != acq.probes.at(p_id).get());
    } else {
      REQUIRE(acq_cloned.probes.at(p_id).get() == acq.probes.at(p_id).get());
    }
  }

  // Check Acquisition::excitations
  REQUIRE(urx::valueComparison(acq_cloned.excitations, acq.excitations));
  for (size_t ex_id = 0; ex_id < acq.excitations.size(); ++ex_id) {
    REQUIRE(urx::valueComparison(acq_cloned.excitations.at(ex_id), acq.excitations.at(ex_id)));
    if (acq.excitations.at(ex_id).get()) {
      REQUIRE(acq_cloned.excitations.at(ex_id).get() != acq.excitations.at(ex_id).get());
    } else {
      REQUIRE(acq_cloned.excitations.at(ex_id).get() == acq.excitations.at(ex_id).get());
    }
  }

  // Check Acquisition::groups
  REQUIRE(urx::valueComparison(acq_cloned.groups, acq.groups));
  for (size_t g_id = 0; g_id < acq.groups.size(); ++g_id) {
    REQUIRE(urx::valueComparison(acq_cloned.groups.at(g_id), acq.groups.at(g_id)));
    if (acq.groups.at(g_id).get()) {
      REQUIRE(acq_cloned.groups.at(g_id).get() != acq.groups.at(g_id).get());
    } else {
      REQUIRE(acq_cloned.groups.at(g_id).get() == acq.groups.at(g_id).get());
    }
  }

  // Check Acquisition::super_groups
  REQUIRE(urx::valueComparison(acq_cloned.super_groups, acq.super_groups));
  for (size_t sg_id = 0; sg_id < acq.super_groups.size(); ++sg_id) {
    REQUIRE(urx::valueComparison(acq_cloned.super_groups.at(sg_id), acq.super_groups.at(sg_id)));
    if (acq.super_groups.at(sg_id).get()) {
      REQUIRE(acq_cloned.super_groups.at(sg_id).get() != acq.super_groups.at(sg_id).get());
    } else {
      REQUIRE(acq_cloned.super_groups.at(sg_id).get() == acq.super_groups.at(sg_id).get());
    }
  }

  for (size_t g_id = 0; g_id < acq.groups.size(); ++g_id) {
    const auto& group = acq.groups.at(g_id);
    const auto& group_cloned = acq_cloned.groups.at(g_id);
    if (group) {
      REQUIRE(group->sequence.size() == group_cloned->sequence.size());
      for (size_t evt_id = 0; evt_id < group_cloned->sequence.size(); ++evt_id) {
        const auto& evt = group->sequence.at(evt_id);
        const auto& evt_cloned = group_cloned->sequence.at(evt_id);

        const auto& receive_setup_cloned = evt_cloned.receive_setup;
        const auto& transmit_setup_cloned = evt_cloned.transmit_setup;

        const auto& receive_setup = evt.receive_setup;
        const auto& transmit_setup = evt.transmit_setup;

        // Check ReceiveSetup::probe
        std::optional<size_t> p_id =
            urx::utils::common::getElementIndex(acq.probes, receive_setup.probe, false);
        std::optional<size_t> p_id_cloned = urx::utils::common::getElementIndex(
            acq_cloned.probes, receive_setup_cloned.probe, false);
        REQUIRE(p_id == p_id_cloned);
        if (!p_id.has_value()) {
          REQUIRE(receive_setup_cloned.probe.expired());
        } else {
          REQUIRE(valueComparison(receive_setup_cloned.probe, acq_cloned.probes.at(*p_id)));
          REQUIRE(valueComparison(receive_setup_cloned.probe, acq.probes.at(*p_id)));
        }

        // Check TansmitSetup::probe
        p_id = urx::utils::common::getElementIndex(acq.probes, transmit_setup.probe, false);
        p_id_cloned = urx::utils::common::getElementIndex(acq_cloned.probes,
                                                          transmit_setup_cloned.probe, false);
        REQUIRE(p_id == p_id_cloned);
        if (!p_id.has_value()) {
          REQUIRE(transmit_setup_cloned.probe.expired());
        } else {
          REQUIRE(valueComparison(transmit_setup_cloned.probe, acq_cloned.probes.at(*p_id)));
          REQUIRE(valueComparison(transmit_setup_cloned.probe, acq.probes.at(*p_id)));
        }

        // Check TansmitSetup::excitation
        REQUIRE(transmit_setup.excitations.size() == transmit_setup_cloned.excitations.size());
        for (size_t ex_id = 0; ex_id < transmit_setup.excitations.size(); ++ex_id) {
          const auto& excitation = transmit_setup.excitations.at(ex_id);
          const auto& excitation_cloned = transmit_setup_cloned.excitations.at(ex_id);
          const std::optional<size_t> acq_ex_id =
              urx::utils::common::getElementIndex(acq.excitations, excitation, false);
          const std::optional<size_t> acq_cloned_ex_id =
              urx::utils::common::getElementIndex(acq_cloned.excitations, excitation_cloned, false);

          REQUIRE(acq_ex_id == acq_cloned_ex_id);
          if (!acq_ex_id.has_value()) {
            REQUIRE(excitation_cloned.expired());
          } else {
            REQUIRE(valueComparison(excitation_cloned, acq_cloned.excitations.at(*acq_ex_id)));
            REQUIRE(valueComparison(excitation_cloned, acq.excitations.at(*acq_ex_id)));
          }
        }
      }
    }
  }
}

TEST_CASE("Clone Acquisition", "[Clone]") {
  genericCloneTest(Acquisition(), false);
  auto d = utils::test::generateFakeDataset<Dataset>();
  Acquisition acq = d->acquisition;
  Acquisition acq_cloned = utils::detail::clone(acq);
  REQUIRE(acq == acq_cloned);
  REQUIRE(std::is_same_v<decltype(acq), decltype(acq_cloned)>);

  testAcquisitionClone(acq, acq_cloned);

  d = utils::test::generateWrongDataset<Dataset>();
  acq = d->acquisition;
  acq_cloned = utils::detail::clone(acq);
  // Wrong Dataset contains pointers in a probe that does not belong to it causing the non equal comparison
  REQUIRE(acq != acq_cloned);
  REQUIRE(std::is_same_v<decltype(acq), decltype(acq_cloned)>);

  for (size_t p_id = 0; p_id < acq.probes.size(); ++p_id) {
    if (acq.probes.at(p_id)->description == "Probe with wrong ptr") {
      acq.probes.erase(acq.probes.begin() + p_id);
      break;
    }
  }
  genericCloneTest(acq, false);
  acq_cloned = utils::detail::clone(acq);
  testAcquisitionClone(acq, acq_cloned);
}

TEST_CASE("Clone Dataset", "[Clone]") {
  genericCloneTest(Dataset(), true);
  const std::shared_ptr<Dataset> d = utils::test::generateFakeDataset<Dataset>();
  genericCloneTest(*d, true);

  const std::shared_ptr<Dataset> d_cloned = utils::detail::clone(d);
  testAcquisitionClone(d->acquisition, d_cloned->acquisition);
}

}  // namespace uac::utils::test