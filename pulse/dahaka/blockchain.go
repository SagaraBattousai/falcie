package dahaka

import (
	"bytes"
)

type Blockchain struct {
	*Chain[*FederatedBlock]
	pool *TransactionPool
}

func calculateGlobalUpdate(block *FederatedBlock) {
	acc := 0
	variableCount := len(block.LocalUpdates[0].Fst)
	gu := make([]float32, variableCount)
	var lu []float32
	for _, v := range block.LocalUpdates {
		acc += v.Snd
		lu = v.Fst
		for j, _ := range gu {
			gu[j] += float32(v.Snd) * lu[j]
		}
	}

	for j, _ := range gu {
		gu[j] /= float32(acc)
	}

	block.GlobalUpdate = gu
}

func NewBlockchain(genisis *FederatedBlock) *Blockchain {
	chain := NewChain[*FederatedBlock](FederatedBlockChainSize)
	chain.Add(genisis)
	return &Blockchain{Chain: chain}
}

//TODO: Stick in mutex's?
//I think this code was wrong!
func (blockchain *Blockchain) AddBlock(block *FederatedBlock) {
	block.BlockHeader.Prevhash = blockchain.GetLatest().Hash()

	//TODO:Update Difficulty

	calculateGlobalUpdate(block)
	block.Mine()

	//TODO:Set everything else required on block

	blockchain.Add(block)
}

func (blockchain *Blockchain) Validate() bool {
	node := blockchain.head
	var prevBlock, currBlock *FederatedBlock

	var i int = 1 //In order to skip genisis in first chain

	for node != nil {
		for i < len(node.elements)-1 {
			prevBlock = node.elements[i]
			currBlock = node.elements[i+1]

			//Annoying I cant use if's shortcircuit because I need an
			//address to then reference
			prevBlockHash := prevBlock.Hash()
			currBlockHash := currBlock.Hash()

			if !(bytes.Equal(prevBlockHash[:],
				currBlock.BlockHeader.Prevhash[:]) &&
				bytes.Compare(currBlockHash[:],
					currBlock.BlockHeader.Target.As256Bit()[:]) == -1) {
				return false
			}
			i++
		}
		i = 0
		node = node.next
	}
	return true
}
