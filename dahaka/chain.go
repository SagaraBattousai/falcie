package dahaka

import "bytes"

/*
 * Must use generics so that oen cannot mix block concrete types on the chain
 * if it were just Blocks than a SimpleBlock and RandomBlock type could coexist
 * on the same Blockchain.
 */

const (
	//For now; rather arbitary, may make 16. Keep y = 2^x
	CHAIN_MAX_ELEMENTS = 32
)

//Unrolled Linked List
type Blockchain[T Block] struct {
	Head *Chain[T]
	Tail *Chain[T]
	//Latest ? i.e. reference to the latest
	factory BlockFactory[T]
}

//Do make slice so range works, then no need for element count but ...
//I like arrays more
//Unrolled Linked List Node
type Chain[T Block] struct {
	next         *Chain[T]
	elementCount int
	elements     [CHAIN_MAX_ELEMENTS]T
}

func NewBlockchain[T Block](genisis GenisisFactory[T],
	factory BlockFactory[T]) *Blockchain[T] {

	chain := new(Chain[T])
	chain.elements[0] = genisis()
	chain.elementCount = 1
	return &Blockchain[T]{Head: chain, Tail: chain, factory: factory}
}

func (bc *Blockchain[T]) addNewChain() *Chain[T] {
	newChain := new(Chain[T])

	//Set last chain's next (must currently be nil) to point to newChain
	prevChain := bc.Tail
	prevChain.next = newChain

	//Set Blockchain's Tail to be newChain
	bc.Tail = newChain

	//assert: returned value == bc.Tail
	return newChain
}

//TODO: Stick on mutex's?
func (bc *Blockchain[T]) AddBlock(block T) {
	chain := bc.Tail
	prevBlock := chain.elements[chain.elementCount-1]
	prevhash := prevBlock.Hash()
	nextId := prevBlock.Id() + 1
	//block = (block.GenerateHash(prevhash)).(T) //Why type assert with generics?
	block = bc.factory(block, nextId, prevhash)

	//index := chain.elementCount

	if chain.elementCount == CHAIN_MAX_ELEMENTS {
		chain = bc.addNewChain()
		//index := 0
	}

	chain.elements[chain.elementCount] = block
	chain.elementCount++
}

//Also slow af if used many times
//Are we alowed to get arbitary Block? thought we usually only need to rec/iter?
func (bc *Blockchain[T]) GetBlock(index int) T {
	nodeCount := index / CHAIN_MAX_ELEMENTS
	elemIndex := index % CHAIN_MAX_ELEMENTS
	node := bc.Head
	for i := 0; i < nodeCount; i++ {
		node = node.next
	}
	return node.elements[elemIndex]
}

func (bc *Blockchain[T]) Walk(ch chan T) {
	//could make chain but would then would use recursion
	var walk = func(bc *Blockchain[T], ch chan T) {
		chain := bc.Head
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

func (bc *Blockchain[T]) ForEach(f func(T)) {
	ch := make(chan T, 32)
	go bc.Walk(ch)

	for x := range ch {
		f(x)
	}
}

func (bc *Blockchain[T]) Validate() bool {
	chain := bc.Head
	//var i int = 0
	var prevBlock, currBlock T

	for chain != nil {
		for i := 0; i < chain.elementCount-1; i++ {
			prevBlock = chain.elements[i]
			currBlock = chain.elements[i+1]
			//Get the prevhash stored in current here to avoid calling twice
			currPrevHash := currBlock.Prevhash()
			if !(bytes.Compare(currBlock.Hash(),
				currBlock.GenerateHash(currPrevHash)) == 0 &&
				bytes.Compare(prevBlock.Hash(), currPrevHash) == 0) {
				return false
			}

		}
		chain = chain.next
	}
	return true
}
