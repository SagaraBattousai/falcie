
#include <valhalla/internal/model_function_impl.h>
#include <valhalla/internal/model_impl.h>

namespace valhalla {

ModelFunctionImpl::ModelFunctionImpl(TfLiteInterpreter* interpreter,
                                     const char* signature_name)
    : func_(TfLiteSignatureRunner_ptr(
          TfLiteInterpreterGetSignatureRunner(interpreter, signature_name),
          &TfLiteSignatureRunnerDelete)) {}

}  // namespace valhalla
