#ifndef __VALHALLA_MODEL_H__
#define __VALHALLA_MODEL_H__

#include <_falcie_config.h>

#include <memory>

namespace valhalla {

// Forward declare PImpl
class ModelImpl;

class FALCIE_EXPORT Model {
 public:
  // Constructors and Destructors must be defined in cpp when ModelImpl is
  // complete
  Model(const char *filename);
  // Can't use default int or pass to ^^ constructor as this one is slightly
  // less efficient due to not initing interpreter in constructor init.
  Model(const char *filename, int num_threads);

  virtual ~Model();

  //Eventually we will have other virtual functions :)

 private:
  std::unique_ptr<ModelImpl> model_;
};

}  // namespace valhalla

#endif