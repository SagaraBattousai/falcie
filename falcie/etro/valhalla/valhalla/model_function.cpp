
#include <valhalla/model_function.h>
#include <valhalla/model.h>

#include <utility>

// Must be included before constructor and destructor so that ModelFunctionImpl
// is a defined type
#include <valhalla/internal/model_function_impl.h>

namespace valhalla {

ModelFunction::ModelFunction(std::unique_ptr<ModelFunctionImpl>&& func)
    : func_(std::move(func)) {}

ModelFunction::~ModelFunction() = default;

int ModelFunction::GetInputCount() const { return func_->GetInputCount(); }

const char* ModelFunction::GetInputName(std::int32_t input_index) const {
  return func_->GetInputName(input_index);
}

}  // namespace valhalla