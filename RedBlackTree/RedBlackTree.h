#pragma once

enum class NodeColor
{
	Black,
	Red,
};

template <typename T>
struct RedBlackNode
{
public:
	RedBlackNode<T>(T* InData);
	NodeColor color;
	T* data;
	RedBlackNode<T>* parent;
	RedBlackNode<T>* left;
	RedBlackNode<T>* right;
};

template <typename T>
class RedBlackTree
{
public:
	RedBlackTree();
	void Insert(T* data);

public:
	RedBlackNode<T>* _root;
	RedBlackNode<T>* _nil;
};

template<typename T>
inline RedBlackTree<T>::RedBlackTree() : _root(nullptr)
{
	_nil = new RedBlackNode<T>();
	_nil->left = _nil->right = nullptr;
}

template<typename T>
inline void RedBlackTree<T>::Insert(T* data)
{
	if (_root == nullptr)
	{
		_root = new RedBlackNode<T>(data);
		_root->color = NodeColor::Black;
		_root->parent = nullptr;
		_root->left = _root->right = _nil;
	}
	else
	{
		RedBlackNode<T>* curNode = _root;
		while (curNode->left != _nil && curNode->right != _nil)
		{
			if (curNode->data == data)
				return;

			if (curNode->data > data)
				curNode = curNode->left;
			else if (curNode->data < data)
				curNode = curNode->right;
		}

		if (curNode->data == data)
			return;

		RedBlackNode<T>* newNode = new RedBlackNode<T>(data);
		if (curNode->data > data)
			curNode->left = newNode;
		else if (curNode->data < data)
			curNode->right = newNode;

		newNode->parent = curNode;
		newNode->left = newNode->right = _nil;

		/** TODO: Red-black tree 조건 체크 */
		while (newNode->parent->color != NodeColor::Black)
		{

		}
	}
}

template<typename T>
inline RedBlackNode<T>::RedBlackNode(T* InData) : color(NodeColor::Red), data(InData)
{
}
