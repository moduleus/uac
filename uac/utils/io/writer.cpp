#include <uac/dataset.h>
#include <uac/utils/io/writer.h>
#include <uac/utils/io/writer_impl.h>  // IWYU pragma: keep

namespace uac::utils::io::writer {

void saveToFile(const std::string& filename, const Dataset& dataset) {
  uac::utils::io::WriterDataset writer;
  writer.write(filename, dataset);
}

}  // namespace uac::utils::io::writer
