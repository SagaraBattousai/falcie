
#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/interpreter_builder.h>
#include <tensorflow/lite/kernels/register.h>
#include <tensorflow/lite/model.h>
#include <tensorflow/lite/signature_runner.h>

#include <memory>
#include <iostream>

int main() {
  auto model = tflite::FlatBufferModel::BuildFromFile(
      "data/trained_models/1epochs/model.tflite");

  tflite::ops::builtin::BuiltinOpResolver resolver;

  tflite::InterpreterBuilder builder(*model,
                                     resolver);

  if (model == nullptr) {
    return 777;
  }
  std::unique_ptr<tflite::Interpreter> interpreter;

  if (builder(&interpreter) != kTfLiteOk) {
    throw std::runtime_error("Interpreter could not be build!");
  }

  tflite::SignatureRunner* runner = interpreter->GetSignatureRunner("patch");

  std::cout << runner->input_size() << std::endl;

  auto ten = runner->input_tensor("images");

  std::cout << ten->dims->data[0] << ", " << ten->dims->data[1] << ", " << ten->dims->data[2]
            << ", " << ten->dims->data[3]
            << std::endl;

  runner->ResizeInputTensor("images", {1, 48, 48, 3});

  if (runner->AllocateTensors() != kTfLiteOk) {
    return 777;
  }
  //auto input = runner->typed_tensor<float>(0);
  auto input = runner->input_tensor("images")->data.f;
  float x = input[0];
  std::cout << x << std::endl;
  
  runner->Invoke();

  //auto output = interpreter

  return 0;
}

/*
#include <valhalla/model.h>
#include <valhalla/model_function.h>
#include <valhalla/status_flag.h>
#include <valhalla/tensor.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <vector>

int main() {

  valhalla::Model m("data/trained_models/1epochs/model.tflite", 4);

  std::unique_ptr<valhalla::ModelFunction> func = m.GetSignatureRunner("patch");
  //("patch");
  int x[4] = {1, 48, 48, 3};
  func->ResizeInputTensor("images", &(x[0]), 4);

  // Allocate tensors and populate the input tensor data.
  /// TfLiteInterpreterAllocateTensors(interpreter);
  valhalla::StatusFlag sf = func->AllocateTensors();
  std::cout << sf << std::endl;

  std::unique_ptr<valhalla::Tensor> tensor = func->GetInputTensor("images");
  std::cout << tensor->NumDims() << std::endl;

  std::cout << tensor->DimLength(0) << ", " << tensor->DimLength(1) << ", "
            << tensor->DimLength(2) << ", " << tensor->DimLength(3)
            << std::endl;

  //float *input = new float[48 * 48 * 3];
  std::vector<float> input(48 * 48 * 3);
  input[0] = 7.f;

  //sf = tensor->CopyFromBuffer(input, (48 * 48 * 3 * (int)sizeof(float)));
  sf = tensor->CopyFromBuffer(input.data(), ((int)input.size() *
(int)sizeof(float)));

  std::cout << sf << std::endl;

  sf = (*func)();

  std::cout << sf << std::endl;

  std::unique_ptr<valhalla::Tensor> tensor_out =
func->GetOutputTensor("output_0");

  float *out = new float[1 * 64 * 108];

  tensor_out->CopyToBuffer(out, (1 * 64 * 108 * (int)sizeof(float)));

  std::cout << out[0] << std::endl;


 // TfLiteTensor* input_tensor =
///     TfLiteInterpreterGetInputTensor(interpreter, 0);
/// TfLiteTensorCopyFromBuffer(input_tensor, input.data(),
///                            input.size() * sizeof(float));
///
/// // Execute inference.
/// TfLiteInterpreterInvoke(interpreter);
///
/// // Extract the output tensor data.
/// const TfLiteTensor* output_tensor =
///      TfLiteInterpreterGetOutputTensor(interpreter, 0);
/// TfLiteTensorCopyToBuffer(output_tensor, output.data(),
///                          output.size() * sizeof(float));



  return 0;
}

*/