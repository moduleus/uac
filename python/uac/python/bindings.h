#pragma once

#include <algorithm>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include <pybind11/cast.h>

#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/vector.h>

#include <uac/destination_link.h>
#include <uac/element.h>
#include <uac/element_geometry.h>
#include <uac/event.h>
#include <uac/excitation.h>
#include <uac/group.h>
#include <uac/hw_config.h>
#include <uac/impulse_response.h>
#include <uac/probe.h>
#include <uac/super_group.h>
#include <uac/vector.h>  // IWYU pragma: keep

namespace uac::python {

using VecGroupPtr = std::vector<std::shared_ptr<uac::Group>>;
using VecVector2D = std::vector<uac::Vector2D<double>>;
using VecVector3D = std::vector<uac::Vector3D<double>>;

using VecElementGeometryPtr = std::vector<std::shared_ptr<uac::ElementGeometry>>;
using VecImpulseResponsePtr = std::vector<std::shared_ptr<uac::ImpulseResponse>>;
using VecElement = std::vector<uac::Element>;
using VecExcitationPtr = std::vector<std::shared_ptr<uac::Excitation>>;
using VecEvent = std::vector<uac::Event>;
using VecProbePtr = std::vector<std::shared_ptr<uac::Probe>>;
using VecSuperGroupPtr = std::vector<std::shared_ptr<uac::SuperGroup>>;
using VecDestinationLink = std::vector<std::shared_ptr<uac::DestinationLink>>;

using VecHwConfig = std::vector<uac::HwConfig>;
using MapHwConfig = uac::HwConfig::Values;

}  // namespace uac::python

PYBIND11_MAKE_OPAQUE(uac::python::VecGroupPtr);

PYBIND11_MAKE_OPAQUE(uac::python::VecExcitationPtr);
PYBIND11_MAKE_OPAQUE(uac::python::VecEvent);
PYBIND11_MAKE_OPAQUE(uac::python::VecSuperGroupPtr);
PYBIND11_MAKE_OPAQUE(uac::python::VecDestinationLink);

PYBIND11_MAKE_OPAQUE(uac::python::VecHwConfig);
PYBIND11_MAKE_OPAQUE(uac::python::MapHwConfig);
