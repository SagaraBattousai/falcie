
#include <iostream>

#include <valhalla/federated_model.h>

int main() {

  valhalla::FederatedModel m("data/trained_models/1epochs/model.tflite");

  auto sig = m.GetSignatureRunner("patch");


  return 0;
}
