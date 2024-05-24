#pragma once

#define CRASH do { int* ptr = nullptr; *ptr = 1; } while(false)

enum class NodeColor
{
	Black,
	Red,
};

enum class EChildDir : int
{
	LEFT,
	RIGHT,
};

template <typename T>
struct RedBlackNode
{
public:
	RedBlackNode<T>(T InData);
	NodeColor color;
	T data;
	RedBlackNode<T>* parent;
	RedBlackNode<T>* child[2];
};

template<typename T>
inline RedBlackNode<T>::RedBlackNode(T InData) : color(NodeColor::Red), data(InData)
{
}

template <typename T>
class RedBlackTree
{
public:
	RedBlackTree<T>();
	RedBlackNode<T>* Search(T data);
	void Insert(T data);
	void Delete(T data);
	inline int GetSize() { return _size; }

private:
	EChildDir ChildDir(RedBlackNode<T>* node);
	RedBlackNode<T>* Rotate(RedBlackNode<T>* InRoot, EChildDir InDir);
	void TransPlant(RedBlackNode<T>* a, RedBlackNode<T>* b);

	void InsertFixUp(RedBlackNode<T>* InNode);
	void DeleteFixUp(RedBlackNode<T>* InNode);

public:
	RedBlackNode<T>* _root;
	RedBlackNode<T>* _nil;

private:
	int _size;
};

template<typename T>
inline RedBlackTree<T>::RedBlackTree()
{
	_nil = new RedBlackNode<T>((T)0);
	_nil->color = NodeColor::Black;
	_nil->child[(int)EChildDir::LEFT] = _nil->child[(int)EChildDir::RIGHT] = _nil;
	_root = _nil;
}

template<typename T>
inline RedBlackNode<T>* RedBlackTree<T>::Search(T data)
{
	RedBlackNode<T>* node = _root;
	while (node != _nil)
	{
		if (node->data == data)
		{
			break;
		}
		else if (node->data > data)
		{
			node = node->child[(int)EChildDir::LEFT];
		}
		else if (node->data < data)
		{
			node = node->child[(int)EChildDir::RIGHT];
		}
	}
	return node;
}

template<typename T>
inline void RedBlackTree<T>::Insert(T data)
{
	RedBlackNode<T>* newNode = _root;
	RedBlackNode<T>* newParent = _nil;
	while (newNode != _nil)
	{
		newParent = newNode;
		if (data < newNode->data)
			newNode = newNode->child[(int)EChildDir::LEFT];
		else
			newNode = newNode->child[(int)EChildDir::RIGHT];
	}
	newNode = new RedBlackNode<T>(data);
	newNode->parent = newParent;
	if (newParent == _nil)
	{
		_root = newNode;
	}
	else if (newNode->data < newParent->data)
		newParent->child[(int)EChildDir::LEFT] = newNode;
	else
		newParent->child[(int)EChildDir::RIGHT] = newNode;
	newNode->child[(int)EChildDir::LEFT] = _nil;
	newNode->child[(int)EChildDir::RIGHT] = _nil;
	newNode->color = NodeColor::Red;

	/** TODO: Implement InsertFixUp. */
	InsertFixUp(newNode);
	_size++;

	//if (_root == _nil)
	//{
	//	_root = new RedBlackNode<T>(data);
	//	_root->color = NodeColor::Black;
	//	_root->parent = nullptr;
	//	_root->left = _root->right = _nil;
	//	return;
	//}
	//else
	//{
	//	RedBlackNode<T>* parent = _root;
	//	while ((parent->left == _nil && parent->right == _nil) == false)
	//	{
	//		if (parent->data == data)
	//			return;

	//		if (parent->data > data)
	//			parent = parent->left;
	//		else if (parent->data < data)
	//			parent = parent->right;
	//	}

	//	if (parent->data == data)
	//		return;

	//	RedBlackNode<T>* newNode = new RedBlackNode<T>(data);
	//	if (parent->data > data)
	//		parent->left = newNode;
	//	else if (parent->data < data)
	//		parent->right = newNode;

	//	newNode->parent = parent;
	//	newNode->left = newNode->right = _nil;

	//	/** TODO: Red-black tree ���� üũ */
	//	RedBlackNode<T>* grandParent = nullptr;
	//	do
	//	{
	//		if (parent->color == NodeColor::Black) return;
	//		
	//		grandParent = parent->parent;
	//		if (grandParent == nullptr || grandParent == _nil)
	//		{
	//			/** root node�� ���� red�� ��� black���� ������ �� return */
	//			parent->color = NodeColor::Black;
	//			return;
	//		}
	//		EChildDir parentDir = grandParent->child[(int)EChildDir::LEFT] == parent ? EChildDir::LEFT : EChildDir::RIGHT;
	//		RedBlackNode<T>* uncle = grandParent->child[1 - parentDir];
	//		/** grandparent�� black�̰� uncle�� black�� ��� */
	//		if (uncle->color == NodeColor::Black)
	//		{
	//			/** ���ο� ��尡 parent�� ����� �ٸ��ٸ� parent�� �������� parent �������� ȸ���Ͽ�
	//			* ���ο� ����� parent�� grandparent�� outer child�� �ǵ��� �����.
	//			* - �׷��� ���� �ʰ� parent�� grandparent �������� parent�� �ݴ� �������� ȸ���� ���
	//			* - parent�� black����, grandparent�� red�� �ٲ� ��
	//			* - grandparent�� parent ���� �ڽ����� ���ο� ��带 �־�� �ϴµ� grandparent�� red�� �Ǿ�� �ϹǷ� �Ұ���
	//			*/
	//			if (newNode == parent->child[1 - parentDir])
	//			{
	//				Rotate(parent, parentDir); // ȸ�� ���� -> parent�� newNode�� �ڽ� ��尡 �ǰ� outer child�� ��.
	//				newNode = parent; // newNode�� parent�� outer child�� �Ǿ����Ƿ� ���� ���Ե� newNode�� �����Ͽ� Ž�� ����
	//				parent = grandParent->child[parentDir]; // ������ newNode�� ���ο� parent�� �Ǿ����Ƿ� parent ����
	//			}

	//			/**
	//			* newNode�� grandparent �������� outer child �̹Ƿ� grandparent �������� parent�� �ݴ� �������� ȸ��
	//			* parent�� grandparent�� parent�� �ȴ�. �� �� parent�� black����, grandparent�� red�� �������شٸ�
	//			* parent�� root�� �ϴ� sub tree�� root���� �� leaf������ ��� ��ο��� black�� ���� ��������.
	//			*/
	//			Rotate(grandParent, 1 - parentDir);
	//			parent->color = NodeColor::Black;
	//			grandParent->color = NodeColor::Red;
	//			return;
	//		}
	//		else
	//		{
	//			/**
	//			 * ���� parent, uncle�� ��� red��� parent, uncle�� ��� black���� �ٲٰ� grandparent�� red�� �ٲ۴�.
	//			 * �� �� grandparent�� �������� grandparent�� ���� ��尡 red���� �ٽ� üũ
	//			 * red black tree�� ������ �����ϵ��� �����.
	//			 */
	//			parent->color = NodeColor::Black;
	//			uncle->color = NodeColor::Black;
	//			grandParent->color = NodeColor::Red;
	//			newNode = grandParent;
	//		}
	//	}
	//	while ((parent = newNode->parent) != nullptr)
	//}
}

template<typename T>
inline void RedBlackTree<T>::Delete(T data)
{
	RedBlackNode<T>* deleteNode = Search(data);
	/** �����ؾ� �� ��� ã�� ����. */
	if (deleteNode == _nil)
		return;

	/**
	* TODO: deleteNode�� child�� 2����� deleteNode�� leftmost child or rightmost child ����� �� ��������
	* �� ��带 ������ �� �뷱�� ���� -> ���� �� �� ����� color, child ����
	* Leftmost or rightmost child node�� �� node�� �� child�� ��� nil�̰ų� right or left child �ϳ��� ������ �ִ�.
	*/
	NodeColor originalColor = deleteNode->color;
	RedBlackNode<T>* deleteCopy = deleteNode;
	RedBlackNode<T>* childNode = nullptr;
	// child�� �ϳ��� ���
	if (deleteNode->child[(int)EChildDir::LEFT] == _nil)
	{
		childNode = deleteNode->child[(int)EChildDir::RIGHT];
		TransPlant(deleteNode, childNode);
	}
	else if (deleteNode->child[(int)EChildDir::RIGHT] == _nil)
	{
		childNode = deleteNode->child[(int)EChildDir::LEFT];
		TransPlant(deleteNode, childNode);
	}
	else
	{
		/** Leftmost child�� ã�� deleteCopy�� �� node�� ����. */
		deleteCopy = deleteNode->child[(int)EChildDir::RIGHT];
		while (deleteCopy->child[(int)EChildDir::LEFT] != _nil)
		{
			deleteCopy = deleteCopy->child[(int)EChildDir::LEFT];
		}
		originalColor = deleteCopy->color; // ������ ����� ���� ����
		childNode = deleteCopy->child[(int)EChildDir::RIGHT];

		/**
		* ���� deleteNode���� �� level �̻� �������ٸ� deleteNode�� ��ġ�� deleteNode�� child node�� �ְ�
		* deleteNode�� �ڽ� ��带 deleteCopy�� �־��ش�. */
		if (deleteCopy != deleteNode->child[(int)EChildDir::RIGHT])
		{
			TransPlant(deleteCopy, deleteCopy->child[(int)EChildDir::RIGHT]);
			deleteCopy->child[(int)EChildDir::RIGHT] = deleteNode->child[(int)EChildDir::RIGHT];
			deleteCopy->child[(int)EChildDir::RIGHT]->parent = deleteCopy;
		}
		else childNode->parent = deleteCopy;

		TransPlant(deleteNode, deleteCopy); // ���� ������ ����� ���� deleteNode�� �����´�.
		deleteCopy->child[(int)EChildDir::LEFT] = deleteNode->child[(int)EChildDir::LEFT];
		deleteCopy->child[(int)EChildDir::LEFT]->parent = deleteCopy;
		deleteCopy->color = deleteNode->color;
	}
	delete deleteNode;

	if (originalColor == NodeColor::Black)
	{
		/** TODO: Implement DeleteFixup. */
		DeleteFixUp(childNode);
	}
	_size--;
}

template<typename T>
inline RedBlackNode<T>* RedBlackTree<T>::Rotate(RedBlackNode<T>* InRoot, EChildDir InDir)
{
	RedBlackNode<T>* parent = InRoot->parent;
	RedBlackNode<T>* oppositeChild = InRoot->child[1 - (int)InDir];
	if (oppositeChild == _nil) CRASH;

	InRoot->child[1 - (int)InDir] = oppositeChild->child[(int)InDir];
	if (oppositeChild->child[(int)InDir] != _nil) oppositeChild->child[(int)InDir]->parent = InRoot;
	oppositeChild->child[(int)InDir] = InRoot;
	InRoot->parent = oppositeChild;
	oppositeChild->parent = parent;
	if (parent != _nil)
	{
		parent->child[(int)EChildDir::LEFT] == InRoot ? parent->child[(int)EChildDir::LEFT] = oppositeChild : parent->child[(int)EChildDir::RIGHT] = oppositeChild;
	}
	else
		_root = oppositeChild;

	return oppositeChild;
}

template<typename T>
inline void RedBlackTree<T>::TransPlant(RedBlackNode<T>* a, RedBlackNode<T>* b)
{
	if (a->parent == _nil)
		_root = b;
	else if (a == a->parent->child[(int)EChildDir::LEFT])
		a->parent->child[(int)EChildDir::LEFT] = b;
	else
		a->parent->child[(int)EChildDir::RIGHT] = b;
	b->parent = a->parent;
}

template<typename T>
inline void RedBlackTree<T>::InsertFixUp(RedBlackNode<T>* InNode)
{
	while (InNode->parent->color == NodeColor::Red)
	{
		EChildDir parentDir = InNode->parent == InNode->parent->parent->child[(int)EChildDir::LEFT] ?
			EChildDir::LEFT : EChildDir::RIGHT;
		RedBlackNode<T>* uncle = InNode->parent->parent->child[1 - (int)parentDir];
		if (uncle->color == NodeColor::Red)
		{
			// Uncle�� red�� ��� parent, uncle ��� black���� �ٲ� �� grandparent�� red�� �ٲٰ� grandparent �������� �ٽ� �뷱�� ����
			InNode->parent->color = NodeColor::Black;
			uncle->color = NodeColor::Black;
			InNode->parent->parent->color = NodeColor::Red;
			InNode = InNode->parent->parent;
		}
		else
		{
			// parent�� ����� �ٸ� �� parent �������� ȸ���Ͽ� parent�� InNode�� ���� ���⿡ �ְ� ����� (InNode�� parent�� parent�� ��)
			if (InNode == InNode->parent->child[1 - (int)parentDir])
			{
				InNode = InNode->parent;
				Rotate(InNode, parentDir);
			}
			InNode->parent->color = NodeColor::Black;
			InNode->parent->parent->color = NodeColor::Red;
			Rotate(InNode->parent->parent, (EChildDir)(1 - (int)parentDir));
		}

	}
	_root->color = NodeColor::Black;
}

template<typename T>
inline void RedBlackTree<T>::DeleteFixUp(RedBlackNode<T>* InNode)
{
	while (InNode != _root && InNode->color == NodeColor::Black)
	{
		EChildDir InNodeDir = InNode->parent->child[(int)EChildDir::LEFT] == InNode ? EChildDir::LEFT : EChildDir::RIGHT;
		RedBlackNode<T>* sibling = InNode->parent->child[1 - (int)InNodeDir];
		if (sibling->color == NodeColor::Red)
		{
			sibling->color = NodeColor::Black;
			InNode->parent->color = NodeColor::Red;
			Rotate(InNode->parent, InNodeDir);
			sibling = InNode->parent->child[1 - (int)InNodeDir];
		}
		if (sibling->child[(int)EChildDir::LEFT]->color == NodeColor::Black
			&& sibling->child[(int)EChildDir::RIGHT]->color == NodeColor::Black)
		{
			sibling->color = NodeColor::Red;
			InNode = InNode->parent;
		}
		else
		{
			if (sibling->child[1 - (int)InNodeDir]->color == NodeColor::Black)
			{
				sibling->child[(int)InNodeDir]->color = NodeColor::Black;
				sibling->color = NodeColor::Red;
				Rotate(sibling, (EChildDir)(1 - (int)InNodeDir));
				sibling = InNode->parent->child[1 - (int)InNodeDir];
			}
			sibling->color = InNode->parent->color;
			InNode->parent->color = NodeColor::Black;
			Rotate(InNode->parent, InNodeDir);
			InNode = _root;
		}
	}
	InNode->color = NodeColor::Black;
}
