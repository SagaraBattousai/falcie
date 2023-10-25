
#include <valhalla/internal/model_function_impl.h>

namespace valhalla {

ModelFunctionImpl::ModelFunctionImpl(TfLiteInterpreter* interpreter,
                                     const char* signature_name)
    : func_(TfLiteSignatureRunner_ptr(
          TfLiteInterpreterGetSignatureRunner(interpreter, signature_name),
          ValhallaTfLiteSignatureRunnerDeleter)) {}
}  // namespace valhalla
