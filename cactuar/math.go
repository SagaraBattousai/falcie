package cactuar

import "math"

const (
	exponentOffset uint32 = 23
	lowerByteMask  uint32 = 255
	exponentBias   uint32 = 127
	exponentClear  uint32 = ^uint32(255 << 23)
	exponentMask   uint32 = 127 << 23
	//Bad names but since they're just polynomial constants that have been
	//calculated with remez they're somewhat arbitary
	a float32 = -0.344845
	b float32 = 2.024658
	c float32 = 1.674873
)

//It's Log 2 btw
func fastlog[T ~float32](diff T) T {
	bits := math.Float32bits(float32(diff))

	//Below is the exponent, usually its exponent -1
	//but this time our mantissa calculation doesn't add 1
	//implicitly.
	characteristic := ((bits >> exponentOffset) & lowerByteMask) - exponentBias

	bits &= exponentClear
	bits += exponentMask // (is this the same as OR (||) since we zeroed it?)
	f := math.Float32frombits(bits)
	//mantissa = ((-1.0/3)*f+2)*f - 2.0/3 //The +1 version
	mantissa := ((a*f+b)*f - c)
	return T(float32(characteristic) + mantissa)
}
