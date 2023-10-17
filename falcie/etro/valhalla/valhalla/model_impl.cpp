
#include <valhalla/internal/model_impl.h>
#include <valhalla/internal/model_function_impl.h>

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


std::unique_ptr<ModelFunction> ModelImpl::GetSignatureRunner(
  const char* signature_name)
{
  return std::make_unique<ModelFunction>(std::make_unique<ModelFunctionImpl>(ModelFunctionImpl(*this, signature_name))
  );
}



}  // namespace valhalla