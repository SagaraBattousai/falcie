
#include <valhalla/model_function.h>
#include <valhalla/model.h>

#include <utility>

// Must be included before constructor and destructor so that ModelFunctionImpl
// is a defined type
#include <valhalla/internal/model_function_impl.h>
//#include <valhalla/internal/tensor_impl.h>

namespace valhalla {

ModelFunction::ModelFunction(std::unique_ptr<ModelFunctionImpl>&& func)
    : func_(std::move(func)) {}

ModelFunction::~ModelFunction() = default;

int ModelFunction::GetInputCount() const { return func_->GetInputCount(); }

const char* ModelFunction::GetInputName(int input_index) const {
  return func_->GetInputName(input_index);
}

StatusFlag ModelFunction::ResizeInputTensor(const char* input_name,
                                     const int* input_dims,
  int input_dims_size) {
  return func_->ResizeInputTensor(input_name, input_dims, input_dims_size);
}

/// /// TODO: Change return type to custom status flag
// TfLiteStatus
StatusFlag ModelFunction::AllocateTensors() { return func_->AllocateTensors(); }

std::unique_ptr<Tensor> ModelFunction::GetInputTensor(const char* input_name) {
  return std::make_unique<Tensor>(func_->GetInputTensor(input_name));
}

StatusFlag ModelFunction::operator()() { return (*func_)(); }

int ModelFunction::GetOutputCount() { return func_->GetOutputCount(); }

const char* ModelFunction::GetOutputName(int output_index) {
  return func_->GetOutputName(output_index);
}

std::unique_ptr<Tensor> ModelFunction::GetOutputTensor(
  const char* output_name) {
  return std::make_unique<Tensor>(
             func_->GetOutputTensor(output_name));
}

}  // namespace valhalla