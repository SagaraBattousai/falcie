// Private Header: Do *NOT* include this file in any public header
// Only Private headers or Implementation files (CPP's)

#ifndef __VALHALLA_INTERNAL_MODEL_FUNCTION_IMPL_H__
#define __VALHALLA_INTERNAL_MODEL_FUNCTION_IMPL_H__

#include <_falcie_config.h>
#include <tensorflow/lite/c/c_api.h>

#include <cstdint>
#include <memory>

namespace valhalla {

class FALCIE_LOCAL ModelFunctionImpl {
 public:

   // From https://abseil.io/tips/134 : In many cases, marking a constructor
  // private is over-engineering. In those cases, the best solution is to mark
  // your constructors public and document their proper use.
  //
  // Constructor should only really be called from ModelImpl
  // aided by the fact neither class is exported
   ModelFunctionImpl(TfLiteInterpreter* interpreter, const char* signature_name);

   //~ModelFunctionImpl() = default;

  int GetInputCount() const {
    return (int)TfLiteSignatureRunnerGetInputCount(func_.get());
  }

  // This const char may have been malloc'd: investigate
  const char* GetInputName(std::int32_t input_index) const {
    return TfLiteSignatureRunnerGetInputName(func_.get(), input_index);
  }

  /*
  /// Resizes the input tensor identified as `input_name` to be the dimensions
  /// specified by `input_dims` and `input_dims_size`. Only unknown dimensions
  /// can be resized with this function. Unknown dimensions are indicated as
  /// `-1` in the `dims_signature` attribute of a TfLiteTensor.
  ///
  /// Returns status of failure or success. Note that this doesn't actually
  /// resize any existing buffers. A call to
  /// TfLiteSignatureRunnerAllocateTensors() is required to change the tensor
  /// input buffer.
  ///
  /// NOTE: This function is similar to
  /// TfLiteInterpreterResizeInputTensorStrict() and not
  /// TfLiteInterpreterResizeInputTensor().
  ///
  /// NOTE: `input_name` must match the name of an input in the signature.
  ///
  /// NOTE: This function makes a copy of the input dimensions, so the caller
  /// can safely deallocate `input_dims` immediately after this function
  /// returns.
  TFL_CAPI_EXPORT extern TfLiteStatus TfLiteSignatureRunnerResizeInputTensor(
      TfLiteSignatureRunner* signature_runner, const char* input_name,
      const int* input_dims, int32_t input_dims_size);

  /// Updates allocations for tensors associated with a signature and resizes
  /// dependent tensors using the specified input tensor dimensionality.
  /// This is a relatively expensive operation and hence should only be called
  /// after initializing the signature runner object and/or resizing any inputs.
  TFL_CAPI_EXPORT extern TfLiteStatus TfLiteSignatureRunnerAllocateTensors(
      TfLiteSignatureRunner* signature_runner);

  /// Returns the input tensor identified by `input_name` in the given
  /// signature. Returns nullptr if the given name is not valid.
  ///
  /// NOTE: The lifetime of the returned tensor is the same as (and depends on)
  /// the lifetime of `signature_runner`.
  TFL_CAPI_EXPORT extern TfLiteTensor* TfLiteSignatureRunnerGetInputTensor(
      TfLiteSignatureRunner* signature_runner, const char* input_name);

  /// Runs inference on a given signature.
  ///
  /// Before calling this function, the caller should first invoke
  /// TfLiteSignatureRunnerAllocateTensors() and should also set the values for
  /// the input tensors. After successfully calling this function, the values
  /// for the output tensors will be set.
  TFL_CAPI_EXPORT extern TfLiteStatus TfLiteSignatureRunnerInvoke(
      TfLiteSignatureRunner* signature_runner);

  /// Returns the number of output tensors associated with the signature.
  TFL_CAPI_EXPORT extern size_t TfLiteSignatureRunnerGetOutputCount(
      const TfLiteSignatureRunner* signature_runner);

  /// Returns the (null-terminated) name of the Nth output in a signature, where
  /// N is specified as `output_index`.
  ///
  /// NOTE: The lifetime of the returned name is the same as (and depends on)
  /// the lifetime of `signature_runner`.
  TFL_CAPI_EXPORT extern const char* TfLiteSignatureRunnerGetOutputName(
      const TfLiteSignatureRunner* signature_runner, int32_t output_index);

  /// Returns the output tensor identified by `output_name` in the given
  /// signature. Returns nullptr if the given name is not valid.
  ///
  /// NOTE: The lifetime of the returned tensor is the same as (and depends on)
  /// the lifetime of `signature_runner`.
  TFL_CAPI_EXPORT extern const TfLiteTensor*
  TfLiteSignatureRunnerGetOutputTensor(
      const TfLiteSignatureRunner* signature_runner, const char* output_name);
  */
  /// ///////////////////////////////////////////////////

 private:

  using TfLiteSignatureRunner_ptr =
      std::unique_ptr<TfLiteSignatureRunner,
                      decltype(&TfLiteSignatureRunnerDelete)>;

  TfLiteSignatureRunner_ptr func_;
};

}  // namespace valhalla

#endif