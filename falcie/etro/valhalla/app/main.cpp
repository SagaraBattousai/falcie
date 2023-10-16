
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>

#include <valhalla/federated_model.h>

int main() {

  valhalla::FederatedModel m("data/trained_models/1epochs/model.tflite");

   _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
  _CrtDumpMemoryLeaks();

  //return 0;
}
