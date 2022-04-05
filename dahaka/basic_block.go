package dahaka

import (
	"crypto/sha256"
	"fmt"
	"time"
)

const (
	BASIC_BLOCK_PRINTF = "{\n" +
		"\tID: #%d\n" +
		"\tTimestamp: %d/%d/%d__%d:%d:%d\n" +
		"\tData: %s\n" +
		"\tHash:  %X\n" +
		"\tPHash: %X\n" +
		"}"
)

//May not need to export (if so chain can be made non generic)
type BasicBlock struct {
	id        uint32
	timestamp int64  //may need to breakdown even smaller
	data      []byte //Ummm, isnt this already here?
	hash      []byte //I think I want these to be sized
	prevhash  []byte
}

func NewBasicBlock(data []byte) BasicBlock {
	return BasicBlock{timestamp: time.Now().Unix(), data: data}
}

func CreateBasicBlockGenisis() BasicBlock {
	genisis := BasicBlock{id: 0,
		timestamp: time.Now().Unix(),
		data:      []byte{},
		prevhash:  []byte{},
	}
	genisis.hash = genisis.GenerateHash([]byte{})
	return genisis
}

//may be able to make interface level or generic if really sneaky
func BBFactory(block BasicBlock, id uint32, prevhash []byte) BasicBlock {
	block.id = id
	block.prevhash = prevhash
	block.hash = block.GenerateHash(prevhash)
	return block
}

func (b BasicBlock) String() string {
	t := time.Unix(b.timestamp, 0)
	year, month, day := t.Date()
	hour, minute, second := t.Clock()
	return fmt.Sprintf(BASIC_BLOCK_PRINTF+"\n",
		b.id,
		day, month, year,
		hour, minute, second,
		b.data, b.hash, b.prevhash)
}

func (b BasicBlock) GenerateHash(prevhash []byte) []byte {
	//b.prevhash = prevhash
	hash := sha256.New()

	encodedId := EncodeId(b.id)
	encodedTimestamp := EncodeTimestamp(b.timestamp)

	hash.Write(encodedId[:])
	hash.Write(encodedTimestamp[:])
	hash.Write(b.data)
	// hash.Write(b.prevhash)
	hash.Write(prevhash)
	// b.hash = hash.Sum(nil)
	return hash.Sum(nil)
	//return b
}

func (b BasicBlock) Id() uint32 {
	return b.id
}

func (b BasicBlock) Timestamp() int64 {
	return b.timestamp
}

func (b BasicBlock) Data() []byte {
	return b.data
}

func (b BasicBlock) Hash() []byte {
	return b.hash
}

func (b BasicBlock) Prevhash() []byte {
	return b.prevhash
}
