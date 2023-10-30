
#include <valhalla/model.h>
#include <valhalla/model_function.h>
#include <valhalla/status_flag.h>
#include <valhalla/tensor.h>


#include <iostream>
#include <vector>
#include <memory>

#include <cstdio>

int main() {

  //valhalla::Model m("data/trained_models/1epochs/model.tflite", 4);
  auto file = fopen("model.tflite", "rb");
  fseek(file, 0, SEEK_END);
  int flen = (int)ftell(file);
  std::cout << "File length: " << flen << std::endl;
  fseek(file, 0, SEEK_SET);
  char *buffer = new char[flen];
  fread(buffer, 1, flen, file);
  valhalla::Model m(buffer, flen, 4);

  for(int i = 0; i < m.GetSignatureCount(); i++)
    std::cout << m.GetSignatureKey(i) << std::endl;

  
  std::unique_ptr<valhalla::ModelFunction> func = m.GetSignatureRunner("train");
  std::cout << "dims: " << func->GetInputCount() << std::endl;
  std::cout << func->GetInputName(0) << std::endl;
  /*
  std::unique_ptr<valhalla::Tensor> tensor = func->GetInputTensor("images");
  std::cout << tensor->NumDims() << std::endl;

  std::cout << tensor->DimLength(0) << ", " << tensor->DimLength(1) << ", "
            << tensor->DimLength(2) << ", " << tensor->DimLength(3)
            << std::endl;

  int x[4] = {1, 48, 48, 3};
  func->ResizeInputTensor("images", &(x[0]), 4);
  */
  // Allocate tensors and populate the input tensor data.
  /// TfLiteInterpreterAllocateTensors(interpreter);
  valhalla::StatusFlag sf = func->AllocateTensors();
  std::cout << sf << std::endl;

  std::unique_ptr<valhalla::Tensor> tensor = func->GetInputTensor("images");
  
  //float *input = new float[48 * 48 * 3];
  //std::vector<float> input(48 * 48 * 3);
  //input[0] = 7.f;

  //sf = tensor->CopyFromBuffer(input, (48 * 48 * 3 * (int)sizeof(float)));
  //sf = tensor->CopyFromBuffer(input.data(), ((int)input.size() * (int)sizeof(float)));

  std::cout << sf << std::endl;

  sf = (*func)();

  std::cout << sf << std::endl;
  return 777;

  std::unique_ptr<valhalla::Tensor> tensor_out = func->GetOutputTensor("output_0");
  std::cout << tensor_out->NumDims() << std::endl;
  std::cout << tensor_out->DimLength(0) << ", " << tensor_out->DimLength(1)
            << std::endl;

  /*
  float *out = new float[1 * 64 * 108];

  tensor_out->CopyToBuffer(out, (1 * 64 * 108 * (int)sizeof(float)));

  std::cout << out[0] << std::endl;
  */
  /*
  TfLiteTensor* input_tensor =
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
  */


  return 0;
}
