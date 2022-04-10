package dahaka

import (
	"bytes"
	"crypto/sha256"
	"fmt"
	"time"

	"github.com/sagarabattousai/falcie/pulse/cactuar"
)

const (
	blockHeaderPrintf = "{\n" +
		"\tVersion: %X\n" +
		"\tTimestamp: %d/%d/%d__%d:%d:%d-%d\n" +
		"\tPrevious Hash:       %X\n" +
		"\tTransactional Hash:  %X\n" +
		"\tTarget:              %X\n" +
		"\tNonce:               %d\n" +
		"}"
)

type BlockHeader struct {
	//Potentially useful to identify issues in previous software
	Version uint32
	//The timestamp in milliseconds since the Unix Epoch
	//(good for ~ 292 Million years)
	Timestamp int64
	Prevhash  [32]byte
	Transhash [32]byte
	Target    cactuar.Cactuar
	//Could go back to uint64 but since time etc can change no need
	Nonce uint32
}

func (blockHeader *BlockHeader) Mine() {

	//TODO: add error for invalid cactaur values
	difficultyArray := blockHeader.Target.As256Bit()

	headerHash := blockHeader.Hash()

	for bytes.Compare(headerHash, difficultyArray[:]) != -1 {
		blockHeader.Nonce += 1
		headerHash = blockHeader.Hash()
	}
}

func (blockHeader *BlockHeader) Hash() []byte {
	hash := sha256.New()
	versionBytes := cactuar.Encode32(blockHeader.Version)
	timestampBytes := cactuar.Encode64(blockHeader.Timestamp)
	targetBytes := cactuar.Encode32(blockHeader.Target)
	nonceBytes := cactuar.Encode32(blockHeader.Nonce)

	hash.Write(versionBytes[:])
	hash.Write(timestampBytes[:])
	hash.Write(blockHeader.Prevhash[:])
	hash.Write(blockHeader.Transhash[:])
	hash.Write(targetBytes[:])
	hash.Write(nonceBytes[:])

	return hash.Sum(nil)
}

func (blockHeader *BlockHeader) String() string {
	t := time.UnixMilli(blockHeader.Timestamp)
	year, month, day := t.Date()
	hour, minute, second := t.Clock()
	milli := t.Nanosecond() / 1e6
	return fmt.Sprintf(blockHeaderPrintf+"\n",
		blockHeader.Version,
		day, month, year,
		hour, minute, second, milli,
		blockHeader.Prevhash, blockHeader.Transhash, blockHeader.Target,
		blockHeader.Nonce)
}
