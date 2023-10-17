
#include <valhalla/model.h>

// Must be included before constructor and destructor so that ModelImpl is a
// defined type
#include <valhalla/internal/model_impl.h>

namespace valhalla {

Model::Model(const char *filename)
    : model_(std::make_unique<ModelImpl>(filename)) {}

Model::Model(const char *filename, int num_threads)
    : model_(std::make_unique<ModelImpl>(filename, num_threads)) {}

Model::~Model() = default;

std::unique_ptr<ModelFunction> Model::GetSignatureRunner(
    const char *signature_name) {
  return model_->GetSignatureRunner(signature_name);
}

}  // namespace valhalla