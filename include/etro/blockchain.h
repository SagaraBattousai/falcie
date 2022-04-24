#ifndef __ETRO_CHAIN_H__
#define __ETRO_CHAIN_H__

#include <cstdint>
#include <array>

//How to check if std::uintptr_t is defined?

template<typename T, size_t MaxElems>
class Chain
{
public:
	Chain();
	virtual ~Chain();
	void Add(T elem);
	T GetLast();

private:
	typedef struct Node
	{
		std::uintptr_t link;
		//No point using std::array as I have to keep
		//track of the current number of elements anyway
		T elements[MaxElems];
		size_t elementCount;
		
	} Node_t;

	Node_t* addNewNode();

	Node_t *head;
	Node_t *tail;

};

template<typename T, size_t MaxElems>
Chain<T, MaxElems>::Chain()
{
	Node_t* node = new Node_t();
	node->link = reinterpret_cast<std::uintptr_t>(node);
	node->elementCount = 0;

	this->head = node;
	this->tail = node;
}

template<typename T, size_t MaxElems>
Chain<T, MaxElems>::~Chain()
{
	//Semi Safety check
	if (this->head == nullptr)
	{
		return;
	}

	Node_t* node = this->head;
	std::uintptr_t addr = reinterpret_cast<std::uintptr_t>(node);
	while (node != this->tail) {
		addr = addr ^ node->link;
		delete node;
		node = reinterpret_cast<Node_t*>(addr);
	}
	delete node;
	//Shouldn't need to set head and tail to nullptr since object is dead/deleted
}

template<typename T, size_t MaxElems>
void Chain<T, MaxElems>::Add(T elem)
{
	Node_t* node = this->tail;

	if (node->elementCount == MaxElems) 
	{
		node = this->addNewNode();
	}

	node->elements[node->elementCount] = elem;
	node->elementCount++;

}


template<typename T, size_t MaxElems>
T Chain<T, MaxElems>::GetLast()
{
	Node_t* last = this->tail;
	return last->elements[last->elementCount - 1];

}

//Why I need typename I do no understand, unfortunatly I do not have time.
//Will only work so long as nodes are onlyy added and only to the end
template<typename T, size_t MaxElems>
typename Chain<T, MaxElems>::Node_t* Chain<T, MaxElems>::addNewNode()
{
	Node_t* prev = this->tail;
	std::uintptr_t prev_addr = reinterpret_cast<std::uintptr_t>(prev);

	Node_t* node = new Node_t();
	//TODO: may want to xor wirh self to help with iter
	node->link = prev_addr;//reinterpret_cast<std::uintptr_t>(node);
	node->elementCount = 0;

	//std::uintptr_t node_addr = reinterpret_cast<std::uintptr_t>(node);

	prev->link ^= reinterpret_cast<std::uintptr_t>(node);  //node_addr;

	this->tail = node;

	return node;
}




#endif
