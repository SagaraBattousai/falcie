
//module;
//#include <iterator>
//#include <stdexcept>
//#include <cstdint>

//export module dahaka:tree;

#ifndef __DAHAKA_TREE__
#define __DAHAKA_TREE__

//Will remove but for now need dup
template <typename T> using comparitor = int(*)(T& self, T& other);

/***************************************************************************
*Plain, standard, no frills tree without any self - balancing utility.
* Since the worst case binary tree has the same time complexity and
* (to some degree) utility as an ordered linked list and so is preferable
* in use cases such as transaction pools.
*************************************************************************** /

/*****************************************************************************
* TODO: Update/Add another class to implement Weighted-Binary Tree.
******************************************************************************/


namespace pulse
{

	template <typename T>
	class Tree
	{
	public:
		Tree(comparitor<T> cmp);
		Tree(comparitor<T> cmp, T root);
		virtual ~Tree();

		void Add(T item);

	private:
		struct Node
		{
			Node *left;
			Node *right;
			T item;

			Node(T item);
			Node(Node *left, Node *right, T item);
			virtual ~Node();
			//void InsertNode(Node *node, comparitor<T> cmp);
		};

		Node *root;
		comparitor<T> cmp;
	};

	template <typename T>
	Tree<T>::Tree(comparitor<T> cmp)
		: cmp(cmp)
		, root(nullptr)
	{}

	template <typename T>
	Tree<T>::Tree(comparitor<T> cmp, T root)
		: cmp(cmp)
		, root(new Node(root))
	{}

	template <typename T>
	Tree<T>::~Tree()
	{
		delete this->root;
	}

	template <typename T>
	void Tree<T>::Add(T elem)
	{
		Node *node = new Node(elem);

		if (this->root == nullptr)
		{
			this->root = node;
			return;
		}

		//this->root->InsertNode(node);

		//Pre node must point to a node i.e. node != nullptr.
		auto insertNode = [&node, &cmp = this->cmp](auto self, Node *currNode) -> void
		{
			int comp = cmp(node->item, currNode->item);
			Node *next = comp < 0 ? currNode->left : currNode->right;

			if (next == nullptr)
			{
				if (comp < 0)
				{
					currNode->left = node;
				}
				else
				{
					currNode->right = node;
				}
				return;
			}

			//next->InsertNode(node);
			self(self, next);
		};
		insertNode(insertNode, this->root);
	}


	template <typename T>
	Tree<T>::Node::Node(T item)
		: left(nullptr)
		, right(nullptr)
		, item(item)
	{}

	template <typename T>
	Tree<T>::Node::Node(Node *left, Node *right, T item)
		: left(left)
		, right(right)
		, item(item)
	{}

	template <typename T>
	Tree<T>::Node::~Node()
	{
		if (this->left != nullptr)
		{
			delete this->left;
		}

		if (this->right != nullptr)
		{
			delete this->right;
		}

	}
} //namespace pulse

#endif

