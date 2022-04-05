package dahaka

type Block interface {
	Id() uint32
	Timestamp() int64 //aka the result of -> time.Time.Unix
	Data() []byte     //But keep this to show additional data
	Hash() []byte
	Prevhash() []byte
	//Or no args??
	GenerateHash(prevhash []byte) []byte
}

type GenisisFactory[T Block] func() T
type BlockFactory[T Block] func(block T, id uint32, prevhash []byte) T

//Default Encoding's and Decodings's
//Use Little endian encoding (Doesn't matter as long as encoded and decoded
//By the pair. i.e. even if x is in big endian, encoding it as little endian
//and then decoding as little endian will restore it as it was.

func EncodeId(id uint32) [4]byte {
	var b [4]byte
	b[0] = byte(id)
	b[1] = byte(id >> 8)
	b[2] = byte(id >> 16)
	b[3] = byte(id >> 24)

	return b
}

func DecodeId(id [4]byte) uint32 {
	return uint32(id[3])<<24 |
		uint32(id[2])<<16 |
		uint32(id[1])<<8 |
		uint32(id[0])
}

func EncodeTimestamp(id int64) [8]byte {
	var b [8]byte
	b[0] = byte(id)
	b[1] = byte(id >> 8)
	b[2] = byte(id >> 16)
	b[3] = byte(id >> 24)
	b[4] = byte(id >> 32)
	b[5] = byte(id >> 40)
	b[6] = byte(id >> 48)
	b[7] = byte(id >> 56)

	return b
}

func DecodeTimestamp(timestamp [8]byte) int64 {
	return int64(timestamp[7])<<56 |
		int64(timestamp[6])<<48 |
		int64(timestamp[5])<<40 |
		int64(timestamp[4])<<32 |
		int64(timestamp[3])<<24 |
		int64(timestamp[2])<<16 |
		int64(timestamp[1])<<8 |
		int64(timestamp[0])
}
