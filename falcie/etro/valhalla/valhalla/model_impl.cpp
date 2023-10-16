
#include <valhalla/internal/model_impl.h>

namespace valhalla {

ModelImpl::ModelImpl(const char *filename)
    : model_(TfLiteModel_ptr(TfLiteModelCreateFromFile(filename),
                             tfLiteModelDeleter )), //& TfLiteModelDelete)),
      options_(TfLiteOptions_ptr(TfLiteInterpreterOptionsCreate(),
          tfLiteOptionsDeleter )),//& TfLiteInterpreterOptionsDelete)),
      interpreter_(TfLiteInterpreter_ptr(
          TfLiteInterpreterCreate(model_.get(), options_.get()),
          tfLiteInterpreterDeleter )) {} //& TfLiteInterpreterDelete)) {}

ModelImpl::ModelImpl(const char *filename, int num_threads)
    : model_(TfLiteModel_ptr(TfLiteModelCreateFromFile(filename),
                             tfLiteModelDeleter )),//& TfLiteModelDelete)),
      options_(TfLiteOptions_ptr(TfLiteInterpreterOptionsCreate(),
          tfLiteOptionsDeleter )),//& TfLiteInterpreterOptionsDelete)),
      interpreter_(nullptr,
                   tfLiteInterpreterDeleter) {// & TfLiteInterpreterDelete) {

  TfLiteInterpreterOptionsSetNumThreads(options_.get(), num_threads);
  interpreter_.reset(TfLiteInterpreterCreate(model_.get(), options_.get()));
}

/*
std::unique_ptr<TfLiteSignatureRunner, decltype(tfLiteSignatureRunnerDelete)>
FederatedModel::ModelImpl::GetSignatureRunner(const char *signature_key) {
  return std::unique_ptr<TfLiteSignatureRunner,
                         decltype(tfLiteSignatureRunnerDelete)>(TfLiteInterpreterGetSignatureRunner(this->interpreter_.get(),
signature_key);
}
*/
}  // namespace valhalla