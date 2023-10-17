
#include <valhalla/model_function.h>

#include <utility>

// Must be included before constructor and destructor so that ModelFunctionImpl
// is a defined type
#include <valhalla/internal/model_function_impl.h>

namespace valhalla {

ModelFunction::ModelFunction(std::unique_ptr<ModelFunctionImpl>&& func)
    : func_(std::move(func)) {}

ModelFunction::~ModelFunction() = default;

}  // namespace valhalla