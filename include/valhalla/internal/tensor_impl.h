// Private Header: Do *NOT* include this file in any public header
// Only Private headers or Implementation files (CPP's)

#ifndef __VALHALLA_INTERNAL_TENSOR_IMPL_H__
#define __VALHALLA_INTERNAL_TENSOR_IMPL_H__

#include <_falcie_config.h>
#include <tensorflow/lite/c/c_api.h>
#include <tensorflow/lite/c/common.h>
#include <valhalla/status_flag.h>
#include <valhalla/type_flag.h>

#include <cstdint>
#include <functional>
#include <memory>

namespace valhalla {

// As far as I'm aware TfLiteTensor's don't need special dealocation (I shall
// check this!) Okay so it seems TF_LITE_STATIC_MEMORY is opt-in and only used
// for MICRO (which were not going to worry about for now!) therefore we can
// assume we need delete (but will common solve this?) Also remember that when
// TF_Lite decides to switch from non-opaque to opaque a deleter will surely
// be in both?
// importing common.h does work so that a mind F!
//
// Furthermore I cannot tell if it even needs to be deleted!!!!
//
// Debug the heck outta this if theres a problem else just get on with your PhD
// you have 7 months left!!!

constexpr auto ValhallaTfLiteTensorDeleter = [](TfLiteTensor* tensor) {
#ifndef TF_LITE_STATIC_MEMORY
  TfLiteTensorFree(tensor);
#else
  delete tensor;  //??????
#endif
};

class FALCIE_LOCAL TensorImpl {
 public:
  // public as A) It's internal and B) saves a lotta confusion/typing
  using TfLiteTensor_ptr =
      std::unique_ptr<TfLiteTensor, decltype(ValhallaTfLiteTensorDeleter)>;

  // From https://abseil.io/tips/134 : In many cases, marking a constructor
  // private is over-engineering. In those cases, the best solution is to mark
  // your constructors public and document their proper use.
  //
  // Constructor should only really be called from Other Impls
  // aided by the fact no impl class' are exported
  TensorImpl(TfLiteTensor*&& tensor);

  /// Returns the type of a tensor element.
  TypeFlag Type() { return FromTfLiteType(TfLiteTensorType(tensor_.get())); }

  /// Returns the number of dimensions that the tensor has.  Returns -1 in case
  /// the 'opaque_tensor' does not have its dimensions property set.
  int NumDims() { return (int)TfLiteTensorNumDims(tensor_.get()); }

  /// Returns the length of the tensor in the "dim_index" dimension.
  /// REQUIRES: 0 <= dim_index < TFLiteTensorNumDims(tensor)
  int DimLength(int dim_index) {
    return (int)TfLiteTensorDim(tensor_.get(), (std::int32_t)dim_index);
  }

  /// Returns the size of the underlying data in bytes.
  int ByteSize() { return (int)TfLiteTensorByteSize(tensor_.get()); }

  /// Returns a pointer to the underlying data buffer.
  ///
  /// \note The result may be null if tensors have not yet been allocated,
  /// e.g., if the Tensor has just been created or resized and
  /// `TfLiteAllocateTensors()` has yet to be called, or if the output tensor
  /// is dynamically sized and the interpreter hasn't been invoked.
  void* Data() { return TfLiteTensorData(tensor_.get()); }

  /// Returns the (null-terminated) name of the tensor.
  const char* Name() { return TfLiteTensorName(tensor_.get()); }

  /******************************************************************
 ////////////UNUSED FOR NOW! TFLITE MAY DEPRECATE///////////////////
 ////////////STRUCT (TfLiteQuantizationParams)//////////////////////
 // Returns the parameters for asymmetric quantization. The quantization
 // parameters are only valid when the tensor type is `kTfLiteUInt8` and the
 // `scale != 0`. Quantized values can be converted back to float using:
 //    real_value = scale * (quantized_value - zero_point);
 //TFL_CAPI_EXPORT extern TfLiteQuantizationParams
 //TfLiteTensorQuantizationParams(const TfLiteTensor* tensor);
 *******************************************************************/

  /// Copies from the provided input buffer into the tensor's buffer.
  /// REQUIRES: input_data_size == TfLiteTensorByteSize(tensor)
  StatusFlag CopyFromBuffer(const void* input_data, int input_data_size) {
    return FromTfLiteStatus(TfLiteTensorCopyFromBuffer(
        tensor_.get(), input_data, (size_t)input_data_size));
  }

  /// Copies to the provided output buffer from the tensor's buffer.
  /// REQUIRES: output_data_size == TfLiteTensorByteSize(tensor)
  StatusFlag CopyToBuffer(void* output_data, int output_data_size) {
    return FromTfLiteStatus(TfLiteTensorCopyToBuffer(tensor_.get(), output_data,
                                                     (size_t)output_data_size));
  }

 private:
  TfLiteTensor_ptr tensor_;
};

}  // namespace valhalla

#endif