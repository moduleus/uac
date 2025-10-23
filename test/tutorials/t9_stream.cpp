#include <cstddef>
#include <ios>
#include <iostream>
#include <memory>
#include <numeric>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/event.h>
#include <urx/group.h>
#include <urx/receive_setup.h>
#include <urx/utils/io/reader_options.h>
#include <urx/utils/io/stream.h>
#include <urx/utils/io/writer.h>
#include <urx/utils/io/writer_options.h>

#include <uac/dataset.h>
#include <uac/utils/io/reader.h>
#include <uac/utils/to_urx.h>

int main() {
  const std::string uac_filename = "./t8-cpp.uac";
  const std::string urx_filename = "./t9-cpp.urx";

  // Generate urx without group_data
  const std::shared_ptr<uac::Dataset> uac_dataset =
      uac::utils::io::reader::loadFromFile(uac_filename);
  const std::shared_ptr<urx::Dataset> urx_dataset = uac::utils::toUrx(*uac_dataset);

  // The urx file must exist and be saved before using stream.
  urx::utils::io::writer::saveToFile(urx_filename, *urx_dataset, {false, false, false});

  const size_t group_idx = 0;

  const size_t sequence_size =
      std::accumulate(urx_dataset->acquisition.groups[group_idx]->sequence.begin(),
                      urx_dataset->acquisition.groups[group_idx]->sequence.end(),
                      static_cast<size_t>(0), [](size_t start, const urx::Event& event) {
                        return start + event.receive_setup.active_elements.size() *
                                           event.receive_setup.number_samples;
                      });
  const std::shared_ptr<urx::RawData> raw_data_vector_short =
      std::make_shared<urx::RawDataVector<short>>(sequence_size);
  short* buf = static_cast<short*>(raw_data_vector_short->getBuffer());
  for (size_t i = 0; i < sequence_size; i++) {
    buf[i] = static_cast<short>(i);
  }

  {
    // Stream and GroupDataStream keep an HDF5 object. So destroy it to release lock on file.
    urx::utils::io::Stream stream(urx_filename, urx_dataset);

    stream.writerOptions().setChunkGroupData(true);
    urx::utils::io::GroupDataStream group_data =
        stream.createGroupData(urx_dataset->acquisition.groups[group_idx], urx::DoubleNan(2.));

    // It's not mandatory to write the full sequence at once.
    group_data.append(raw_data_vector_short, 9., {8.});
  }

  {
    const std::shared_ptr<urx::Dataset> dataset_loaded = std::make_shared<urx::Dataset>();
    urx::utils::io::Stream stream(urx_filename, dataset_loaded);
    stream.readerOptions().setRawDataLoadPolicy(urx::utils::io::RawDataLoadPolicy::STREAM);
    stream.loadFromFile();

    const size_t group_data_idx = 0;

    // Buffer must reserve memory.
    const std::shared_ptr<urx::RawData> buffer =
        std::make_shared<urx::RawDataVector<short>>(sequence_size);

    // It's not mandatory to write the full sequence at once.
    stream.readRawData(group_data_idx, buffer,
                       0,   // Index of the sequence in the buffer.
                       0,   // Index of the sequence in the urx file.
                       1);  // Count of sequence to read.

    if (*buffer != *raw_data_vector_short) {
      std::cout << "Something went wrong.\n";
    }
  }

  return 0;
}
