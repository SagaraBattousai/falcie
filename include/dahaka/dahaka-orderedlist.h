

//export module dahaka:orderedlist;
#ifndef __DAHAKA_ORDEREDLIST__
#define __DAHAKA_ORDEREDLIST__

//Will remove but for now need dup
template <typename T> using comparitor = int(*)(T& self, T& other);

//export 
namespace dahaka
{
	template <typename T>
	class OrderedLinkedList
	{
	public:
		OrderedLinkedList(comparitor<T> comp);
		virtual ~OrderedLinkedList();
		void Add(T elem);

	private:
		struct Node {
			Node* next;
			T item;

			explicit Node(T item);
		};

		Node *head;
		comparitor<T> comp;
	};

	template <typename T>
	OrderedLinkedList<T>::OrderedLinkedList(comparitor<T> comp)
		: head(nullptr)
		, comp(comp)
	{}

	//TODO: Use smart pointers instead.
	template <typename T>
	OrderedLinkedList<T>::~OrderedLinkedList()
	{
		Node *node = this->head;
		Node *tmp;
		while (node != nullptr)
		{
			tmp = node->next;
			//Can I reuse node stack variable after deleting what it points to.
			delete node;
			node = tmp;
		}
	}

	template <typename T>
	void OrderedLinkedList<T>::Add(T elem)
	{
		Node *newNode = new Node(elem);
		Node *currNode = this->head;

		if (currNode == nullptr)
		{
			this->head = newNode;
			return;
		}

		if (this->comp(elem, currNode->item) < 0)
		{
			this->head = newNode;
			newNode->next = currNode;
			return;
		}

		while (currNode->next != nullptr)
		{
			if (this->comp(elem, currNode->next->elem) < 0)
			{
				newNode->next = currNode->next;
				currNode->next = currNode;
				return;
			}

		}

		currNode->next = newNode;
	}

	template <typename T>
	OrderedLinkedList<T>::Node::Node(T item)
		: next(nullptr)
		, item(item)
	{}


} //namespace pulse

#endif

