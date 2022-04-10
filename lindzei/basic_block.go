package lindzei

import (
	"crypto/sha256"
	"fmt"
	"time"

	"github.com/sagarabattousai/falcie/pulse/cactuar"
)

const (
	basicBlockPrintf = "{\n" +
		"\tID: #%d\n" +
		"\tTimestamp: %d/%d/%d__%d:%d:%d-%d\n" +
		"\tData:  %s\n" +
		"\tHash:  %X\n" +
		"\tPHash: %X\n" +
		"\tNonce: %d\n" +
		"}"
)

//May not need to export (if so chain can be made non generic)
type BasicBlock struct {
	id        uint32
	timestamp int64
	data      []byte //Ummm, isnt this already here?
	hash      []byte //I think I want these to be sized
	prevhash  []byte
	nonce     uint64 //Might be too big etc Also may not want like this ?
}

func NewBasicBlock(data []byte) BasicBlock {
	return BasicBlock{timestamp: time.Now().UnixMilli(), data: data}
}

func CreateBasicBlockGenisis() BasicBlock {
	genisis := BasicBlock{id: 0,
		timestamp: time.Now().UnixMilli(),
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
	t := time.UnixMilli(b.timestamp)
	year, month, day := t.Date()
	hour, minute, second := t.Clock()
	milli := t.Nanosecond() / 1e6
	return fmt.Sprintf(basicBlockPrintf+"\n",
		b.id,
		day, month, year,
		hour, minute, second, milli,
		b.data, b.hash, b.prevhash,
		b.nonce)
}

func (b BasicBlock) GenerateHash(prevhash []byte) []byte {
	//b.prevhash = prevhash
	hash := sha256.New()

	encodedId := cactuar.Encode32(b.id)
	encodedTimestamp := cactuar.Encode64(b.timestamp)
	encodedNonce := cactuar.Encode64(b.nonce)

	hash.Write(encodedId[:])
	hash.Write(encodedTimestamp[:])
	hash.Write(b.data)
	// hash.Write(b.prevhash)
	hash.Write(prevhash)
	// b.hash = hash.Sum(nil)
	hash.Write(encodedNonce[:])
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
