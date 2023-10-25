
#include <utility>

#include <valhalla/tensor.h>

#include <valhalla/internal/tensor_impl.h>

namespace valhalla {

Tensor::Tensor(std::unique_ptr<TensorImpl>&& tensor)
    : tensor_(std::move(tensor)) {}

Tensor ::~Tensor() = default;

TypeFlag Tensor::Type() { return tensor_->Type(); }

// TODO: Note there actually may be a case to be specific on int width as
// there is an underlying implementation + can't garuntee always on 32bit or
// greater machine

/// Returns the number of dimensions that the tensor has.  Returns -1 in case
/// its dimensions property is not set.
int Tensor::NumDims() { return tensor_->NumDims(); };

// TODO Could convert to vector

/// Returns the length of the tensor in the "dim_index" dimension.
/// Pre: 0 <= dim_index < TFLiteTensorNumDims(tensor)
int Tensor::DimLength(int dim_index) { return tensor_->DimLength(dim_index); }

/// Returns the size of the underlying data in bytes.
int Tensor::ByteSize() { return tensor_->ByteSize(); }

// TODO: make template With backing void* version !!!!!! or have TypeFlag
// perform cast?

/// Returns a pointer to the underlying data buffer.
///
/// \note The result may be null if tensors have not yet been allocated, e.g.,
/// if the Tensor has just been created or resized and
/// `TfLiteAllocateTensors()` has yet to be called, or if the output tensor is
/// dynamically sized and the interpreter hasn't been invoked.
void* Tensor::Data() { return tensor_->Data(); }

/// Returns the (null-terminated) name of the tensor.
const char* Tensor::Name() { return tensor_->Name(); }

// Removed TfLiteTensorQuantizationParams() (see comments in _impl)

/// Copies from the provided input buffer into the tensor's buffer.
/// REQUIRES: input_data_size == TfLiteTensorByteSize(tensor)
StatusFlag Tensor::CopyFromBuffer(const void* input_data, int input_data_size) {
  return tensor_->CopyFromBuffer(input_data, input_data_size);
}

StatusFlag Tensor::CopyToBuffer(void* output_data, int output_data_size) {
  return tensor_->CopyToBuffer(output_data, output_data_size);
}


}  // namespace valhalla