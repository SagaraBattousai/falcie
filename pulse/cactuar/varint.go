package cactuar

import (
	"golang.org/x/exp/constraints"
)

const (
	defaultVarIntCapacity = 9
	mask                  = 0b01111111
)

type VarInt []byte

//func [I constraints.Integer] ToVarInt(value I) VarInt {
func ToVarInt[U constraints.Unsigned](value U) VarInt {
	out := make([]byte, 0, defaultVarIntCapacity)
	for bits := value; bits != 0; bits >>= 7 {
		out = append(out, byte((bits&mask)|0x80))
	}

	out[len(out)-1] ^= 0x80

	return VarInt(out)

}

func FromVarInt[U constraints.Unsigned](value VarInt) U {
	var out U
	for i, bytes := range value {
		out |= U(bytes&mask) << (7 * i)
	}
	return out
}
