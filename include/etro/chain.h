#ifndef __ETRO_CHAIN_H__
#define __ETRO_CHAIN_H__

/*-------------------------------------------
* C++ Code only (Template code)
---------------------------------------------*/

#include <iterator>
#include <cstdint>

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

	Node_t* head;
	Node_t* tail;

public:
	enum class ChainIteratorStartingPoint : unsigned char { Head, Tail };

	class ChainIterator : public std::iterator<
		std::bidirectional_iterator_tag, //iter_type
		std::ptrdiff_t,                  //difference_type
		T,                               //value_type
		T*,								 //pointer
		T&>                              //reference
	{

	private:
		friend class Chain;
		enum class CurrentNodeDirection : unsigned char { Forward, Backward };
		ChainIterator(const Chain<T, MaxElems>& chain); //< private constructor for Chain to use

		Node_t* node;
		std::uintptr_t lastVisitedNode;
		std::size_t unwrappedIndex;
		CurrentNodeDirection dir;

	public:
		ChainIterator(const Chain<T, MaxElems>& chain, ChainIteratorStartingPoint startingPoint);
		ChainIterator& operator++();
		ChainIterator operator++(int);
		ChainIterator& operator--();
		ChainIterator operator--(int);
		bool operator==(ChainIterator other) const;
		bool operator!=(ChainIterator other) const;
		T& operator*();
	};

	ChainIterator begin();
	ChainIterator end();
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

	//Head and tail must be deleted separatly (aka, outside of the loop)
	//Might not be the best/most elegant solution but meh, needs must and all.

	Node_t* node = this->head;
	std::uintptr_t prevAddr = reinterpret_cast<std::uintptr_t>(node);
	std::uintptr_t nextAddr = prevAddr ^ node->link;
	std:uintptr_t tmp;

	delete node; //< Delete Head
	node = reinterpret_cast<Node_t*>(nextAddr); //< First node that isn't head

	while (node != this->tail) {
		tmp = prevAddr;
		prevAddr = nextAddr;
		nextAddr = tmp ^ node->link;

		delete node;
		node = reinterpret_cast<Node_t*>(nextAddr);
	}
	delete node; //< Delete Tail
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


template<typename T, size_t MaxElems>
typename Chain<T, MaxElems>::ChainIterator Chain<T, MaxElems>::begin()
{
	return ChainIterator(*this, ChainIteratorStartingPoint::Head);
}

template<typename T, size_t MaxElems>
typename Chain<T, MaxElems>::ChainIterator Chain<T, MaxElems>::end()
{
	return ChainIterator(*this);
}

template<typename T, size_t MaxElems>
Chain<T, MaxElems>::ChainIterator::ChainIterator(const Chain<T, MaxElems>& chain, ChainIteratorStartingPoint startingPoint)
{
	switch (startingPoint)
	{
	case ChainIteratorStartingPoint::Head:
		this->node = chain.head;
		this->lastVisitedNode = reinterpret_cast<std::uintptr_t>(chain.head);
		this->unwrappedIndex = 0;
		this->dir = CurrentNodeDirection::Forward;
		break;
	case ChainIteratorStartingPoint::Tail:
		this->node = chain.tail;
		this->lastVisitedNode = reinterpret_cast<std::uintptr_t>(chain.tail);
		this->unwrappedIndex = chain.tail->elementCount - 1;
		this->dir = CurrentNodeDirection::Backward;
		break;
	}
}

template<typename T, size_t MaxElems>
Chain<T, MaxElems>::ChainIterator::ChainIterator(const Chain<T, MaxElems>& chain)
	: node(chain.tail)
	, lastVisitedNode(reinterpret_cast<std::uintptr_t>(chain.tail))
	, unwrappedIndex(chain.tail->elementCount)
	, dir(CurrentNodeDirection::Backward)
{}

template<typename T, size_t MaxElems>
typename Chain<T, MaxElems>::ChainIterator& Chain<T, MaxElems>::ChainIterator::operator++()
{
	//TODO: Do the increment here

	//First see if we're still within unwrapped node
	std::size_t nextIndex = this->unwrappedIndex + 1;
	if (nextIndex < this->node->elementCount)
	{
		this->unwrappedIndex = nextIndex;
	}
	else
	{
		//Theres a bug here due to back then forward where lastVisitedNode will be Tail!!!
		// TODO: ^^ Fix
		//Gotta check if were at the end
		if (this->lastVisitedNode == this->node->link)
		{
			//since we're at the end we'll use the one past the end trick :)
			this->unwrappedIndex += 1; //same as this->node->elementCount
			return *this;
		}
		
		//Note, youve worried about direction but actually youve got to remember that it's an
		//unwrapped list and so it only matters if yove reached the end of the wrap.

		std::uintptr_t nextAddr;
		switch (this->dir)
		{
		case CurrentNodeDirection::Forward: //< Good ++ and forward
			nextAddr = this->node->link ^ this->lastVisitedNode;
			break;
		case CurrentNodeDirection::Backward: //< Eugh Change of dir
			nextAddr = this->lastVisitedNode;
			this->dir = CurrentNodeDirection::Forward; //< Change Node Direction
			break;
		}
		this->lastVisitedNode = reinterpret_cast<std::uintptr_t>(this->node);
		this->node = reinterpret_cast<Node_t*>(nextAddr);
		this->unwrappedIndex = 0;
	}


	return *this;
}

template<typename T, size_t MaxElems>
typename Chain<T, MaxElems>::ChainIterator Chain<T, MaxElems>::ChainIterator::operator++(int unused)
{
	auto tmp = *this;
	operator++();
	return tmp;
}

template<typename T, size_t MaxElems>
typename Chain<T, MaxElems>::ChainIterator& Chain<T, MaxElems>::ChainIterator::operator--()
{
	//TODO: Do the decrement here

	//First see if we're still within unwrapped node
	if (this->unwrappedIndex > 0)
	{
		this->unwrappedIndex -= 1;
	}
	else
	{
		std::uintptr_t nextAddr;

		//Note, youve worried about direction but actually youve got to remember that it's an
		//unwrapped list and so it only matters if yove reached the end of the wrap.
		switch (this->dir)
		{
		case CurrentNodeDirection::Backward: //< Good -- and backwards
			nextAddr = this->node->link ^ this->lastVisitedNode;
			break;
		case CurrentNodeDirection::Forward: //< Eugh Change of dir
			nextAddr = this->lastVisitedNode;
			this->dir = CurrentNodeDirection::Backward; //<	Change NodeDir
			break;
		}
		this->lastVisitedNode = reinterpret_cast<std::uintptr_t>(this->node);
		this->node = reinterpret_cast<Node_t*>(nextAddr);
		this->unwrappedIndex = this->node->elementCount - 1;
	}

	return *this;
}

template<typename T, size_t MaxElems>
typename Chain<T, MaxElems>::ChainIterator Chain<T, MaxElems>::ChainIterator::operator--(int)
{
	auto tmp = *this;
	operator--();
	return tmp;
}

template<typename T, size_t MaxElems>
bool Chain<T, MaxElems>::ChainIterator::operator==(ChainIterator other) const
{
	//Note, direction doesn't matter for equality, nor does last Visited Node

	return (this->node == other.node) && (this->unwrappedIndex == other.unwrappedIndex);
}

template<typename T, size_t MaxElems>
bool Chain<T, MaxElems>::ChainIterator::operator!=(ChainIterator other) const
{
	return !(operator==(other));
}

template<typename T, size_t MaxElems>
T& Chain<T, MaxElems>::ChainIterator::operator*()
{
	return this->node->elements[this->unwrappedIndex];
}









#endif