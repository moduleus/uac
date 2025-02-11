#include <algorithm>
#include <memory>
#include <optional>
#include <typeindex>
#include <utility>
#include <vector>

#include <H5Cpp.h>

#include <urx/probe.h>
#include <urx/utils/exception.h>
#include <urx/utils/io/serialize_helper.h>
#include <urx/utils/io/writer_impl.h>

#include <uac/acquisition.h>
#include <uac/dataset.h>
#include <uac/excitation.h>
#include <uac/group.h>
#include <uac/probe.h>
#include <uac/super_group.h>
#include <uac/utils/io/serialize_helper.h>
#include <uac/utils/io/writer.h>
#include <uac/utils/io/writer_impl.h>  // IWYU pragma: keep

namespace uac::utils::io::writer {

void saveToFile(const std::string& filename, const Dataset& dataset) {
  try {
    const H5::H5File file(filename.data(), H5F_ACC_TRUNC);

    MapToSharedPtr map_to_shared_ptr{{nameTypeid<Group>(), &dataset.acquisition.groups},
                                     {nameTypeid<Probe>(), &dataset.acquisition.probes},
                                     {nameTypeid<Excitation>(), &dataset.acquisition.excitations},
                                     {nameTypeid<SuperGroup>(), &dataset.acquisition.super_groups}};

    urx::utils::io::writer::SerializeHdf5<Dataset, AllTypeInVariant, ContainerType::RAW>::f(
        "dataset", dataset, file, map_to_shared_ptr, getMemberMap());
  } catch (const H5::FileIException&) {
    throw urx::utils::WriteFileException("Failed to write " + filename + ".");
  }
}

}  // namespace uac::utils::io::writer
