package cactuar

//Unrolled Linked List
type Chain[T any] struct {
	head        *node[T]
	tail        *node[T]
	maxElements int
}

type node[T any] struct {
	next     *node[T]
	prev     *node[T]
	elements []T
}

func NewChain[T any](maxElements int) *Chain[T] {
	elements := make([]T, 0, maxElements)
	n := &node[T]{next: nil, prev: nil, elements: elements}

	return &Chain[T]{head: n, tail: n, maxElements: maxElements}
}

func (chain *Chain[T]) addNewNode() *node[T] {
	//Get Last node
	lastNode := chain.tail

	elements := make([]T, 0, chain.maxElements)
	newNode := &node[T]{next: nil, prev: lastNode, elements: elements}

	//Set last node's next (which must currently be nil) to point to newNode
	lastNode.next = newNode

	//Set Chain's tail to be newNode
	chain.tail = newNode

	//assert: returned value == bc.tail
	return newNode
}

//TODO: Decide whether or not to also use mutex's?
func (chain *Chain[T]) Add(elem T) {
	node := chain.tail

	//If Slice is full
	if len(node.elements) == cap(node.elements) {
		node = chain.addNewNode()
	}

	node.elements = append(node.elements, elem)
}

func (chain *Chain[T]) GetLatest() T {
	latestElems := chain.tail.elements
	return latestElems[len(latestElems)-1]
}

//Also slow af if used many times
func (chain *Chain[T]) Get(index int) T {
	nodeCount := index / chain.maxElements
	elemIndex := index % chain.maxElements

	node := chain.head

	for i := 0; i < nodeCount; i++ {
		node = node.next
	}
	return node.elements[elemIndex]
}

func (chain *Chain[T]) Walk(ch chan T) {
	var walk = func(chain *Chain[T], ch chan T) {
		node := chain.head
		for node != nil {
			for _, elem := range node.elements {
				ch <- elem //block
			}
			node = node.next
		}
	}
	walk(chain, ch)
	close(ch)
}

func (chain *Chain[T]) ForEach(f func(T)) {
	ch := make(chan T, chain.maxElements)
	go chain.Walk(ch)

	for x := range ch {
		f(x)
	}
}
