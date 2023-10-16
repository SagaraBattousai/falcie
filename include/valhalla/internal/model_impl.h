// Private Header: Do *NOT* include this file in any public header
// Only Private headers or Implementation files (CPP's)

#ifndef __VALHALLA_INTERNAL_MODEL_IMPL_H__
#define __VALHALLA_INTERNAL_MODEL_IMPL_H__

#include <_falcie_config.h>
#include <tensorflow/lite/c/c_api.h>

#include <memory>

namespace valhalla {


namespace {
auto tfLiteModelDeleter = [](TfLiteModel *model) { TfLiteModelDelete(model); };

auto tfLiteOptionsDeleter = [](TfLiteInterpreterOptions *options) {
TfLiteInterpreterOptionsDelete(options);
};

auto tfLiteInterpreterDeleter = [](TfLiteInterpreter *interpreter) {
TfLiteInterpreterDelete(interpreter);
};

// TODO: Do I actually need to wrap with a lambda?
auto tfLiteSignatureRunnerDeleter = [](TfLiteSignatureRunner *runner) {
  TfLiteSignatureRunnerDelete(runner);
};

}  // namespace


class FALCIE_LOCAL ModelImpl {
 public:
  ModelImpl(const char *filename);

  // Can't use default int or pass to ^^ constructor as this one is slightly
  // less efficient due to not initing interpreter in constructor init.
  ModelImpl(const char *filename, int num_threads);

  // std::unique_ptr<TfLiteSignatureRunner,
  // decltype(tfLiteSignatureRunnerDelete)> GetSignatureRunner(const char
  // *signature_key);

 private:
  // decltype(tfLiteModelDeleter)>;
  using TfLiteModel_ptr =
      std::unique_ptr<TfLiteModel, decltype(tfLiteModelDeleter)>;
  //decltype(&TfLiteModelDelete) > ;

  using TfLiteOptions_ptr =
      std::unique_ptr<TfLiteInterpreterOptions, decltype(tfLiteOptionsDeleter)>;
  //decltype(&TfLiteInterpreterOptionsDelete) > ;
  // decltype(tfLiteOptionsDeleter)>;

  // decltype(tfLiteInterpreterDeleter)>;
  using TfLiteInterpreter_ptr =
      std::unique_ptr<TfLiteInterpreter, decltype(tfLiteInterpreterDeleter)>;
  //decltype(&TfLiteInterpreterDelete) > ;

  TfLiteModel_ptr model_;
  TfLiteOptions_ptr options_;
  TfLiteInterpreter_ptr interpreter_;
};

}  // namespace valhalla

#endif