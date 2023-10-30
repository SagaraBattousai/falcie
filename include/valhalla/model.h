#ifndef __VALHALLA_MODEL_H__
#define __VALHALLA_MODEL_H__

#include <_falcie_config.h>
#include <valhalla/model_function.h>

#include <memory>

namespace valhalla {

// Forward declare PImpl
class ModelImpl;

// template class FALCIE_EXPORT std::unique_ptr<ModelFunction>;

// Actually, for now (since this class is just a wrapper) we shall remove
// inheritance stuff
//class FALCIE_EXPORT Model {
class Model {
 public:
  explicit Model(const char *filename);
  // Model(const char *filename);
  //  Can't use default int or pass to ^^ constructor as this one (VV) is
  //  slightly less efficient due to not initing interpreter in constructor
  //  init.
  Model(const char *filename, int num_threads);
  Model(const void *model_data, int model_size, int num_threads);

  virtual ~Model();

  // for now use unique_tpr but probably change to shared
  // Make inline so the stl part is created in the client code and therefore
  // does not cross the dll boundary
  std::unique_ptr<ModelFunction> GetSignatureRunner(const char *signature_name);

  /// Returns the number of signatures defined in the model.
  int GetSignatureCount();

  /// Returns the key of the Nth signature in the model, where N is specified as
  /// `signature_index`.
  ///
  /// NOTE: The lifetime of the returned key is the same as (and depends on) the
  /// lifetime of `interpreter`.
  const char *GetSignatureKey(int signature_index);

 private:
  std::unique_ptr<ModelImpl> model_;
};

}  // namespace valhalla

#endif