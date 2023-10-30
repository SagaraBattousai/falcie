
#include <valhalla/model.h>
#include <valhalla/model_function.h>

// Must be included before constructor and destructor so that ModelImpl is a
// defined type
#include <valhalla/internal/model_impl.h>

namespace valhalla {

Model::Model(const char *filename)
    : model_(std::make_unique<ModelImpl>(filename)) {}

Model::Model(const char *filename, int num_threads)
    : model_(std::make_unique<ModelImpl>(filename, num_threads)) {}

Model::Model(const void *model_data, int model_size, int num_threads)
    : model_(std::make_unique<ModelImpl>(model_data, model_size, num_threads)) {}


Model::~Model() = default;

int Model::GetSignatureCount() { return model_->GetSignatureCount(); }

const char* Model::GetSignatureKey(int signature_index) {
  return model_->GetSignatureKey(signature_index);
}

std::unique_ptr<ModelFunction> Model::GetSignatureRunner(const char *signature_name) {
  return make_unique<ModelFunction>(model_->GetSignatureRunner(signature_name));
}


}  // namespace valhalla