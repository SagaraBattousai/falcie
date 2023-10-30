#ifndef __VALHALLA_MODEL_FUNCTION_H__
#define __VALHALLA_MODEL_FUNCTION_H__

#include <_falcie_config.h>

#include <memory>

#include <valhalla/status_flag.h>
#include <valhalla/tensor.h>

namespace valhalla {

// Forward declare PImpl
class ModelFunctionImpl;

class ModelFunction {
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

  // This const char may have been malloc'd: investigate
  const char* GetInputName(int input_index) const;

  StatusFlag ResizeInputTensor(const char* input_name,
                                            const int* input_dims,
                        int input_dims_size);

  StatusFlag AllocateTensors();

  std::unique_ptr<Tensor> GetInputTensor(const char* input_name);

  StatusFlag operator()();

  int GetOutputCount();

  const char* GetOutputName(int output_index);

  //TODO: Const up??
  //std::unique_ptr<const Tensor> GetOutputTensor(const char* output_name);
  std::unique_ptr<Tensor> GetOutputTensor(const char* output_name);

 private:
  std::unique_ptr<ModelFunctionImpl> func_;
};

}  // namespace valhalla

#endif