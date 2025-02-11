#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include <pybind11/cast.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>       // IWYU pragma: keep
#include <pybind11/stl_bind.h>  // IWYU pragma: keep
#include <pyerrors.h>

#include <urx/config.h>
#include <urx/dataset.h>
#include <urx/detail/double_nan.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/enums.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/python/bindings.h>
#include <urx/transform.h>
#include <urx/utils/exception.h>
#include <urx/vector.h>
#include <urx/wave.h>

#include <uac/acquisition.h>
#include <uac/dataset.h>
#include <uac/destination_link.h>
#include <uac/detail/compare.h>
#include <uac/element.h>
#include <uac/element_geometry.h>
#include <uac/enums.h>
#include <uac/event.h>
#include <uac/excitation.h>
#include <uac/group.h>
#include <uac/hw_config.h>
#include <uac/igroup.h>
#include <uac/impulse_response.h>
#include <uac/probe.h>
#include <uac/python/bindings.h>
#include <uac/receive_setup.h>
#include <uac/super_group.h>
#include <uac/transform.h>
#include <uac/transmit_setup.h>
#include <uac/trigger.h>
#include <uac/uac.h>
#include <uac/utils/to_urx.h>
#include <uac/version.h>
#include <uac/wave.h>

#ifdef URX_WITH_HDF5
#include <uac/utils/io/reader.h>
#include <uac/utils/io/writer.h>
#endif

namespace py = pybind11;

namespace urx::python::detail {

namespace {
constexpr const char VEC_EXCITATION_PTR[] = "VecExcitationPtr";
constexpr const char VEC_EVENT[] = "VecEvent";
constexpr const char VEC_GROUP_PTR[] = "VecGroupPtr";
constexpr const char VEC_SUPER_GROUP_PTR[] = "VecSuperGroupPtr";
constexpr const char VEC_DESTINATION_LINK[] = "VecDestinationLink";
constexpr const char VEC_HW_CONFIG[] = "VecHwConfig";
}  // namespace

template <>
void bindVector<uac::python::VecExcitationPtr>(pybind11::module_ &m) {
  bindVectorNoBufferProtocol<uac::python::VecExcitationPtr, VEC_EXCITATION_PTR>(m);
}
template <>
void bindVector<uac::python::VecEvent>(pybind11::module_ &m) {
  bindVectorNoBufferProtocol<uac::python::VecEvent, VEC_EVENT>(m);
}
template <>
void bindVector<uac::python::VecGroupPtr>(pybind11::module_ &m) {
  bindVectorNoBufferProtocol<uac::python::VecGroupPtr, VEC_GROUP_PTR>(m);
}
template <>
void bindVector<uac::python::VecSuperGroupPtr>(pybind11::module_ &m) {
  bindVectorNoBufferProtocol<uac::python::VecSuperGroupPtr, VEC_SUPER_GROUP_PTR>(m);
}
template <>
void bindVector<uac::python::VecDestinationLink>(pybind11::module_ &m) {
  bindVectorNoBufferProtocol<uac::python::VecDestinationLink, VEC_DESTINATION_LINK>(m);
}
template <>
void bindVector<uac::python::VecHwConfig>(pybind11::module_ &m) {
  bindVectorNoBufferProtocol<uac::python::VecHwConfig, VEC_HW_CONFIG>(m);
}

}  // namespace urx::python::detail

// NOLINTBEGIN(misc-no-recursion)
namespace {

uac::HwConfig snToHwConfigValue(const py::object &object);

template <typename T>
uac::HwConfig::VecDataTypeVariant numpyToHwConfigValue(const py::object &object) {
  const auto array = object.cast<py::array_t<T>>();
  const auto *data_ptr = array.data();
  if (array.size() == 1) {
    return *data_ptr;
  }
  std::vector<T> retval;
  retval.reserve(array.size());
  std::copy(array.data(), array.data() + array.size(), std::back_inserter(retval));
  return retval;
}

uac::HwConfig::VecDataTypeVariant singlePyobjectToHwConfigValue(const py::object &object) {
  const py::object numpy_hwconfig = py::module::import("types").attr("SimpleNamespace");

  const py::object numpy_uint8_type = py::module::import("numpy").attr("uint8");
  const py::object numpy_uint16_type = py::module::import("numpy").attr("uint16");
  const py::object numpy_uint32_type = py::module::import("numpy").attr("uint32");
  const py::object numpy_uint64_type = py::module::import("numpy").attr("uint64");
  const py::object numpy_int8_type = py::module::import("numpy").attr("int8");
  const py::object numpy_int16_type = py::module::import("numpy").attr("int16");
  const py::object numpy_int32_type = py::module::import("numpy").attr("int32");
  const py::object numpy_int64_type = py::module::import("numpy").attr("int64");
  const py::object numpy_float32_type = py::module::import("numpy").attr("float32");
  const py::object numpy_float64_type = py::module::import("numpy").attr("float64");

  if (object.get_type().is(numpy_hwconfig)) {
    return snToHwConfigValue(object);
  }
  if (object.get_type().is(numpy_uint8_type)) {
    return numpyToHwConfigValue<uint8_t>(object);
  }
  if (object.get_type().is(numpy_uint16_type)) {
    return numpyToHwConfigValue<uint16_t>(object);
  }
  if (object.get_type().is(numpy_uint32_type)) {
    return numpyToHwConfigValue<uint32_t>(object);
  }
  if (object.get_type().is(numpy_uint64_type)) {
    return numpyToHwConfigValue<uint64_t>(object);
  }
  if (object.get_type().is(numpy_int8_type)) {
    return numpyToHwConfigValue<int8_t>(object);
  }
  if (object.get_type().is(numpy_int16_type)) {
    return numpyToHwConfigValue<int16_t>(object);
  }
  if (object.get_type().is(numpy_int32_type)) {
    return numpyToHwConfigValue<int32_t>(object);
  }
  if (object.get_type().is(numpy_int64_type)) {
    return numpyToHwConfigValue<int64_t>(object);
  }
  if (object.get_type().is(numpy_float32_type)) {
    return numpyToHwConfigValue<float>(object);
  }
  if (object.get_type().is(numpy_float64_type)) {
    return numpyToHwConfigValue<double>(object);
  }
  if (py::isinstance<py::str>(object)) {
    return py::str(object);
  }
  if (py::isinstance<urx::python::VecUInt8>(object)) {
    return object.cast<urx::python::VecUInt8>();
  }
  if (py::isinstance<urx::python::VecUInt16>(object)) {
    return object.cast<urx::python::VecUInt16>();
  }
  if (py::isinstance<urx::python::VecUInt32>(object)) {
    return object.cast<urx::python::VecUInt32>();
  }
  if (py::isinstance<urx::python::VecUInt64>(object)) {
    return object.cast<urx::python::VecUInt64>();
  }
  if (py::isinstance<urx::python::VecInt8>(object)) {
    return object.cast<urx::python::VecInt8>();
  }
  if (py::isinstance<urx::python::VecInt16>(object)) {
    return object.cast<urx::python::VecInt16>();
  }
  if (py::isinstance<urx::python::VecInt32>(object)) {
    return object.cast<urx::python::VecInt32>();
  }
  if (py::isinstance<urx::python::VecInt64>(object)) {
    return object.cast<urx::python::VecInt64>();
  }
  if (py::isinstance<urx::python::VecFloat32>(object)) {
    return object.cast<urx::python::VecFloat32>();
  }
  if (py::isinstance<urx::python::VecFloat64>(object)) {
    return object.cast<urx::python::VecFloat64>();
  }
  if (py::isinstance<urx::python::VecString>(object)) {
    return object.cast<urx::python::VecString>();
  }
  throw std::runtime_error(
      ("Unknown type (" + py::str(py::type::of(object)).cast<std::string>() + ")").c_str());
}

template <typename T>
uac::HwConfig::VecDataTypeVariant pylistToHwConfigValue(const py::object &object) {
  std::vector<T> retval;
  for (const auto &elem : object) {
    const py::object elem_object = elem.cast<py::object>();
    retval.push_back(std::get<T>(numpyToHwConfigValue<T>(elem_object)));
  }
  return retval;
}

template <>
uac::HwConfig::VecDataTypeVariant pylistToHwConfigValue<std::string>(const py::object &object) {
  std::vector<std::string> retval;
  for (const auto &elem : object) {
    const py::object elem_object = elem.cast<py::object>();
    retval.push_back(py::str(elem_object).cast<std::string>());
  }
  return retval;
}

template <>
uac::HwConfig::VecDataTypeVariant pylistToHwConfigValue<uac::HwConfig>(const py::object &object) {
  std::vector<uac::HwConfig> retval;
  for (const auto &elem : object) {
    const py::object elem_object = elem.cast<py::object>();
    retval.push_back(snToHwConfigValue(elem_object));
  }
  return retval;
}

uac::HwConfig::VecDataTypeVariant pyobjectToHwConfigValue(const py::object &object) {
  if (py::isinstance<py::list>(object)) {
    const py::list list_obj = object.cast<py::list>();
    const auto elem_type = *list_obj.begin();

    const py::object numpy_hwconfig = py::module::import("types").attr("SimpleNamespace");

    const py::object numpy_uint8_type = py::module::import("numpy").attr("uint8");
    const py::object numpy_uint16_type = py::module::import("numpy").attr("uint16");
    const py::object numpy_uint32_type = py::module::import("numpy").attr("uint32");
    const py::object numpy_uint64_type = py::module::import("numpy").attr("uint64");
    const py::object numpy_int8_type = py::module::import("numpy").attr("int8");
    const py::object numpy_int16_type = py::module::import("numpy").attr("int16");
    const py::object numpy_int32_type = py::module::import("numpy").attr("int32");
    const py::object numpy_int64_type = py::module::import("numpy").attr("int64");
    const py::object numpy_float32_type = py::module::import("numpy").attr("float32");
    const py::object numpy_float64_type = py::module::import("numpy").attr("float64");

    if (elem_type.get_type().is(numpy_hwconfig)) {
      return pylistToHwConfigValue<uac::HwConfig>(list_obj);
    }
    if (elem_type.get_type().is(numpy_uint8_type)) {
      return pylistToHwConfigValue<uint8_t>(list_obj);
    }
    if (elem_type.get_type().is(numpy_uint16_type)) {
      return pylistToHwConfigValue<uint16_t>(list_obj);
    }
    if (elem_type.get_type().is(numpy_uint32_type)) {
      return pylistToHwConfigValue<uint32_t>(list_obj);
    }
    if (elem_type.get_type().is(numpy_uint64_type)) {
      return pylistToHwConfigValue<uint64_t>(list_obj);
    }
    if (elem_type.get_type().is(numpy_int8_type)) {
      return pylistToHwConfigValue<int8_t>(list_obj);
    }
    if (elem_type.get_type().is(numpy_int16_type)) {
      return pylistToHwConfigValue<int16_t>(list_obj);
    }
    if (elem_type.get_type().is(numpy_int32_type)) {
      return pylistToHwConfigValue<int32_t>(list_obj);
    }
    if (elem_type.get_type().is(numpy_int64_type)) {
      return pylistToHwConfigValue<int64_t>(list_obj);
    }
    if (elem_type.get_type().is(numpy_float32_type)) {
      return pylistToHwConfigValue<float>(list_obj);
    }
    if (elem_type.get_type().is(numpy_float64_type)) {
      return pylistToHwConfigValue<double>(list_obj);
    }
    if (py::reinterpret_borrow<py::object>(elem_type)) {
      return pylistToHwConfigValue<std::string>(list_obj);
    }
    throw std::runtime_error(
        ("Unknown type (" + py::str(py::type::of(elem_type)).cast<std::string>() + ") in list")
            .c_str());
  }
  return singlePyobjectToHwConfigValue(object);
}

uac::HwConfig snToHwConfigValue(const py::object &object) {
  uac::HwConfig retval;

  const py::object dict = object.attr("__dict__");
  const py::object keys = dict.attr("keys")();
  for (const auto &key : keys) {
    const py::object value = dict[key];
    const std::string key_str = py::str(key);
    retval.values[key_str] = pyobjectToHwConfigValue(value);
  }

  return retval;
}

}  // namespace
// NOLINTEND(misc-no-recursion)

template <typename... Ts>
struct Overload : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
Overload(Ts...) -> Overload<Ts...>;

// NOLINTBEGIN(misc-redundant-expression)
PYBIND11_MODULE(bindings, m) {
  m.doc() = "Python binding of urx library";

  const py::module_ purx = py::module_::import("ultrasound_rawdata_exchange");

  urx::python::detail::bindVector<urx::python::VecUInt8>(m);
  urx::python::detail::bindVector<urx::python::VecVecUInt8>(m);
  urx::python::detail::bindVector<urx::python::VecUInt16>(m);
  urx::python::detail::bindVector<urx::python::VecVecUInt16>(m);
  urx::python::detail::bindVector<urx::python::VecUInt32>(m);
  urx::python::detail::bindVector<urx::python::VecVecUInt32>(m);
  urx::python::detail::bindVector<urx::python::VecUInt64>(m);
  urx::python::detail::bindVector<urx::python::VecVecUInt64>(m);
  urx::python::detail::bindVector<urx::python::VecInt8>(m);
  urx::python::detail::bindVector<urx::python::VecVecInt8>(m);
  urx::python::detail::bindVector<urx::python::VecInt16>(m);
  urx::python::detail::bindVector<urx::python::VecVecInt16>(m);
  urx::python::detail::bindVector<urx::python::VecInt32>(m);
  urx::python::detail::bindVector<urx::python::VecVecInt32>(m);
  urx::python::detail::bindVector<urx::python::VecInt64>(m);
  urx::python::detail::bindVector<urx::python::VecVecInt64>(m);
  urx::python::detail::bindVector<urx::python::VecFloat32>(m);
  urx::python::detail::bindVector<urx::python::VecVecFloat32>(m);
  urx::python::detail::bindVector<urx::python::VecFloat64>(m);
  urx::python::detail::bindVector<urx::python::VecVecFloat64>(m);
  urx::python::detail::bindVector<urx::python::VecString>(m);
  urx::python::detail::bindVector<urx::python::VecVecString>(m);

  /*
  py::enum_<urx::SamplingType>(m, "UacSamplingType")
      .value("RF", urx::SamplingType::RF)
      .value("IQ", urx::SamplingType::IQ)
      .value("UNDEFINED", urx::SamplingType::UNDEFINED);
  m.attr("SamplingType") = m.attr("UacSamplingType");
  */
  static_assert(std::is_same_v<urx::SamplingType, uac::SamplingType>);
  m.attr("SamplingType") = purx.attr("SamplingType");

  /*
  py::enum_<urx::DataType>(m, "UacDataType")
      .value("INT16", urx::DataType::INT16)
      .value("INT32", urx::DataType::INT32)
      .value("FLOAT", urx::DataType::FLOAT)
      .value("DOUBLE", urx::DataType::DOUBLE)
      .value("UNDEFINED", urx::DataType::UNDEFINED);
  m.attr("DataType") = m.attr("UacDataType");
  */
  static_assert(std::is_same_v<urx::DataType, uac::DataType>);
  m.attr("DataType") = purx.attr("DataType");

  /*
  py::enum_<urx::ProbeType>(m, "UacProbeType")
      .value("LINEAR", urx::ProbeType::LINEAR)
      .value("CURVILINEAR", urx::ProbeType::CURVILINEAR)
      .value("RCA", urx::ProbeType::RCA)
      .value("MATRIX", urx::ProbeType::MATRIX)
      .value("SPARSE", urx::ProbeType::SPARSE)
      .value("UNDEFINED", urx::ProbeType::UNDEFINED);
  m.attr("ProbeType") = m.attr("UacProbeType");
  */
  static_assert(std::is_same_v<urx::ProbeType, uac::ProbeType>);
  m.attr("ProbeType") = purx.attr("ProbeType");

  /*
  py::enum_<urx::WaveType>(m, "UacWaveType")
      .value("CONVERGING_WAVE", urx::WaveType::CONVERGING_WAVE)
      .value("DIVERGING_WAVE", urx::WaveType::DIVERGING_WAVE)
      .value("PLANE_WAVE", urx::WaveType::PLANE_WAVE)
      .value("CYLINDRICAL_WAVE", urx::WaveType::CYLINDRICAL_WAVE)
      .value("UNDEFINED", urx::WaveType::UNDEFINED);
  m.attr("WaveType") = m.attr("UacWaveType");
  */
  static_assert(std::is_same_v<urx::WaveType, uac::WaveType>);
  m.attr("WaveType") = purx.attr("WaveType");

  py::enum_<uac::Edge>(m, "UacEdge")
      .value("RISING", uac::Edge::RISING)
      .value("FALLING", uac::Edge::FALLING)
      .value("HIGH", uac::Edge::HIGH)
      .value("LOW", uac::Edge::LOW)
      .value("UNDEFINED", uac::Edge::UNDEFINED);
  m.attr("Edge") = m.attr("UacEdge");

  py::enum_<uac::Polarity>(m, "UacPolarity")
      .value("POSITIVE", uac::Polarity::POSITIVE)
      .value("NEGATIVE", uac::Polarity::NEGATIVE)
      .value("UNDEFINED", uac::Polarity::UNDEFINED);
  m.attr("Polarity") = m.attr("UacPolarity");

  /*
  py::class_<urx::DoubleNan>(m, "UacDoubleNan")
      .def(py::init<double>())
      .def(py::init<const urx::DoubleNan &>())
      .def(py::init())
      .def_readwrite("value", &urx::DoubleNan::value)
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def(double() == py::self)
      .def(double() != py::self)

      .def(py::self + py::self)
      .def(double() + py::self)
      .def(py::self + double())

      .def(py::self - py::self)
      .def(double() - py::self)
      .def(py::self - double())

      .def(py::self * py::self)
      .def(double() * py::self)
      .def(py::self * double())

      .def(py::self / py::self)
      .def(double() / py::self)
      .def(py::self / double())

      .def(py::self += py::self)
      .def(py::self += double())

      .def(py::self -= py::self)
      .def(py::self -= double())

      .def(py::self *= py::self)
      .def(py::self *= double())

      .def(py::self /= py::self)
      .def(py::self /= double())

      .def(+py::self)
      .def(-py::self);
  py::implicitly_convertible<double, urx::DoubleNan>();
  py::implicitly_convertible<int, urx::DoubleNan>();
  m.attr("DoubleNan") = m.attr("UacDoubleNan");
  */
  m.attr("DoubleNan") = purx.attr("DoubleNan");

  /*
  urx::python::registerImpulseResponse<uac::ImpulseResponse>(m, "Uac").def(
      py::init([](const urx::DoubleNan &sampling_frequency, const urx::DoubleNan &time_offset,
                  const std::string &units, const urx::python::VecFloat64 &vec) {
        return uac::ImpulseResponse{sampling_frequency, time_offset, units, vec};
      }));
  */
  static_assert(std::is_same_v<urx::ImpulseResponse, uac::ImpulseResponse>);
  m.attr("ImpulseResponse") = purx.attr("ImpulseResponse");
  urx::python::detail::bindVector<uac::python::VecImpulseResponsePtr>(m);

  /*
  urx::python::registerVector3D<uac::Vector3D<double>>(m, "Uac");
  urx::python::detail::bindVector<uac::python::VecVector3D>(m);
  */
  static_assert(std::is_same_v<urx::Vector3D<int>, uac::Vector3D<int>>);
  m.attr("Vector3D") = purx.attr("Vector3D");
  m.attr("VecVector3D") = purx.attr("VecVector3D");

  /*
  urx::python::registerVector2D<uac::Vector2D<double>>(m, "Uac");
  urx::python::detail::bindVector<uac::python::VecVector2D>(m);
  */
  static_assert(std::is_same_v<urx::Vector2D<int>, uac::Vector2D<int>>);
  m.attr("Vector2D") = purx.attr("Vector2D");
  m.attr("VecVector2D") = purx.attr("VecVector2D");

  // Version constants
  m.attr("UAC_VERSION_MAJOR") = uac::UAC_VERSION_MAJOR;
  m.attr("UAC_VERSION_MINOR") = uac::UAC_VERSION_MINOR;
  m.attr("UAC_VERSION_PATCH") = uac::UAC_VERSION_PATCH;

  urx::python::registerVersion<uac::Version>(m, "Uac");

  py::class_<uac::HwConfig>(m, "UacHwConfig")
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_property(
          "values",
          [](uac::HwConfig &hwconfig) -> py::object {
            // Création d'un objet Python vide
            py::object obj = py::module::import("types").attr("SimpleNamespace")();

            for (const auto &field : hwconfig.values) {
              std::visit(
                  Overload{[&fieldname = field.first, &obj](const auto &value) {
                             obj.attr("__dict__")[fieldname.c_str()] = value;
                           },
                           [&fieldname = field.first, &obj](const uac::HwConfig &value) {
                             py::object obj2 =
                                 py::module::import("types").attr("SimpleNamespace")();
                             for (const auto &[key, val] : value.values) {
                               obj2.attr("__dict__")[key.c_str()] = val;
                             }
                             obj.attr("__dict__")[fieldname.c_str()] = obj2;
                           },
                           [&fieldname = field.first, &obj](const uac::python::VecHwConfig &value) {
                             py::list list;

                             for (const auto &value_i : value) {
                               py::object obj2 =
                                   py::module::import("types").attr("SimpleNamespace")();
                               for (const auto &[key, val] : value_i.values) {
                                 obj2.attr("__dict__")[key.c_str()] = val;
                               }
                               list.append(obj2);
                             }
                             obj.attr("__dict__")[fieldname.c_str()] = list;
                           }},
                  field.second);
            }

            return obj;
          },
          [](uac::HwConfig &hwconfig, const py::object &object) {
            hwconfig = snToHwConfigValue(object);
          });
  urx::python::detail::bindVector<uac::python::VecHwConfig>(m);
  py::bind_map<uac::python::MapHwConfig>(m, "MapHwConfig");
  m.attr("HwConfig") = m.attr("UacHwConfig");

  /*
  urx::python::registerElementGeometry<uac::ElementGeometry>(m, "Uac").def(
      py::init<const uac::python::VecVector3D &>());
  */
  static_assert(std::is_same_v<urx::ElementGeometry, uac::ElementGeometry>);
  m.attr("ElementGeometry") = purx.attr("ElementGeometry");
  urx::python::detail::bindVector<uac::python::VecElementGeometryPtr>(m);

  /*
  urx::python::registerTransform<uac::Transform>(m, "Uac").def(
      py::init<const uac::Vector3D<double> &, const uac::Vector3D<double> &>());
      */
  static_assert(std::is_same_v<urx::Transform, uac::Transform>);
  m.attr("Transform") = purx.attr("Transform");

  /*
  urx::python::registerElement<uac::Element>(m, "Uac").def(
      py::init<const uac::Transform &, const std::shared_ptr<uac::ElementGeometry> &,
               const std::shared_ptr<uac::ImpulseResponse> &>());
  urx::python::detail::bindVector<uac::python::VecElement>(m);
  */
  static_assert(std::is_same_v<urx::Element, uac::Element>);
  m.attr("Element") = purx.attr("Element");
  m.attr("VecElement") = purx.attr("VecElement");

  urx::python::registerExcitation<uac::Excitation>(m, "Uac")
      .def(py::init<const std::string &, const urx::DoubleNan &, const urx::DoubleNan &,
                    const urx::python::VecFloat64 &, const uac::HwConfig &>())
      .def_readwrite("hw_config", &uac::Excitation::hw_config);
  urx::python::detail::bindVector<uac::python::VecExcitationPtr>(m);

  /*
  urx::python::registerProbe<uac::Probe>(m, "Uac").def(
      py::init<const std::string &, urx::ProbeType, const uac::Transform &,
               const uac::python::VecElementGeometryPtr &,
               const uac::python::VecImpulseResponsePtr &, const uac::python::VecElement &>());
  */
  static_assert(std::is_same_v<urx::Probe, uac::Probe>);
  m.attr("Probe") = purx.attr("Probe");
  urx::python::detail::bindVector<uac::python::VecProbePtr>(m);

  /*
  urx::python::registerWave<uac::Wave>(m, "Uac").def(
      py::init<const urx::WaveType &, const urx::DoubleNan &, const urx::Vector3D<double> &,
               const urx::python::VecFloat64 &>());
  */
  static_assert(std::is_same_v<urx::Wave, uac::Wave>);
  m.attr("Wave") = purx.attr("Wave");

  urx::python::registerTransmitSetup<uac::TransmitSetup>(m, "Uac")
      .def(py::init([](const std::shared_ptr<uac::Probe> &probe, const uac::Wave &wave,
                       const urx::python::VecVecUInt32 &active_elements,
                       const uac::python::VecExcitationPtr &excitations,
                       const urx::python::VecFloat64 &delays, const uac::Transform &probe_transform,
                       const urx::DoubleNan &time_offset, const uac::HwConfig &hw_config) {
        const std::vector<std::weak_ptr<uac::Excitation>> excitations_weak(excitations.begin(),
                                                                           excitations.end());
        return uac::TransmitSetup{
            {probe, wave, active_elements, excitations_weak, delays, probe_transform, time_offset},
            hw_config};
      }))
      .def_readwrite("hw_config", &uac::TransmitSetup::hw_config);

  urx::python::registerReceiveSetup<uac::ReceiveSetup>(m, "Uac")
      .def(py::init<const std::shared_ptr<urx::Probe> &, const urx::Transform &,
                    const urx::DoubleNan &, uint32_t, const urx::python::VecVecUInt32 &,
                    const urx::python::VecFloat64 &, const urx::DoubleNan &, const urx::DoubleNan &,
                    const urx::DoubleNan &, const uac::HwConfig &>())
      .def_readwrite("hw_config", &uac::ReceiveSetup::hw_config);

  py::class_<uac::TriggerIn, std::shared_ptr<uac::TriggerIn>>(m, "UacTriggerIn")
      .def(py::init())
      .def(py::init<const uac::TriggerIn &>())
      .def(py::init<const std::string &, uac::Edge>())
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def_readwrite("channel", &uac::TriggerIn::channel)
      .def_readwrite("edge", &uac::TriggerIn::edge);
  m.attr("TriggerIn") = m.attr("UacTriggerIn");

  py::class_<uac::TriggerOut, std::shared_ptr<uac::TriggerOut>>(m, "UacTriggerOut")
      .def(py::init())
      .def(py::init<const uac::TriggerOut &>())
      .def(py::init<const std::string &, urx::DoubleNan, urx::DoubleNan, uac::Polarity>())
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def_readwrite("channel", &uac::TriggerOut::channel)
      .def_readwrite("time_offset", &uac::TriggerOut::time_offset)
      .def_readwrite("pulse_duration", &uac::TriggerOut::pulse_duration)
      .def_readwrite("polarity", &uac::TriggerOut::polarity);
  m.attr("TriggerOut") = m.attr("UacTriggerOut");

  urx::python::registerEvent<uac::Event>(m, "Uac")
      .def(py::init<const uac::TransmitSetup &, const uac::ReceiveSetup &, const urx::DoubleNan &,
                    const uac::TriggerIn &, const uac::TriggerOut &, const uac::HwConfig &>())
      .def_readwrite("time_offset", &uac::Event::time_offset)
      .def_readwrite("trigger_in", &uac::Event::trigger_in)
      .def_readwrite("trigger_out", &uac::Event::trigger_out)
      .def_readwrite("hw_config", &uac::Event::hw_config);
  urx::python::detail::bindVector<uac::python::VecEvent>(m);

  py::class_<uac::IGroup, std::shared_ptr<uac::IGroup>>(m, "UacIGroup")
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def_readwrite("time_offset", &uac::IGroup::time_offset)
      .def_readwrite("trigger_in", &uac::IGroup::trigger_in)
      .def_readwrite("trigger_out", &uac::IGroup::trigger_out)
      .def_readwrite("repetition_count", &uac::IGroup::repetition_count)
      .def_readwrite("destinations", &uac::IGroup::destinations)
      .def_readwrite("period", &uac::IGroup::period)
      .def_readwrite("hw_config", &uac::IGroup::hw_config);
  m.attr("IGroup") = m.attr("UacIGroup");

  py::class_<uac::DestinationLink, std::shared_ptr<uac::DestinationLink>>(m, "UacDestinationLink")
      .def(py::init())
      .def(py::init<const uac::DestinationLink &>())
      .def(py::init<const uac::TriggerIn &, const std::shared_ptr<uac::IGroup> &>())
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def_readwrite("trigger", &uac::DestinationLink::trigger)
      .def_property(
          "destination",
          [](uac::DestinationLink &self) -> std::shared_ptr<uac::IGroup> {
            if (self.destination.expired()) {
              return {};
            }
            return self.destination.lock();
          },
          [](uac::DestinationLink &self, const std::shared_ptr<uac::IGroup> &destination) {
            self.destination = destination;
          });
  urx::python::detail::bindVector<uac::python::VecDestinationLink>(m);
  m.attr("DestinationLink") = m.attr("UacDestinationLink");

  urx::python::registerGroup<uac::Group, uac::IGroup>(m, "Uac")
      .def(py::init([](urx::SamplingType sampling_type, urx::DataType data_type,
                       const std::string &description, const urx::DoubleNan &sound_speed,
                       const uac::python::VecEvent &sequence, const urx::DoubleNan &time_offset,
                       const uac::TriggerIn &trigger_in, const uac::TriggerOut &trigger_out,
                       uint32_t repetition_count,
                       const std::vector<uac::DestinationLink> &destinations,
                       const urx::DoubleNan &period, const uac::HwConfig &hw_config) {
        uac::Group group;

        group.sampling_type = sampling_type;
        group.data_type = data_type;
        group.description = description;
        group.sound_speed = sound_speed;
        group.sequence = sequence;
        group.time_offset = time_offset;
        group.trigger_in = trigger_in;
        group.trigger_out = trigger_out;
        group.repetition_count = repetition_count;
        group.destinations = destinations;
        group.period = period;
        group.hw_config = hw_config;

        return group;
      }))
      .def_readwrite("time_offset", &uac::Group::time_offset)
      .def_readwrite("trigger_in", &uac::Group::trigger_in)
      .def_readwrite("trigger_out", &uac::Group::trigger_out)
      .def_readwrite("repetition_count", &uac::Group::repetition_count)
      .def_readwrite("destinations", &uac::Group::destinations)
      .def_readwrite("period", &uac::Group::period)
      .def_readwrite("hw_config", &uac::Group::hw_config);
  urx::python::detail::bindVector<uac::python::VecGroupPtr>(m);

  py::class_<uac::SuperGroup, std::shared_ptr<uac::SuperGroup>, uac::IGroup>(m, "UacSuperGroup")
      .def(py::init())
      .def(py::init<const uac::SuperGroup &>())
      .def(py::init([](const urx::DoubleNan &time_offset, const uac::TriggerIn &trigger_in,
                       const uac::TriggerOut &trigger_out, uint32_t repetition_count,
                       const std::vector<uac::DestinationLink> &destinations,
                       const urx::DoubleNan &period, const uac::HwConfig &hw_config,
                       const std::shared_ptr<uac::IGroup> &initial_group,
                       const std::string &description) {
        uac::SuperGroup group;

        group.time_offset = time_offset;
        group.trigger_in = trigger_in;
        group.trigger_out = trigger_out;
        group.repetition_count = repetition_count;
        group.destinations = destinations;
        group.period = period;
        group.hw_config = hw_config;
        group.initial_group = initial_group;
        group.description = description;

        return group;
      }))
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def_readwrite("time_offset", &uac::SuperGroup::time_offset)
      .def_readwrite("trigger_in", &uac::SuperGroup::trigger_in)
      .def_readwrite("trigger_out", &uac::SuperGroup::trigger_out)
      .def_readwrite("repetition_count", &uac::SuperGroup::repetition_count)
      .def_readwrite("destinations", &uac::SuperGroup::destinations)
      .def_readwrite("period", &uac::SuperGroup::period)
      .def_readwrite("hw_config", &uac::SuperGroup::hw_config)
      .def_readwrite("description", &uac::SuperGroup::description)
      .def_property(
          "initial_group",
          [](uac::SuperGroup &self) -> std::shared_ptr<uac::IGroup> {
            if (self.initial_group.expired()) {
              return {};
            }
            return self.initial_group.lock();
          },
          [](uac::SuperGroup &self, const std::shared_ptr<uac::IGroup> &initial_group) {
            self.initial_group = initial_group;
          });
  urx::python::detail::bindVector<uac::python::VecSuperGroupPtr>(m);
  m.attr("SuperGroup") = m.attr("UacSuperGroup");

  urx::python::registerAcquisition<uac::Acquisition>(m, "Uac")
      .def(py::init<const std::string &, const std::string &, const std::string &,
                    const std::string &, const std::string &, const urx::DoubleNan &,
                    const std::vector<std::shared_ptr<uac::Probe>> &,
                    const std::vector<std::shared_ptr<uac::Excitation>> &,
                    const std::vector<std::shared_ptr<uac::Group>>,
                    const std::vector<std::shared_ptr<uac::SuperGroup>> &,
                    const std::shared_ptr<uac::IGroup> &, const urx::DoubleNan &,
                    const uac::TriggerIn &, const uac::TriggerOut &, const uac::HwConfig &>())
      .def_readwrite("super_groups", &uac::Acquisition::super_groups)
      .def_property(
          "initial_group",
          [](uac::Acquisition &self) -> std::shared_ptr<uac::IGroup> {
            if (self.initial_group.expired()) {
              return {};
            }
            return self.initial_group.lock();
          },
          [](uac::Acquisition &self, const std::shared_ptr<uac::IGroup> &initial_group) {
            self.initial_group = initial_group;
          })
      .def_readwrite("time_offset", &uac::Acquisition::time_offset)
      .def_readwrite("trigger_in", &uac::Acquisition::trigger_in)
      .def_readwrite("trigger_out", &uac::Acquisition::trigger_out)
      .def_readwrite("hw_config", &uac::Acquisition::hw_config);

  urx::python::registerDataset<uac::Dataset>(m, "Uac").def(
      py::init<const uac::Acquisition &, const uac::Version &>());

  py::register_exception<urx::utils::ReadFileException>(m, "ReadFileException", PyExc_RuntimeError);
  py::register_exception<urx::utils::WriteFileException>(m, "WriteFileException",
                                                         PyExc_RuntimeError);

#ifdef URX_WITH_HDF5
  m.def("loadFromFile", &uac::utils::io::reader::loadFromFile);
  m.def("saveToFile", &uac::utils::io::writer::saveToFile);
#endif

  m.def("toUrx", &uac::utils::toUrx);
}

// NOLINTEND(misc-redundant-expression)
