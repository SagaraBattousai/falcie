#ifndef __VALHALLA_TENSOR_H__
#define __VALHALLA_TENSOR_H__

#include <_falcie_config.h>
#include <valhalla/type_flag.h>
#include <valhalla/status_flag.h>

#include <memory>

namespace valhalla {

// Forward declare PImpl
class TensorImpl;

class Tensor {
 public:
  // From https://abseil.io/tips/134 : In many cases, marking a constructor
  // private is over-engineering. In those cases, the best solution is to mark
  // your constructors public and document their proper use.
  //
  // Constructor should only really be called from Model
  // aided by the fact TensorImpl is not exported
  Tensor(std::unique_ptr<TensorImpl>&& tensor);

  ~Tensor();

  /// Returns the type of a tensor element.
  TypeFlag Type();

  // TODO: Note there actually may be a case to be specific on int width as
  // there is an underlying implementation + can't garuntee always on 32bit or
  // greater machine

  /// Returns the number of dimensions that the tensor has.  Returns -1 in case
  /// its dimensions property is not set.
  int NumDims();
  
  //TODO Could convert to vector

  /// Returns the length of the tensor in the "dim_index" dimension.
  /// Pre: 0 <= dim_index < TFLiteTensorNumDims(tensor)
  int DimLength(int dim_index);

  /// Returns the size of the underlying data in bytes.
  int ByteSize();

  //TODO: make template With backing void* version !!!!!! or have TypeFlag perform cast?

  /// Returns a pointer to the underlying data buffer.
  ///
  /// \note The result may be null if tensors have not yet been allocated, e.g.,
  /// if the Tensor has just been created or resized and
  /// `TfLiteAllocateTensors()` has yet to be called, or if the output tensor is
  /// dynamically sized and the interpreter hasn't been invoked.
  void* Data();

  /// Returns the (null-terminated) name of the tensor.
  const char* Name();

  //Removed TfLiteTensorQuantizationParams() (see comments in _impl)


  /// Copies from the provided input buffer into the tensor's buffer.
  /// REQUIRES: input_data_size == TfLiteTensorByteSize(tensor)
  StatusFlag CopyFromBuffer(const void* input_data,
                                         int input_data_size);

  /// Copies to the provided output buffer from the tensor's buffer.
  /// REQUIRES: output_data_size == TfLiteTensorByteSize(tensor)
  StatusFlag CopyToBuffer(void* output_data, int output_data_size);

 private:
  std::unique_ptr<TensorImpl> tensor_;
};

}  // namespace valhalla

#endif