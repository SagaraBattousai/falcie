package dahaka

type Block interface {
	Id() uint32
	Timestamp() int64 //aka the result of -> time.Time.Unix
	Data() []byte     //But keep this to show additional data
	Hash() []byte
	Prevhash() []byte
	//Or no args??
	GenerateHash(prevhash []byte) []byte
	//PoW Branch
	Mine(difficulty uint32) Block //I hate interface func that returns itself
}

type GenisisFactory[T Block] func() T
type BlockFactory[T Block] func(block T, id uint32, prevhash []byte) T
