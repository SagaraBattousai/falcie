package dahaka

//GENERICS AINT BAD FOR REDUCING TYPE ASSERTIONS ETC

const (
	//For now; rather arbitary, may make 16. Keep y = 2^x
	CHAIN_MAX_ELEMENTS = 32
)

//I Think I might be overdoing the generics here! No Its all good as
//individual blocks may have methods we cba to do type assertions on to get
//However since generics are (possibly) mainly to avoid writing the same func
//twice, maybe I am abusing it?

//Unrolled Linked List
type Blockchain[T Block] struct {
	Head *Chain[T]
	Tail *Chain[T]
	//Latest ? i.e. reference to the latest
}

//No Delete?!?! Right?
//May make private too?
//Unrolled Linked List Node
type Chain[T Block] struct {
	next         *Chain[T]
	elementCount int //Is this really needed? Makes indexing easier as len vs cap
	//is for slice really .... I like it I think, though Slice with set cap and
	//no append could also prove beneficial
	elements [CHAIN_MAX_ELEMENTS]T
}

//Probably dont need to make generic but that said would be nice to no need
//to cast once/if multiple block implementations exist
func NewBlockchain[T Block](genisis GenisisFactory[T]) *Blockchain[T] {
	chain := &Chain[T]{}
	//As brand new will/(should?) not need to do checks
	chain.elements[0] = genisis()
	chain.elementCount++
	return &Blockchain[T]{Head: chain, Tail: chain}
}

//For conveniance return new chain to then use
//Impure function and also returns :(
func (bc *Blockchain[T]) addNewChain() *Chain[T] {
	newChain := &Chain[T]{} //{next: nil, elementCount: 0}

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
	prevhash := chain.elements[chain.elementCount-1].Hash()
	block = (block.GenerateHash(prevhash)).(T) //Why type assert with generics?

	//index := chain.elementCount

	if chain.elementCount == CHAIN_MAX_ELEMENTS {
		chain = bc.addNewChain()
		//index := 0
	}

	chain.elements[chain.elementCount] = block
	chain.elementCount++
}

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
