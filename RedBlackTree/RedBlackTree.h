#pragma once

#define CRASH do { int* ptr = nullptr; *ptr = 1; } while(false)

enum class NodeColor
{
	Black,
	Red,
};

enum class EChildDir
{
	LEFT,
	RIGHT,
};

template <typename T>
struct RedBlackNode
{
public:
	RedBlackNode<T>(T* InData);
	NodeColor color;
	T* data;
	RedBlackNode<T>* parent;
	RedBlackNode<T>* child[2];
};

template <typename T>
class RedBlackTree
{
public:
	RedBlackTree();
	void Insert(T* data);

private:
	EChildDir ChildDir(RedBlackNode<T>* node);
	RedBlackNode<T>* Rotate(RedBlackNode<T>* InRoot, EChildDir InDir);

public:
	RedBlackNode<T>* _root;
	RedBlackNode<T>* _nil;
};

template<typename T>
inline RedBlackTree<T>::RedBlackTree() : _root(nullptr)
{
	_nil = new RedBlackNode<T>();
	_nil->color = NodeColor::Black;
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
		if (_root == nullptr)
		{
			_root = new RedBlackNode<T>(data);
			_root->left = _root->right = _nil;
			_root->color = NodeColor::Black;
			return;
		}
		RedBlackNode<T>* parent = _root;
		while (parent->left != _nil && parent->right != _nil)
		{
			if (parent->data == data)
				return;

			if (parent->data > data)
				parent = parent->left;
			else if (parent->data < data)
				parent = parent->right;
		}

		if (parent->data == data)
			return;

		RedBlackNode<T>* newNode = new RedBlackNode<T>(data);
		if (parent->data > data)
			parent->left = newNode;
		else if (parent->data < data)
			parent->right = newNode;

		newNode->parent = parent;
		newNode->left = newNode->right = _nil;

		/** TODO: Red-black tree ���� üũ */
		RedBlackNode<T>* grandParent = nullptr;
		do
		{
			if (parent->color == NodeColor::Black) return;
			
			grandParent = parent->parent;
			if (grandParent == nullptr)
			{
				/** root node�� ���� red�� ��� black���� ������ �� return */
				parent->color = NodeColor::Black;
				return;
			}
			EChildDir parentDir = grandParent->child[EChildDir::LEFT] == parent ? EChildDir::LEFT : EChildDir::RIGHT;
			RedBlackNode<T>* uncle = grandParent->child[1 - parentDir];
			/** grandparent�� black�̰� uncle�� black�� ��� */
			if (uncle->color == NodeColor::Black)
			{
				/** ���ο� ��尡 parent�� ����� �ٸ��ٸ� parent�� �������� parent �������� ȸ���Ͽ�
				* ���ο� ����� parent�� grandparent�� outer child�� �ǵ��� �����.
				* - �׷��� ���� �ʰ� parent�� grandparent �������� parent�� �ݴ� �������� ȸ���� ���
				* - parent�� black����, grandparent�� red�� �ٲ� ��
				* - grandparent�� parent ���� �ڽ����� ���ο� ��带 �־�� �ϴµ� grandparent�� red�� �Ǿ�� �ϹǷ� �Ұ���
				*/
				if (newNode == parent->child[1 - parentDir])
				{
					Rotate(parent, parentDir); // ȸ�� ���� -> parent�� newNode�� �ڽ� ��尡 �ǰ� outer child�� ��.
					newNode = parent; // newNode�� parent�� outer child�� �Ǿ����Ƿ� ���� ���Ե� newNode�� �����Ͽ� Ž�� ����
					parent = grandParent->child[parentDir]; // ������ newNode�� ���ο� parent�� �Ǿ����Ƿ� parent ����
				}

				/**
				* newNode�� grandparent �������� outer child �̹Ƿ� grandparent �������� parent�� �ݴ� �������� ȸ��
				* parent�� grandparent�� parent�� �ȴ�. �� �� parent�� black����, grandparent�� red�� �������شٸ�
				* parent�� root�� �ϴ� sub tree�� root���� �� leaf������ ��� ��ο��� black�� ���� ��������.
				*/
				Rotate(grandParent, 1 - parentDir);
				parent->color = NodeColor::Black;
				grandParent->color = NodeColor::Red;
				return;
			}
			else
			{
				/**
				 * ���� parent, uncle�� ��� red��� parent, uncle�� ��� black���� �ٲٰ� grandparent�� red�� �ٲ۴�.
				 * �� �� grandparent�� �������� grandparent�� ���� ��尡 red���� �ٽ� üũ
				 * red black tree�� ������ �����ϵ��� �����.
				 */
				parent->color = NodeColor::Black;
				uncle->color = NodeColor::Black;
				grandParent->color = NodeColor::Red;
				newNode = grandParent;
			}
		}
		while ((parent = newNode->parent) != nullptr)
	}
}

template<typename T>
inline RedBlackNode<T>* RedBlackTree<T>::Rotate(RedBlackNode<T>* InRoot, EChildDir InDir)
{
	RedBlackNode<T>* parent = InRoot->parent;
	RedBlackNode<T>* oppositeChild = InRoot->child[1 - (int)InDir];
	if (oppositeChild == _nil) CRASH;

	InRoot->child[1 - InDir] = oppositeChild->child[(int)InDir];
	if (oppositeChild[(int)InDir] != _nil) oppositeChild[(int)InDir]->parent = InRoot;
	oppositeChild->child[(int)InDir] = InRoot;
	InRoot->parent = oppositeChild;
	oppositeChild->parent = parent;
	if (parent != nullptr)
	{
		parent->child[EChildDir::LEFT] == InRoot ? parent->child[EChildDir::RIGHT] = oppositeChild : parent->child[EChildDir::LEFT];
	}
	else
		_root = oppositeChild;

	return oppositeChild;
}

template<typename T>
inline RedBlackNode<T>::RedBlackNode(T* InData) : color(NodeColor::Red), data(InData)
{
}
