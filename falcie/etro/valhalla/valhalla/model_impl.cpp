
#include <stdexcept>

#include <valhalla/internal/model_impl.h>

namespace valhalla {

ModelImpl::ModelImpl(const char *filename)
    : model_(TfLiteModel_ptr(TfLiteModelCreateFromFile(filename),
                             &TfLiteModelDelete)),
      options_(TfLiteOptions_ptr(TfLiteInterpreterOptionsCreate(),
                                 &TfLiteInterpreterOptionsDelete)),
      interpreter_(TfLiteInterpreter_ptr(
          TfLiteInterpreterCreate(model_.get(), options_.get()),
          &TfLiteInterpreterDelete)) {}

ModelImpl::ModelImpl(const char *filename, int num_threads)
    : model_(TfLiteModel_ptr(TfLiteModelCreateFromFile(filename),
                             &TfLiteModelDelete)),
      options_(TfLiteOptions_ptr(TfLiteInterpreterOptionsCreate(),
                                 &TfLiteInterpreterOptionsDelete)),
      interpreter_(nullptr, &TfLiteInterpreterDelete) {
 
  TfLiteInterpreterOptionsSetNumThreads(options_.get(), num_threads);
  interpreter_.reset(TfLiteInterpreterCreate(model_.get(), options_.get()));
}

ModelImpl::ModelImpl(const void *model_data, int model_size)
    : model_(TfLiteModel_ptr(TfLiteModelCreate(model_data, (size_t)model_size),
                             &TfLiteModelDelete)),
      options_(TfLiteOptions_ptr(TfLiteInterpreterOptionsCreate(),
                                 &TfLiteInterpreterOptionsDelete)),
      interpreter_(nullptr, &TfLiteInterpreterDelete) {}

ModelImpl::ModelImpl(const void *model_data, int model_size, int num_threads)
    : model_(TfLiteModel_ptr(TfLiteModelCreate(model_data, (size_t)model_size),
                             &TfLiteModelDelete)),
      options_(TfLiteOptions_ptr(TfLiteInterpreterOptionsCreate(),
                                 &TfLiteInterpreterOptionsDelete)),
      interpreter_(nullptr, &TfLiteInterpreterDelete) {

  TfLiteInterpreterOptionsSetNumThreads(options_.get(), num_threads);
  interpreter_.reset(TfLiteInterpreterCreate(model_.get(), options_.get()));
}

ModelImpl::~ModelImpl() = default;

std::unique_ptr<ModelFunctionImpl> ModelImpl::GetSignatureRunner(const char *signature_name) {
  return std::make_unique<ModelFunctionImpl>(interpreter_.get(), signature_name);
}

}  // namespace valhalla