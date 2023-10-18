// Private Header: Do *NOT* include this file in any public header
// Only Private headers or Implementation files (CPP's)

#ifndef __VALHALLA_INTERNAL_MODEL_IMPL_H__
#define __VALHALLA_INTERNAL_MODEL_IMPL_H__

#include <_falcie_config.h>
#include <tensorflow/lite/c/c_api.h>

#include <memory>

#include <valhalla/internal/model_function_impl.h>


namespace valhalla {

class FALCIE_LOCAL ModelImpl {
 public:
  // ctors have same issues as model

  ModelImpl(const char *filename);
  ModelImpl(const char *filename, int num_threads);

  ~ModelImpl();

  // TODO: Check if Signatures can be re run and if so keep a shared ptr list of
  // them! Or possibly unique if they are only called from here!
  std::unique_ptr<ModelFunctionImpl> GetSignatureRunner(
      const char *signature_name);

 private:
  using TfLiteModel_ptr =
      std::unique_ptr<TfLiteModel, decltype(&TfLiteModelDelete)>;

  using TfLiteOptions_ptr =
      std::unique_ptr<TfLiteInterpreterOptions,
                      decltype(&TfLiteInterpreterOptionsDelete)>;

  using TfLiteInterpreter_ptr =
      std::unique_ptr<TfLiteInterpreter, decltype(&TfLiteInterpreterDelete)>;

  TfLiteModel_ptr model_;
  TfLiteOptions_ptr options_;
  TfLiteInterpreter_ptr interpreter_;
};

}  // namespace valhalla

#endif