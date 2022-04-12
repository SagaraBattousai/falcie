package dahaka

import (
	"time"

	"github.com/sagarabattousai/falcie/pulse/cactuar"
)

const (
	FederatedBlockId        uint32 = 0x43616C6F
	FederatedBlockChainSize        = 32
)

type FederatedBlock struct {
	//Blocksize uint32
	*BlockHeader
	//transactionCounter varint
	//transactions collection_of_transaction
}

func (_ *FederatedBlock) BlockTypeId() uint32 {
	return FederatedBlockId
}

func NewFederatedBlock(version uint32) *FederatedBlock {
	timestamp := time.Now().UnixMilli()
	blockHeader := &BlockHeader{Version: version,
		Timestamp: timestamp,
		Target:    cactuar.BaseDifficulty,
		Nonce:     0,
	}
	return &FederatedBlock{BlockHeader: blockHeader}
}

func GenisisFederatedBlock() *FederatedBlock {
	genisis := NewFederatedBlock(0x00000000)
	genisis.Target = 0
	return genisis
}
