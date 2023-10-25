// #include <Windows.h>
#include <valhalla/internal/model_impl.h>

#include <stdexcept>

namespace valhalla {

// auto fd = [](TfLiteDelegate *ptr) { delete ptr; };

ModelImpl::ModelImpl(const char *filename)
    : model_(TfLiteModel_ptr(TfLiteModelCreateFromFile(filename),
                             &TfLiteModelDelete)),
      options_(TfLiteOptions_ptr(TfLiteInterpreterOptionsCreate(),
                                 &TfLiteInterpreterOptionsDelete)),
      // flex_(nullptr, fd),
      interpreter_(TfLiteInterpreter_ptr(
          TfLiteInterpreterCreate(model_.get(), options_.get()),
          &TfLiteInterpreterDelete)) {}

ModelImpl::ModelImpl(const char *filename, int num_threads)
    : model_(TfLiteModel_ptr(TfLiteModelCreateFromFile(filename),
                             &TfLiteModelDelete)),
      options_(TfLiteOptions_ptr(TfLiteInterpreterOptionsCreate(),
                                 &TfLiteInterpreterOptionsDelete)),
      // flex_(nullptr, fd),
      interpreter_(nullptr, &TfLiteInterpreterDelete) {
  /*
  auto hdll = LoadLibrary("tensorflowlite_flex.dll");



  auto fp = reinterpret_cast<TfLiteDelegatePtr(*)()>(
      GetProcAddress(hdll, "TF_AcquireFlexDelegate"));
  if (fp == NULL) {
    throw std::runtime_error("fp couldn't be run");
  }

  flex_ = fp();

  TfLiteInterpreterOptionsSetNumThreads(options_.get(), num_threads);

  TfLiteInterpreterOptionsAddDelegate(options_.get(), flex_.get());
  */

  interpreter_.reset(TfLiteInterpreterCreate(model_.get(), options_.get()));
}

ModelImpl::ModelImpl(const void *model_data, int model_size, int num_threads)
    : model_(TfLiteModel_ptr(TfLiteModelCreate(model_data, (size_t)model_size),
                             &TfLiteModelDelete)),
      options_(TfLiteOptions_ptr(TfLiteInterpreterOptionsCreate(),
                                 &TfLiteInterpreterOptionsDelete)),
      interpreter_(nullptr, &TfLiteInterpreterDelete) {
  interpreter_.reset(TfLiteInterpreterCreate(model_.get(), options_.get()));
}

ModelImpl::~ModelImpl() = default;

std::unique_ptr<ModelFunctionImpl> ModelImpl::GetSignatureRunner(
    const char *signature_name) {
  return std::make_unique<ModelFunctionImpl>(
      ModelFunctionImpl(interpreter_.get(), signature_name));
}

}  // namespace valhalla