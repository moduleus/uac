#include <functional>
#include <memory>
#include <optional>
#include <typeindex>
#include <utility>
#include <vector>

#include <H5Cpp.h>

#include <urx/dataset.h>
#include <urx/probe.h>
#include <urx/utils/exception.h>
#include <urx/utils/io/reader_impl.h>
#include <urx/utils/io/serialize_helper.h>

#include <uac/acquisition.h>
#include <uac/dataset.h>
#include <uac/excitation.h>
#include <uac/group.h>
#include <uac/probe.h>
#include <uac/super_group.h>
#include <uac/uac.h>
#include <uac/utils/io/reader.h>
#include <uac/utils/io/reader_impl.h>  // IWYU pragma: keep
#include <uac/utils/io/serialize_helper.h>
#include <uac/version.h>

namespace uac::utils::io::reader {
std::shared_ptr<Dataset> loadFromFile(const std::string& filename) {
  auto dataset = std::make_shared<Dataset>();

  try {
    const H5::H5File file(filename.data(), H5F_ACC_RDONLY);

    std::vector<std::function<void()>> async_weak_assign;
    MapToSharedPtr map_to_shared_ptr{{nameTypeid<Group>(), &dataset->acquisition.groups},
                                     {nameTypeid<Probe>(), &dataset->acquisition.probes},
                                     {nameTypeid<Excitation>(), &dataset->acquisition.excitations},
                                     {nameTypeid<SuperGroup>(), &dataset->acquisition.super_groups},
                                     {nameTypeid<std::function<void()>>(), &async_weak_assign}};

    urx::utils::io::reader::DeserializeHdf5<Dataset, AllTypeInVariant>::f(
        "dataset", *dataset, file, map_to_shared_ptr, getMemberMap());

    for (auto& funct : async_weak_assign) {
      funct();
    }
  } catch (const H5::FileIException&) {
    throw urx::utils::ReadFileException("Failed to read " + filename + ".");
  }

  if (dataset->version.major != uac::UAC_VERSION_MAJOR) {
    return {};
  }

  dataset->version.minor = uac::UAC_VERSION_MINOR;
  dataset->version.patch = uac::UAC_VERSION_PATCH;

  return dataset;
}

}  // namespace uac::utils::io::reader
