package cactuar

import (
	//"fmt"
	"math"
)

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

//type Cactuar float32
//Trying to remember if I should use type in order to
//show that its not for all float32

//It's Log 2 btw
func fastlog(diff float32) float32 {
	bits := math.Float32bits(diff)

	//Below is the exponent, usually its exponent -1
	//but this time our mantissa calculation doesn't add 1
	//implicitly.
	characteristic := ((bits >> exponentOffset) & lowerByteMask) - exponentBias

	bits &= exponentClear
	bits += exponentMask // (is this the same as OR (||) since we zeroed it?)
	f := math.Float32frombits(bits)
	//v = ((-1.0/3)*f+2)*f - 2.0/3
	mantissa := ((a*f+b)*f - c)
	return float32(characteristic) + mantissa
}
