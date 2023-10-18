#ifndef __VALHALLA_MODEL_FUNCTION_H__
#define __VALHALLA_MODEL_FUNCTION_H__

#include <_falcie_config.h>

#include <memory>

namespace valhalla {

// Forward declare PImpl
class ModelFunctionImpl;

class FALCIE_EXPORT ModelFunction {
 public:
  // From https://abseil.io/tips/134 : In many cases, marking a constructor
  // private is over-engineering. In those cases, the best solution is to mark
  // your constructors public and document their proper use.
  //
  //Constructor should only really be called from Model
  // aided by the fact ModelFunctionImpl is not exported
  ModelFunction(std::unique_ptr<ModelFunctionImpl>&& func);

  ~ModelFunction();

  int GetInputCount() const;

  const char* GetInputName(std::int32_t input_index) const;

 private:
  std::unique_ptr<ModelFunctionImpl> func_;
};

}  // namespace valhalla

#endif