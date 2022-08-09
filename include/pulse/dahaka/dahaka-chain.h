#ifndef __DAHAKA_CHAIN__
#define __DAHAKA_CHAIN__

//TODO: Redesign! Remove the unrolled template (or atleast only as a child)

//module;
#include <iterator>
#include <stdexcept>
#include <cstdint>

//export module dahaka:chain;

#include <array> //import <array>;

//export
namespace pulse
{

	template<typename T, std::int64_t UnrolledElems>
	class Chain
	{
	public:
		Chain();

		//~Chain();

		void Add(T&& elem);

		const T& GetLast();

	private:
		struct Node
		{
			std::uintptr_t link;
			//No point using std::array as I have to keep
			//track of the current number of elements anyway
			std::array<T, UnrolledElems> elements;
			std::int64_t elementCount;

			Node();
			explicit Node(std::uintptr_t link);
			Node(std::uintptr_t link, std::int64_t elementCount);
		};

		Node* addNewNode();

		Node* head;
		Node* tail;

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
			ChainIterator(const Chain<T, UnrolledElems>& chain); //< private constructor for Chain to use

			Node* node;
			std::uintptr_t lastVisitedNode;
			std::int64_t unwrappedIndex;
			CurrentNodeDirection dir;

		public:
			ChainIterator(const Chain<T, UnrolledElems>& chain,
				ChainIteratorStartingPoint startingPoint);

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

	template<typename T, std::int64_t UnrolledElems>
	Chain<T, UnrolledElems>::Chain()
	{
		Node* node = new Node();

		this->head = node;
		this->tail = node;
	}

	//Its worng I just cant remember why
	/*
	template<typename T, std::int64_t UnrolledElems>
	Chain<T, UnrolledElems>::~Chain()
	{
		//Semi Safety check
		if (this->head == nullptr)
		{
			return;
		}

		//Head and tail must be deleted separatly (aka, outside of the loop)
		//Might not be the best/most elegant solution but meh, needs must and all.

		Node* node = this->head;
		std::uintptr_t prevAddr = reinterpret_cast<std::uintptr_t>(node);
		std::uintptr_t nextAddr = prevAddr ^ node->link;

		std:uintptr_t tmp;

		delete node; //< Delete Head
		node = reinterpret_cast<Node*>(nextAddr); //< First node that isn't head

		while (node != this->tail)
		{
			tmp = prevAddr;
			prevAddr = nextAddr;
			nextAddr = tmp ^ node->link;

			delete node;
			node = reinterpret_cast<Node*>(nextAddr);
		}
		delete node; //< Delete Tail
		//Shouldn't need to set head and tail to nullptr since object is dead/deleted
	}
	*/
	template<typename T, std::int64_t UnrolledElems>
	void Chain<T, UnrolledElems>::Add(T&& elem)
	{
		Node* node = this->tail;

		if (node->elementCount == UnrolledElems)
		{
			node = this->addNewNode();
		}

		node->elements[node->elementCount] = std::forward<T>(elem);
		node->elementCount++;

	}

	template<typename T, std::int64_t UnrolledElems>
	const T& Chain<T, UnrolledElems>::GetLast()
	{
		Node* last = this->tail;
		return last->elements[last->elementCount - 1];
	}

	template<typename T, std::int64_t UnrolledElems>
	typename Chain<T, UnrolledElems>::Node* Chain<T, UnrolledElems>::addNewNode()
	{
		Node* prev = this->tail;
		std::uintptr_t prev_addr = reinterpret_cast<std::uintptr_t>(prev);

		Node* node = new Node(prev_addr);

		prev->link ^= reinterpret_cast<std::uintptr_t>(node);

		this->tail = node;

		return node;
	}

	template<typename T, std::int64_t UnrolledElems>
	Chain<T, UnrolledElems>::Node::Node()
		: link(reinterpret_cast<std::uintptr_t>(this))
		, elementCount(0)
	{
	}

	template<typename T, std::int64_t UnrolledElems>
	Chain<T, UnrolledElems>::Node::Node(std::uintptr_t link)
		: link(link)
		, elementCount(0)
	{
	}

	template<typename T, std::int64_t UnrolledElems>
	Chain<T, UnrolledElems>::Node::Node(std::uintptr_t link, std::int64_t elementCount)
		: link(link)
		, elementCount(elementCount)
	{
	}

	template<typename T, std::int64_t UnrolledElems>
	typename Chain<T, UnrolledElems>::ChainIterator Chain<T, UnrolledElems>::begin()
	{
		return ChainIterator(*this, ChainIteratorStartingPoint::Head);
	}

	template<typename T, std::int64_t UnrolledElems>
	typename Chain<T, UnrolledElems>::ChainIterator Chain<T, UnrolledElems>::end()
	{
		return ChainIterator(*this);
	}

	template<typename T, std::int64_t UnrolledElems>
	Chain<T, UnrolledElems>::ChainIterator::ChainIterator(const Chain<T, UnrolledElems>& chain, ChainIteratorStartingPoint startingPoint)
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
		default:
			throw std::invalid_argument("Parameter \"startingPoint\" must be either "
				"ChainIteratorStartingPoint::Head or ChainIteratorStartingPoint::Tail.");
		}
	}

	template<typename T, std::int64_t UnrolledElems>
	Chain<T, UnrolledElems>::ChainIterator::ChainIterator(const Chain<T, UnrolledElems>& chain)
		: node(chain.tail)
		, lastVisitedNode(reinterpret_cast<std::uintptr_t>(chain.tail))
		, unwrappedIndex(chain.tail->elementCount)
		, dir(CurrentNodeDirection::Backward)
	{
	}

	template<typename T, std::int64_t UnrolledElems>
	typename Chain<T, UnrolledElems>::ChainIterator& Chain<T, UnrolledElems>::ChainIterator::operator++()
	{
		//TODO: Do the increment here

		//First see if we're still within unwrapped node
		std::int64_t nextIndex = this->unwrappedIndex + 1;
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
			this->node = reinterpret_cast<Node*>(nextAddr);
			this->unwrappedIndex = 0;
		}


		return *this;
	}

	template<typename T, std::int64_t UnrolledElems>
	typename Chain<T, UnrolledElems>::ChainIterator Chain<T, UnrolledElems>::ChainIterator::operator++(int unused)
	{
		auto tmp = *this;
		operator++();
		return tmp;
	}

	template<typename T, std::int64_t UnrolledElems>
	typename Chain<T, UnrolledElems>::ChainIterator& Chain<T, UnrolledElems>::ChainIterator::operator--()
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
			this->node = reinterpret_cast<Node*>(nextAddr);
			this->unwrappedIndex = this->node->elementCount - 1;
		}

		return *this;
	}

	template<typename T, std::int64_t UnrolledElems>
	typename Chain<T, UnrolledElems>::ChainIterator Chain<T, UnrolledElems>::ChainIterator::operator--(int)
	{
		auto tmp = *this;
		operator--();
		return tmp;
	}

	template<typename T, std::int64_t UnrolledElems>
	bool Chain<T, UnrolledElems>::ChainIterator::operator==(ChainIterator other) const
	{
		//Note, direction doesn't matter for equality, nor does last Visited Node

		return (this->node == other.node) && (this->unwrappedIndex == other.unwrappedIndex);
	}

	template<typename T, std::int64_t UnrolledElems>
	bool Chain<T, UnrolledElems>::ChainIterator::operator!=(ChainIterator other) const
	{
		return !(operator==(other));
	}

	template<typename T, std::int64_t UnrolledElems>
	T& Chain<T, UnrolledElems>::ChainIterator::operator*()
	{
		return this->node->elements[this->unwrappedIndex];
	}

} //namespace pulse

#endif
