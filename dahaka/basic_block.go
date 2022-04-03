package dahaka

import (
	"crypto/sha256"
	"fmt"
	"time"
)

const (
	BASIC_BLOCK_PRINTF = "{\n\t%d\n\t%d/%d/%d__%d:%d:%d\n\t%s\n\t%X\n\t%X\n}"
)

//May not need to export (if so chain can be made non generic)
type BasicBlock struct {
	id        uint32
	timestamp int64
	data      []byte //Ummm, isnt this already here?
	hash      []byte //I think I want these to be sized
	prevhash  []byte
}

func CreateBasicBlockGenisis() BasicBlock {
	genisis := BasicBlock{id: 0,
		timestamp: time.Now().Unix(),
		data:      []byte{},
		prevhash:  []byte{},
	}
	return genisis.GenerateHash([]byte{}).(BasicBlock)
}

func (b BasicBlock) String() string {
	t := time.Unix(b.timestamp, 0)
	year, month, day := t.Date()
	hour, minute, second := t.Clock()
	return fmt.Sprintf(BASIC_BLOCK_PRINTF+"\n",
		b.id, day, month, year, hour, minute, second, b.data, b.hash, b.prevhash)
}

//Maybe we should consider pointer recivers.... Does doing all this reall
//make it immutable more than using pointers and only exposing functions
//that are "safe"?
func (b BasicBlock) GenerateHash(prevhash []byte) Block {
	b.prevhash = prevhash
	hash := sha256.New()

	encodedId := EncodeId(b.id)
	encodedTimestamp := EncodeTimestamp(b.timestamp)

	hash.Write(encodedId[:])
	hash.Write(encodedTimestamp[:])
	hash.Write(b.data)
	hash.Write(b.prevhash)
	b.hash = hash.Sum(nil)
	return b
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
