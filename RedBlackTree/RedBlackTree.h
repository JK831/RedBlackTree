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

		/** TODO: Red-black tree 조건 체크 */
		RedBlackNode<T>* grandParent = nullptr;
		do
		{
			if (parent->color == NodeColor::Black) return;
			
			grandParent = parent->parent;
			if (grandParent == nullptr)
			{
				/** root node의 색이 red인 경우 black으로 변경한 후 return */
				parent->color = NodeColor::Black;
				return;
			}
			EChildDir parentDir = grandParent->child[EChildDir::LEFT] == parent ? EChildDir::LEFT : EChildDir::RIGHT;
			RedBlackNode<T>* uncle = grandParent->child[1 - parentDir];
			/** grandparent가 black이고 uncle도 black인 경우 */
			if (uncle->color == NodeColor::Black)
			{
				/** 새로운 노드가 parent의 방향과 다르다면 parent를 기준으로 parent 방향으로 회전하여
				* 새로운 노드의 parent가 grandparent의 outer child가 되도록 만든다.
				* - 그렇게 하지 않고 parent를 grandparent 기준으로 parent의 반대 방향으로 회전할 경우
				* - parent를 black으로, grandparent를 red로 바꾼 후
				* - grandparent의 parent 방향 자식으로 새로운 노드를 넣어야 하는데 grandparent가 red가 되어야 하므로 불가능
				*/
				if (newNode == parent->child[1 - parentDir])
				{
					Rotate(parent, parentDir); // 회전 수행 -> parent가 newNode의 자식 노드가 되고 outer child가 됨.
					newNode = parent; // newNode의 parent가 outer child가 되었으므로 새로 삽입된 newNode로 간주하여 탐색 시작
					parent = grandParent->child[parentDir]; // 이전의 newNode가 새로운 parent가 되었으므로 parent 갱신
				}

				/**
				* newNode가 grandparent 기준으로 outer child 이므로 grandparent 기준으로 parent의 반대 방향으로 회전
				* parent가 grandparent의 parent가 된다. 그 후 parent를 black으로, grandparent를 red로 변경해준다면
				* parent를 root로 하는 sub tree는 root부터 각 leaf까지의 모든 경로에서 black의 수가 같아진다.
				*/
				Rotate(grandParent, 1 - parentDir);
				parent->color = NodeColor::Black;
				grandParent->color = NodeColor::Red;
				return;
			}
			else
			{
				/**
				 * 만약 parent, uncle이 모두 red라면 parent, uncle을 모두 black으로 바꾸고 grandparent를 red로 바꾼다.
				 * 그 후 grandparent를 기준으로 grandparent의 상위 노드가 red인지 다시 체크
				 * red black tree의 조건을 만족하도록 만든다.
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
