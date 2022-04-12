package dahaka

//Wish It could be Generic so I could assert that both
//orderables have the same base type
//Fortunatly I can do just that in the datastructure that uses this!
type Orderable interface {
	Equal(other Orderable) bool
	LessThan(other Orderable) bool
	GreaterThan(other Orderable) bool
}

//Dont technically need inner ollNode but .....
//it does make things easier especially if I want to use tail
//and/or use prev as well as next.

//Ordered Linked List
type OrderedLinkedList[T Orderable] struct {
	head *ollNode[T]
	//tail        *ollNode[T]
}

type ollNode[T Orderable] struct {
	next *ollNode[T]
	//prev     *ollNode[T]
	elem T
}

// func NewOrderedLinkedList[T Orderable]() *OrderedLinkedList[T] {
// 	return &OrderedLinkedList[T]{head: n}
// }

//TODO: Test
func (list *OrderedLinkedList[T]) Add(elem T) {
	newNode := &ollNode[T]{elem: elem}

	if list.head == nil {
		list.head = newNode
		return
	}

	currNode := list.head

	if elem.LessThan(currNode.elem) {
		newNode.next = currNode
		list.head = newNode
		return
	}

	nextNode := currNode.next

	//As long as short circuiting is on were all good
	for (nextNode != nil) && (elem.GreaterThan(nextNode.elem)) {
		currNode = nextNode
		nextNode = nextNode.next
	}

	currNode.next = newNode
	newNode.next = nextNode //Which is good even if nextNode is nil :)
}

func (list *OrderedLinkedList[T]) Get(index int) T {
	currNode := list.head
	for i := 0; i < index; i++ {
		currNode = currNode.next
	}
	return currNode.elem
}

func (list *OrderedLinkedList[T]) Walk(ch chan T) {
	var walk = func(list *OrderedLinkedList[T], ch chan T) {
		currNode := list.head
		for currNode != nil {
			ch <- currNode.elem //blocking
			currNode = currNode.next
		}
	}
	walk(list, ch)
	close(ch)
}

func (list *OrderedLinkedList[T]) ForEach(f func(T)) {
	//Will this not cause it to close before its finished reciving?
	ch := make(chan T, 32)
	go list.Walk(ch)

	for x := range ch {
		f(x)
	}
}
