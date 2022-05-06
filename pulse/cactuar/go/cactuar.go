package cactuar

// "fmt"
// "math"

//Cactuar formula = Θ * 2 ^ ( 4 * ( α - 4 ) )
//Cactuar formula = ψ * 2 ^ ( 4 * ( φ - 4 ) )
const (
	//Every 1 less exponent byte (starting at max 0x21) means an extra byte (of
	//zeros)
	BaseDifficulty Cactuar = 0x1F0FFF1D //0x1F00FFFF
	multiplierMask uint32  = 0x00FFFFFF
	//Same as bitcoin I assume as it scales the bits to bytes
	CactuarExpScalar        uint32 = 8
	CactuarExpShifter       uint32 = 4
	CactuarExpScaledShifter uint32 = 32
	//223 * 8 is the absolute maximum it can be!
	//223 * 8 means calculations are for 256 blocks.
	//as 8 * (255 - 3) = 2016 (bitcoins block check)
	//32 //Vs bitcoins 24 (more small numbers?)
	//BlockCheckCount uint32 = 1004 // 4 * ( 255 - 4 ) Aka Cactuar exponential
	//^^ Belongs to the Block / Chain not this class (for now) set block check
	//count to the length of the unrolled chain list which is of power 2
)

type Cactuar uint32
type Log2Cactuar float32

func (c Cactuar) AsPair() Pair[byte, uint32] {
	cactuarBits := uint32(c)
	exponent := byte((cactuarBits >> 24) & 255)
	multiplier := cactuarBits & multiplierMask

	return Pair[byte, uint32]{exponent, multiplier}
}

func (c Cactuar) Components() (uint32, uint32) {
	cactuarBits := uint32(c)
	exponent := (cactuarBits >> 24) & 255
	multiplier := cactuarBits & multiplierMask

	return exponent, multiplier
}

//Could be byte given Components already exists
func (c Cactuar) Exponent() uint32 {
	return (uint32(c) >> 24) & 255
}

//Little Endian
func (c Cactuar) MultiplierBytes() [3]byte {
	bits := uint32(c)
	var bytes [3]byte
	bytes[0] = byte(bits & 255)
	bytes[1] = byte((bits >> 8) & 255)
	bytes[2] = byte((bits >> 16) & 255)
	return bytes
}

//Little Endian
func (c Cactuar) AsBytes() [4]byte {
	bits := uint32(c)
	var bytes [4]byte
	bytes[0] = byte(bits & 255)
	bytes[1] = byte((bits >> 8) & 255)
	bytes[2] = byte((bits >> 16) & 255)
	bytes[3] = byte((bits >> 24) & 255)
	return bytes
}

//May be able to do this without explicit cast to array
//In BigEndian format due to the hash leading zeros style
func (c Cactuar) As256Bit() *[32]byte {
	var byteArray [32]byte //Auto Zero'd :D

	cactuarBytes := c.AsBytes()
	zeroByteCount := (uint32(cactuarBytes[3]) - CactuarExpShifter)
	arrIndex := uint32(31 - zeroByteCount)
	var i uint32
	for ; i < 3; i++ {
		byteArray[arrIndex-i] = cactuarBytes[i]
	}
	return &byteArray
}

func (c Cactuar) AsLog2() Log2Cactuar { //float32 {

	exponent, multiplier := c.Components()

	var logMultiplier float32

	if multiplier == 0 {
		logMultiplier = 0.0
	} else {
		logMultiplier = fastlog(float32(multiplier))
	}

	logExponent := float32(CactuarExpScalar*exponent - CactuarExpScaledShifter)

	return Log2Cactuar(logMultiplier + logExponent)
}

func (c Cactuar) LessThan(other Cactuar) bool {
	cLog2 := float32(c.AsLog2())
	otherLog2 := float32(other.AsLog2())

	return cLog2 < otherLog2
}

func (c Cactuar) GreaterThanOrEqualTo(other Cactuar) bool {
	cLog2 := float32(c.AsLog2())
	otherLog2 := float32(other.AsLog2())

	return cLog2 >= otherLog2
}

//Do we need to return error because of all the zeros ?
func (numerator Cactuar) Log2Div(denominator Cactuar) Log2Cactuar {
	numExp, numMult := numerator.Components()
	denExp, denMult := denominator.Components()

	var logMultiplier float32

	if numMult == 0 || denMult == 0 {
		logMultiplier = 0.0
	} else {
		logMultiplier = fastlog(float32(numMult) / float32(denMult))
	}

	logExponent := float32(CactuarExpScalar * (numExp - denExp))

	return Log2Cactuar(logMultiplier + logExponent)
}
