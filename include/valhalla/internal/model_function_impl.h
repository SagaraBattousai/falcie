// Private Header: Do *NOT* include this file in any public header
// Only Private headers or Implementation files (CPP's)

#ifndef __VALHALLA_INTERNAL_MODEL_FUNCTION_IMPL_H__
#define __VALHALLA_INTERNAL_MODEL_FUNCTION_IMPL_H__

#include <_falcie_config.h>
#include <tensorflow/lite/c/c_api.h>
#include <valhalla/internal/tensor_impl.h>
#include <valhalla/status_flag.h>

#include <cstdint>
#include <memory>

namespace valhalla {

constexpr auto ValhallaTfLiteSignatureRunnerDeleter =
    [](TfLiteSignatureRunner* sr) { TfLiteSignatureRunnerDelete(sr); };

class FALCIE_LOCAL ModelFunctionImpl {
 public:
  // From https://abseil.io/tips/134 : In many cases, marking a constructor
  // private is over-engineering. In those cases, the best solution is to mark
  // your constructors public and document their proper use.
  //
  // Constructor should only really be called from ModelImpl
  // aided by the fact neither class is exported
  ModelFunctionImpl(TfLiteInterpreter* interpreter, const char* signature_name);

  int GetInputCount() const {
    return (int)TfLiteSignatureRunnerGetInputCount(func_.get());
  }

  // This const char may have been malloc'd: investigate
  const char* GetInputName(int input_index) const {
    return TfLiteSignatureRunnerGetInputName(func_.get(),
                                             (std::int32_t)input_index);
  }

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

  StatusFlag ResizeInputTensor(const char* input_name, const int* input_dims,
                               int input_dims_size) {
    return FromTfLiteStatus(TfLiteSignatureRunnerResizeInputTensor(
        func_.get(), input_name, input_dims, (std::int32_t)input_dims_size));
  }

  /// Updates allocations for tensors associated with a signature and resizes
  /// dependent tensors using the specified input tensor dimensionality.
  /// This is a relatively expensive operation and hence should only be called
  /// after initializing the signature runner object and/or resizing any inputs.

  StatusFlag AllocateTensors() {
    return FromTfLiteStatus(TfLiteSignatureRunnerAllocateTensors(func_.get()));
  }

  /// Returns the input tensor identified by `input_name` in the given
  /// signature. Returns nullptr if the given name is not valid.
  ///
  /// NOTE: The lifetime of the returned tensor is the same as (and depends on)
  /// the lifetime of `signature_runner`.
  // std::unique_ptr<TensorImpl> GetInputTensor(const char* input_name) {
  //   return std::make_unique<TensorImpl>(  // TensorImpl::TfLiteTensor_ptr(
  //       TfLiteSignatureRunnerGetInputTensor(func_.get(), input_name));  //,
  //   //));  // ValhallaTfLiteTensorDeleter));
  // }

  std::unique_ptr<TensorImpl> GetInputTensor(const char* input_name) {
    return std::make_unique<TensorImpl>(
        TfLiteSignatureRunnerGetInputTensor(func_.get(), input_name));
  }

  /// Runs inference on a given signature.
  ///
  /// Before calling this function, the caller should first invoke
  /// TfLiteSignatureRunnerAllocateTensors() and should also set the values for
  /// the input tensors. After successfully calling this function, the values
  /// for the output tensors will be set.

  StatusFlag operator()() {
    return FromTfLiteStatus(TfLiteSignatureRunnerInvoke(func_.get()));
  }

  /// Returns the number of output tensors associated with the signature.
  int GetOutputCount() {
    return (int)TfLiteSignatureRunnerGetOutputCount(func_.get());
  }

  /// Returns the (null-terminated) name of the Nth output in a signature, where
  /// N is specified as `output_index`.
  ///
  /// NOTE: The lifetime of the returned name is the same as (and depends on)
  /// the lifetime of `signature_runner`.
  const char* GetOutputName(int output_index) {
    return TfLiteSignatureRunnerGetOutputName(func_.get(),
                                              (std::int32_t)output_index);
  }

  /// Returns the output tensor identified by `output_name` in the given
  /// signature. Returns nullptr if the given name is not valid.
  ///
  /// NOTE: The lifetime of the returned tensor is the same as (and depends on)
  /// the lifetime of `signature_runner`.
  ///
  /// In this case is it at all acceptable to cast away constness?
  // std::unique_ptr<const TensorImpl> GetOutputTensor(const char* output_name)
  // {
  // TODO: Work out all this constness!!!!
  std::unique_ptr<TensorImpl> GetOutputTensor(const char* output_name) {
    return std::make_unique<TensorImpl>(const_cast<TfLiteTensor*>(
        TfLiteSignatureRunnerGetOutputTensor(func_.get(), output_name)));
  }

  /// ///////////////////////////////////////////////////

 private:
  using TfLiteSignatureRunner_ptr =
      std::unique_ptr<TfLiteSignatureRunner,
                      decltype(ValhallaTfLiteSignatureRunnerDeleter)>;
  // decltype(&TfLiteSignatureRunnerDelete)>;

  TfLiteSignatureRunner_ptr func_;
};

}  // namespace valhalla

#endif