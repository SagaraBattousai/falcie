package dahaka

import (
	"time"

	"github.com/sagarabattousai/falcie/pulse/cactuar"
)

//TODO: Stop using secp256r1 and use the k1 version bitcoin uses
//when you A) Understand it, eliptical curves and why bitcoin chose it
//and B) how to implement it myself

const (
	FederatedBlockId        uint32 = 0x43616C6F
	FederatedBlockChainSize        = 32
	//TODO: This is temp
	FederatedBlockMiningReward int32 = 37
)

type FederatedBlock struct {
	//Blocksize uint32
	*BlockHeader
	//transactionCounter varint
	*TransactionPool
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
	return &FederatedBlock{BlockHeader: blockHeader,
		TransactionPool: NewTransactionPool()}
}

func GenisisFederatedBlock() *FederatedBlock {
	genisis := NewFederatedBlock(0x00000000)
	genisis.Target = 0
	return genisis
}
