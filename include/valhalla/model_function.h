#ifndef __VALHALLA_MODEL_FUNCTION_H__
#define __VALHALLA_MODEL_FUNCTION_H__

#include <_falcie_config.h>

#include <functional>
#include <memory>

namespace valhalla {

class FALCIE_EXPORT ModelFunction {
 public:
  // Constructors and Destructors must be defined in cpp when ModelImpl is
  // complete
  FederatedModel(const char *filename);
  // Can't use default int or pass to ^^ constructor as this one is slightly
  // less efficient due to not initing interpreter in constructor init.
  FederatedModel(const char *filename, int num_threads);
  ~FederatedModel();

  // tflite::SignatureRunner *GetSignatureRunner();

 private:
  class ModelFunctionImpl;
  std::unique_ptr<ModelImpl> model_;
};

}  // namespace valhalla

#endif