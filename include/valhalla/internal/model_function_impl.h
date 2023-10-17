// Private Header: Do *NOT* include this file in any public header
// Only Private headers or Implementation files (CPP's)

#ifndef __VALHALLA_INTERNAL_MODEL_FUNCTION_IMPL_H__
#define __VALHALLA_INTERNAL_MODEL_FUNCTION_IMPL_H__

#include <_falcie_config.h>
#include <tensorflow/lite/c/c_api.h>

#include <memory>

namespace valhalla {

class FALCIE_LOCAL ModelFunctionImpl {
 public:

  // std::unique_ptr<TfLiteSignatureRunner,
  // decltype(&TfLiteSignatureRunnerDelete)> GetSignatureRunner(const char
  // *signature_key);

 private:
  friend class ModelImpl;

  ModelFunctionImpl(const ModelImpl& model, const char* signature_name);

  
  using TfLiteSignatureRunner_ptr =
      std::unique_ptr<TfLiteSignatureRunner,
                      decltype(&TfLiteSignatureRunnerDelete)>;

  TfLiteSignatureRunner_ptr func_;
};

}  // namespace valhalla

#endif