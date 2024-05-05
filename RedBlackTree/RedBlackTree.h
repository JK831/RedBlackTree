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
	T data;
	RedBlackNode<T>* parent;
	RedBlackNode<T>* child[2];
};

template <typename T>
class RedBlackTree
{
public:
	RedBlackTree();
	void Insert(T data);
	void Delete(T data);

private:
	EChildDir ChildDir(RedBlackNode<T>* node);
	RedBlackNode<T>* Rotate(RedBlackNode<T>* InRoot, EChildDir InDir);

public:
	RedBlackNode<T>* _root;
	RedBlackNode<T>* _nil;
};

template<typename T>
inline RedBlackTree<T>::RedBlackTree()
{
	_nil = new RedBlackNode<T>();
	_nil->color = NodeColor::Black;
	_nil->left = _nil->right = nullptr;
	_root = _nil;
}

template<typename T>
inline void RedBlackTree<T>::Insert(T data)
{
	if (_root == _nil)
	{
		_root = new RedBlackNode<T>(data);
		_root->color = NodeColor::Black;
		_root->parent = nullptr;
		_root->left = _root->right = _nil;
		return;
	}
	else
	{
		RedBlackNode<T>* parent = _root;
		while ((parent->left == _nil && parent->right == _nil) == false)
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
inline void RedBlackTree<T>::Delete(T data)
{
	RedBlackNode<T>* deleteNode = _root;
	while (deleteNode != _nil)
	{
		if (deleteNode->data == data)
		{
			break;
		}
		else if (deleteNode->data > data)
		{
			deleteNode = deleteNode->child[EChildDir::LEFT];
		}
		else if (deleteNode->data < data)
		{
			deleteNode = deleteNode->child[EChildDir::RIGHT];
		}
	}

	/** �����ؾ� �� ��� ã�� ����. */
	if (deleteNode == _nil)
		return;

	/**
	* TODO: deleteNode�� child�� 2����� deleteNode�� sibling�� leftmost child or rightmost child ����� �� ��������
	* �� ��带 ������ �� �� ��� �������� �뷱�� ����
	*/
	if (deleteNode->child[EChildDir::LEFT] != _nil && deleteNode->child[EChildDir::RIGHT] != _nil)
	{

	}

	/** When the deleted node has only one child, delete the node and replace it with its child and color it black. */
	if (deleteNode->child[EChildDir::LEFT] == _nil && deleteNode->child[EChildDir::RIGHT] != _nil)
	{
		RedBlackNode<T>* child = deleteNode->child[EChildDir::RIGHT];
		RedBlackNode<T>* grandParent = deleteNode->parent;
		child->parent = grandParent;
		child->color = NodeColor::Black;
		/** deleteNode�� root�� ��� */
		if (grandParent == nullptr)
		{
			delete deleteNode;
			return;
		}

		if (grandParent->child[EChildDir::LEFT] == deleteNode)
			grandParent->child[EChildDir::LEFT] = child;
		else
			grandParent->child[EChildDir::RIGHT] = child;
		delete deleteNode;
		return;
	}
	else if (deleteNode->child[EChildDir::RIGHT] == _nil && deleteNode->child[EChildDir::LEFT] != _nil)
	{
		RedBlackNode<T>* child = deleteNode->child[EChildDir::LEFT];
		RedBlackNode<T>* grandParent = deleteNode->parent;
		child->parent = grandParent;
		/** deleteNode�� root�� ��� */
		if (grandParent == nullptr)
		{
			delete deleteNode;
			return;
		}

		grandParent->child[EChildDir::LEFT] == deleteNode ? grandParent->child[EChildDir::LEFT] = child : grandParent->child[EChildDir::RIGHT] = child;
		delete deleteNode;
		return;
	}

	/** When the deleted node is root and has no children => Just delete root and replace it with nil. */
	if (deleteNode == _root && deleteNode->child[EChildDir::LEFT] == _nil && deleteNode->child[EChildDir::RIGHT])
	{
		delete _root;
		_root = _nil;
		return;
	}

	/** When the deleted node is red and has no children => remove it */
	if (deleteNode->color == NodeColor::Red && deleteNode->child[EChildDir::LEFT] == _nil && deleteNode->child[EChildDir::RIGHT])
	{
		RedBlackNode<T>* grandParent = deleteNode->parent;
		grandParent->child[EChildDir::LEFT] == deleteNode ? grandParent->child[EChildDir::LEFT] = _nil : grandParent->child[EChildDir::RIGHT];
		delete deleteNode;
		return;
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
