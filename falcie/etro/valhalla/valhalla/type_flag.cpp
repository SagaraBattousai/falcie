
#include <tensorflow/lite/c/c_api_types.h>
#include <valhalla/type_flag.h>

namespace valhalla {
TypeFlag FromTfLiteType(int tfLiteType) noexcept {
  switch (static_cast<TfLiteType>(tfLiteType)) {
    case (kTfLiteFloat32):
      return TypeFlag::kFloat32;
    case (kTfLiteInt32):
      return TypeFlag::kInt32;
    case (kTfLiteUInt8):
      return TypeFlag::kUInt8;
    case (kTfLiteInt64):
      return TypeFlag::kInt64;
    case (kTfLiteString):
      return TypeFlag::kString;
    case (kTfLiteBool):
      return TypeFlag::kBool;
    case (kTfLiteInt16):
      return TypeFlag::kInt16;
    case (kTfLiteComplex64):
      return TypeFlag::kComplex64;
    case (kTfLiteInt8):
      return TypeFlag::kInt8;
    case (kTfLiteFloat16):
      return TypeFlag::kFloat16;
    case (kTfLiteFloat64):
      return TypeFlag::kFloat64;
    case (kTfLiteComplex128):
      return TypeFlag::kComplex128;
    case (kTfLiteUInt64):
      return TypeFlag::kUInt64;
    case (kTfLiteResource):
      return TypeFlag::kResource;
    case (kTfLiteVariant):
      return TypeFlag::kVariant;
    case (kTfLiteUInt32):
      return TypeFlag::kUInt32;
    case (kTfLiteUInt16):
      return TypeFlag::kUInt16;
    case (kTfLiteInt4):
      return TypeFlag::kInt4;
    case (kTfLiteNoType):
      [[fallthrough]];
    default:
      return TypeFlag::kNoType;
  }
}

}  // namespace valhalla
