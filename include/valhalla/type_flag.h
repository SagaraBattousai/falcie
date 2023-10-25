#ifndef __VALHALLA_TYPE_FLAG_H__
#define __VALHALLA_TYPE_FLAG_H__

namespace valhalla {

enum class TypeFlag {
  kNoType = 0,  // 0b0000'0000'0000'0000 //kTfLiteNoType = 0,
  // 1 - 18 is reserved for kTfLite values!
  kInt = 64,    // 0b0000'0000'0100'0000
  kInt4 = 65,   // 0b0000'0000'0100'0001 // kTfLiteInt4 = 18,
  kInt8 = 66,   // 0b0000'0000'0100'0010 // kTfLiteInt8 = 9,
  kInt16 = 68,  // 0b0000'0000'0100'0100 // kTfLiteInt16 = 7,
  kInt32 = 72,  // 0b0000'0000'0100'1000 // kTfLiteInt32 = 2,
  kInt64 = 80,  // 0b0000'0000'0101'0000 // kTfLiteInt64 = 4,
  // kInt128 = 96,     // 0b0000'0000'0110'0000 // doesn't exist
  kUInt = 128,  // 0b0000'0000'1000'0000
  // kUInt4 = 129,     // 0b0000'0000'1000'0001 // doesn't exist
  kUInt8 = 130,   // 0b0000'0000'1000'0010 // kTfLiteUInt8 = 3,
  kUInt16 = 132,  // 0b0000'0000'1000'0100 // kTfLiteUInt16 = 17,
  kUInt32 = 136,  // 0b0000'0000'1000'1000 // kTfLiteUInt32 = 16,
  kUInt64 = 144,  // 0b0000'0000'1001'0000 // kTfLiteUInt64 = 13,
  kFloat = 256,   // 0b0000'0001'0000'0000
  // kFloat4 = 257,    // 0b0000'0001'0000'0001 // doesn't exist
  // kFloat8 = 258,    // 0b0000'0001'0000'0010 // doesn't exist
  kFloat16 = 260,  // 0b0000'0001'0000'0100 // kTfLiteFloat16 = 10,
  kFloat32 = 264,  // 0b0000'0001'0000'1000 // kTfLiteFloat32 = 1,
  kFloat64 = 272,  // 0b0000'0001'0001'0000 // kTfLiteFloat64 = 11,
  // kFloat128 = 288,  // 0b0000'0001'0010'0000 // doesn't exist
  kComplex = 512,  // 0b0000'0010'0000'0000
  // kComplex4 = 513,  // 0b0000'0010'0000'0001 // doesn't exist
  // kComplex8 = 514,  // 0b0000'0010'0000'0010 // doesn't exist
  // kComplex16 = 516, // 0b0000'0010'0000'0100 // doesn't exist
  // kComplex32 = 520, // 0b0000'0010'0000'1000 // doesn't exist
  kComplex64 = 528,   // 0b0000'0010'0001'0000 // kTfLiteComplex64 = 8,
  kComplex128 = 544,  // 0b0000'0010'0010'0000 // kTfLiteComplex128 = 12,
  kBool = 1024,       // 0b0000'0100'0000'0000 // kTfLiteBool = 6,
  kString = 2048,     // 0b0000'1000'0000'0000 // kTfLiteString = 5,
  kResource = 4096,   // 0b0001'0000'0000'0000 // kTfLiteResource = 14,
  kVariant = 8192,    // 0b0010'0000'0000'0000 // kTfLiteVariant = 15,
};

// Two options, constexpr but can't use direct tf enum values so we'll use
// implfile

TypeFlag FromTfLiteType(int tfLiteType) noexcept;

}  // namespace valhalla

#endif