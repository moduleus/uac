#include <algorithm>

#include <urx/utils/io/writer_options.h>

#include <uac/dataset.h>
#include <uac/utils/io/writer.h>
#include <uac/utils/io/writer_impl.h>  // IWYU pragma: keep
#include <uac/utils/validator.h>

namespace uac::utils::io::writer {

void saveToFile(const std::string& filename, const Dataset& dataset,
                const urx::utils::io::WriterOptions& options) {
  uac::utils::io::WriterDataset writer;
  writer.setOptions(options);
  if (options.getCheckData()) {
    ValidatorReport validator;
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    validator.check(const_cast<Dataset&>(dataset));
    validator.throwIfFailure();
  }
  writer.write(filename, dataset);
}

}  // namespace uac::utils::io::writer
