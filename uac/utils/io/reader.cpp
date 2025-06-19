#include <memory>

#include <uac/dataset.h>
#include <uac/utils/io/reader.h>
#include <uac/utils/io/reader_impl.h>  // IWYU pragma: keep

namespace uac::utils::io::reader {

std::shared_ptr<Dataset> loadFromFile(const std::string& filename) {
  std::shared_ptr<Dataset> dataset = std::make_shared<Dataset>();
  uac::utils::io::ReaderDataset reader;
  reader.read(filename, *dataset);
  return dataset;
}

}  // namespace uac::utils::io::reader
