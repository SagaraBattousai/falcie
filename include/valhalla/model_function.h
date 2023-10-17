#ifndef __VALHALLA_MODEL_FUNCTION_H__
#define __VALHALLA_MODEL_FUNCTION_H__

#include <_falcie_config.h>

#include <memory>

namespace valhalla {

// Forward declare PImpl
class ModelFunctionImpl;

class FALCIE_EXPORT ModelFunction {
 public:
  // Destructor must be defined in cpp when ModelImpl is complete
  ~ModelFunction();

  // tflite::SignatureRunner *GetSignatureRunner();
  ModelFunction(std::unique_ptr<ModelFunctionImpl>&& func);

 private:
   //Will this stop clients being able to have extra power
   //since if Model was the friend they could.....?
  //friend class ModelImpl;

  // Constructors must be defined in cpp when ModelImpl is complete
  //ModelFunction(std::unique_ptr<ModelFunctionImpl>&& func);


  std::unique_ptr<ModelFunctionImpl> func_;
};

}  // namespace valhalla

#endif