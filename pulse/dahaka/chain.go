package dahaka

import (
	"bytes"
)

const (
	//For now; rather arbitary, may make 16. Keep y = 2^x ?
	//To simplify things chain elems is also number of block before
	//difficulty is recalculated.
	CHAIN_MAX_ELEMENTS = 32 //may make 1024
	//32 = 48 minutes, 1024 = 25.6
)

//Unrolled Linked List
type Blockchain struct {
	head *Chain
	tail *Chain
}

//Unrolled Linked List Node
type Chain struct {
	next         *Chain
	elementCount int
	elements     [CHAIN_MAX_ELEMENTS]*FederatedBlock
}

func (bc *Blockchain) Head() *Chain {
	return bc.head
}
func (bc *Blockchain) Tail() *Chain {
	return bc.tail
}

func NewBlockchain(genisis *FederatedBlock) *Blockchain {

	chain := new(Chain)
	chain.elements[0] = genisis
	chain.elementCount = 1
	return &Blockchain{head: chain, tail: chain}
}

func (bc *Blockchain) addNewChain() *Chain {
	newChain := new(Chain)

	//Set last chain's next (must currently be nil) to point to newChain
	prevChain := bc.tail
	prevChain.next = newChain

	//Set Blockchain's tail to be newChain
	bc.tail = newChain

	//assert: returned value == bc.tail
	return newChain
}

//TODO: Stick on mutex's?
//I think this code was wrong!
func (bc *Blockchain) AddBlock(block *FederatedBlock) {
	chain := bc.tail
	//	prevHeader := chain.elements[chain.elementCount-1].Header()

	if (chain.elementCount % CHAIN_MAX_ELEMENTS) == 0 {
		//Add new chain
		chain = bc.addNewChain()

		//Update Difficulty

	}

	block.Mine()

	if chain.elementCount == CHAIN_MAX_ELEMENTS {
		chain = bc.addNewChain()
	}
	chain.elements[chain.elementCount] = block
	chain.elementCount++
}

//Also slow af if used many times
//Are we alowed to get arbitary Block? thought we usually only need to rec/iter?
func (bc *Blockchain) GetBlock(index int) *FederatedBlock {
	nodeCount := index / CHAIN_MAX_ELEMENTS
	elemIndex := index % CHAIN_MAX_ELEMENTS
	node := bc.head
	for i := 0; i < nodeCount; i++ {
		node = node.next
	}
	return node.elements[elemIndex]
}

func (bc *Blockchain) Walk(ch chan *FederatedBlock) {
	//could make chain but would then would use recursion
	var walk = func(bc *Blockchain, ch chan *FederatedBlock) {
		chain := bc.head
		for chain != nil {
			//_, block := range chain.elements {
			for i := 0; i < chain.elementCount; i++ {
				ch <- chain.elements[i] //block
			}
			chain = chain.next
		}
	}
	walk(bc, ch)
	close(ch)
}

func (bc *Blockchain) ForEach(f func(*FederatedBlock)) {
	ch := make(chan *FederatedBlock, 32)
	go bc.Walk(ch)

	for x := range ch {
		f(x)
	}
}

func (bc *Blockchain) Validate() bool {
	chain := bc.head
	var prevBlock, currBlock *FederatedBlock

	var i int = 1 //In order to skip genisis in first chain

	for chain != nil {
		for i < chain.elementCount-1 {
			prevBlock = chain.elements[i]
			currBlock = chain.elements[i+1]

			if !(bytes.Equal(prevBlock.Hash()[:],
				currBlock.BlockHeader.Prevhash[:]) &&
				bytes.Compare(currBlock.Hash()[:],
					currBlock.BlockHeader.Target.As256Bit()[:]) == -1) {
				return false
			}
			i++
		}
		i = 0
		chain = chain.next
	}
	return true
}
