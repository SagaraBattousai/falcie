#include <valhalla/internal/tensor_impl.h>

#include <utility>

namespace valhalla {

TensorImpl::TensorImpl(TfLiteTensor*&& tensor)
    : tensor_(TfLiteTensor_ptr(std::move(tensor), ValhallaTfLiteTensorDeleter)) {}

}  // namespace valhalla