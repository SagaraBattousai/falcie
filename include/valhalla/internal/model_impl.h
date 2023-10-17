// Private Header: Do *NOT* include this file in any public header
// Only Private headers or Implementation files (CPP's)

#ifndef __VALHALLA_INTERNAL_MODEL_IMPL_H__
#define __VALHALLA_INTERNAL_MODEL_IMPL_H__

#include <_falcie_config.h>
#include <tensorflow/lite/c/c_api.h>

#include <memory>

#include <valhalla/model_function.h>

namespace valhalla {

class FALCIE_LOCAL ModelImpl {
 public:
  ModelImpl(const char *filename);

  // Can't use default int or pass to ^^ constructor as this one is slightly
  // less efficient due to not initing interpreter in constructor init.
  ModelImpl(const char *filename, int num_threads);

  // TODO: Check if Signatures can be re run and if so keep a shared ptr list of
  // them!
  //  Or possibly unique if they are only called from here!
  //
  //  std::unique_ptr<TfLiteSignatureRunner,
  //  decltype(tfLiteSignatureRunnerDelete)> GetSignatureRunner(const char
  //  *signature_key);
  std::unique_ptr<ModelFunction> GetSignatureRunner(const char *signature_name);

  // private:
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