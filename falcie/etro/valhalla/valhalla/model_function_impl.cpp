
#include <valhalla/internal/model_function_impl.h>
#include <valhalla/internal/model_impl.h>

namespace valhalla {

ModelFunctionImpl::ModelFunctionImpl(const ModelImpl& model,
                                     const char* signature_name)
    : func_(TfLiteSignatureRunner_ptr(
          TfLiteInterpreterGetSignatureRunner(model.interpreter_.get(),
                                              signature_name),
          &TfLiteSignatureRunnerDelete)) {}

}  // namespace valhalla
