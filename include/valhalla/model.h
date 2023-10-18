#ifndef __VALHALLA_MODEL_H__
#define __VALHALLA_MODEL_H__

#include <_falcie_config.h>
#include <valhalla/model_function.h>

#include <memory>

namespace valhalla {

// Forward declare PImpl
class ModelImpl;

class ModelFunction;

class FALCIE_EXPORT Model {
 public:
  Model(const char *filename);
  // Can't use default int or pass to ^^ constructor as this one (VV) is
  // slightly less efficient due to not initing interpreter in constructor init.
  Model(const char *filename, int num_threads);

  virtual ~Model();

  // for now use unique_tpr but probably change to shared
  virtual std::unique_ptr<ModelFunction> GetSignatureRunner(
      const char *signature_name);

 private:
  std::unique_ptr<ModelImpl> model_;
};

}  // namespace valhalla

#endif