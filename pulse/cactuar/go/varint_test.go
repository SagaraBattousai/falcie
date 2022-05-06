package cactuar

import (
	"bytes"
	"testing"
)

const (
	toVarInt    = 300
	fromVarIntH = 0b10101100
	fromVarIntL = 0b00000010
)

func TestToVarInt(t *testing.T) {
	var actual VarInt
	expected := []byte{fromVarIntH, fromVarIntL}

	actual = ToVarInt(uint(toVarInt))
	if !bytes.Equal(actual, expected) {
		t.Fatalf("ToVarInt(%d) = %b but wanted %b", toVarInt, actual, expected)
	}

	actual = ToVarInt(uint16(toVarInt))
	if !bytes.Equal(actual, expected) {
		t.Fatalf("ToVarInt(%d) = %b but wanted %b", toVarInt, actual, expected)
	}

	actual = ToVarInt(uint32(toVarInt))
	if !bytes.Equal(actual, expected) {
		t.Fatalf("ToVarInt(%d) = %b but wanted %b", toVarInt, actual, expected)
	}

	actual = ToVarInt(uint64(toVarInt))
	if !bytes.Equal(actual, expected) {
		t.Fatalf("ToVarInt(%d) = %b but wanted %b", toVarInt, actual, expected)
	}

	actual = ToVarInt(uintptr(toVarInt))
	if !bytes.Equal(actual, expected) {
		t.Fatalf("ToVarInt(%d) = %b but wanted %b", toVarInt, actual, expected)
	}
}

////////////////////////////////////////////////////////////////////////////

func TestFromVarInt(t *testing.T) {
	fromVarInt := []byte{fromVarIntH, fromVarIntL}

	actualUint := FromVarInt[uint](fromVarInt)
	if actualUint != toVarInt {
		t.Fatalf("FromVarInt(%b) = %d but wanted %d",
			fromVarInt, actualUint, toVarInt)
	}

	actualUint16 := FromVarInt[uint16](fromVarInt)
	if actualUint16 != toVarInt {
		t.Fatalf("FromVarInt(%b) = %d but wanted %d",
			fromVarInt, actualUint16, toVarInt)
	}

	actualUint32 := FromVarInt[uint32](fromVarInt)
	if actualUint32 != toVarInt {
		t.Fatalf("FromVarInt(%b) = %d but wanted %d",
			fromVarInt, actualUint32, toVarInt)
	}

	actualUint64 := FromVarInt[uint64](fromVarInt)
	if actualUint64 != toVarInt {
		t.Fatalf("FromVarInt(%b) = %d but wanted %d",
			fromVarInt, actualUint64, toVarInt)
	}

	actualUintptr := FromVarInt[uintptr](fromVarInt)
	if actualUintptr != toVarInt {
		t.Fatalf("FromVarInt(%b) = %d but wanted %d",
			fromVarInt, actualUintptr, toVarInt)
	}
}
