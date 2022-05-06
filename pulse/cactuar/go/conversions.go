package cactuar

import "math"

//Avoids using unsafe (i.e. we just specify for 32 and 64)
type B32 interface {
	~int32 | ~uint32
}

type B64 interface {
	~int64 | ~uint64
}

func Encode32[T B32](enc T) [4]byte {
	var b [4]byte
	b[0] = byte(enc)
	b[1] = byte(enc >> 8)
	b[2] = byte(enc >> 16)
	b[3] = byte(enc >> 24)

	return b
}

func Decode32[T B32](dec [4]byte) T {
	return T(dec[3])<<24 |
		T(dec[2])<<16 |
		T(dec[1])<<8 |
		T(dec[0])
}

func Encode64[T B64](enc T) [8]byte {
	var b [8]byte
	b[0] = byte(enc)
	b[1] = byte(enc >> 8)
	b[2] = byte(enc >> 16)
	b[3] = byte(enc >> 24)
	b[4] = byte(enc >> 32)
	b[5] = byte(enc >> 40)
	b[6] = byte(enc >> 48)
	b[7] = byte(enc >> 56)

	return b
}

func Decode64[T B64](dec [8]byte) T {
	return T(dec[7])<<56 |
		T(dec[6])<<48 |
		T(dec[5])<<40 |
		T(dec[4])<<32 |
		T(dec[3])<<24 |
		T(dec[2])<<16 |
		T(dec[1])<<8 |
		T(dec[0])
}

func EncodeFloat32[T ~float32](enc T) [4]byte {
	var bits uint32 = math.Float32bits(float32(enc))

	return Encode32(bits)
}

func DecodeFloat32[T ~float32](dec [4]byte) T {
	var bits uint32 = Decode32[uint32](dec)

	return T(math.Float32frombits(bits))
}

func EncodeFloat64[T ~float64](enc T) [8]byte {
	var bits uint64 = math.Float64bits(float64(enc))

	return Encode64(bits)
}

func DecodeFloat64[T ~float64](dec [8]byte) T {
	var bits uint64 = Decode64[uint64](dec)

	return T(math.Float64frombits(bits))
}
