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

	//	/** TODO: Red-black tree 조건 체크 */
	//	RedBlackNode<T>* grandParent = nullptr;
	//	do
	//	{
	//		if (parent->color == NodeColor::Black) return;
	//		
	//		grandParent = parent->parent;
	//		if (grandParent == nullptr || grandParent == _nil)
	//		{
	//			/** root node의 색이 red인 경우 black으로 변경한 후 return */
	//			parent->color = NodeColor::Black;
	//			return;
	//		}
	//		EChildDir parentDir = grandParent->child[(int)EChildDir::LEFT] == parent ? EChildDir::LEFT : EChildDir::RIGHT;
	//		RedBlackNode<T>* uncle = grandParent->child[1 - parentDir];
	//		/** grandparent가 black이고 uncle도 black인 경우 */
	//		if (uncle->color == NodeColor::Black)
	//		{
	//			/** 새로운 노드가 parent의 방향과 다르다면 parent를 기준으로 parent 방향으로 회전하여
	//			* 새로운 노드의 parent가 grandparent의 outer child가 되도록 만든다.
	//			* - 그렇게 하지 않고 parent를 grandparent 기준으로 parent의 반대 방향으로 회전할 경우
	//			* - parent를 black으로, grandparent를 red로 바꾼 후
	//			* - grandparent의 parent 방향 자식으로 새로운 노드를 넣어야 하는데 grandparent가 red가 되어야 하므로 불가능
	//			*/
	//			if (newNode == parent->child[1 - parentDir])
	//			{
	//				Rotate(parent, parentDir); // 회전 수행 -> parent가 newNode의 자식 노드가 되고 outer child가 됨.
	//				newNode = parent; // newNode의 parent가 outer child가 되었으므로 새로 삽입된 newNode로 간주하여 탐색 시작
	//				parent = grandParent->child[parentDir]; // 이전의 newNode가 새로운 parent가 되었으므로 parent 갱신
	//			}

	//			/**
	//			* newNode가 grandparent 기준으로 outer child 이므로 grandparent 기준으로 parent의 반대 방향으로 회전
	//			* parent가 grandparent의 parent가 된다. 그 후 parent를 black으로, grandparent를 red로 변경해준다면
	//			* parent를 root로 하는 sub tree는 root부터 각 leaf까지의 모든 경로에서 black의 수가 같아진다.
	//			*/
	//			Rotate(grandParent, 1 - parentDir);
	//			parent->color = NodeColor::Black;
	//			grandParent->color = NodeColor::Red;
	//			return;
	//		}
	//		else
	//		{
	//			/**
	//			 * 만약 parent, uncle이 모두 red라면 parent, uncle을 모두 black으로 바꾸고 grandparent를 red로 바꾼다.
	//			 * 그 후 grandparent를 기준으로 grandparent의 상위 노드가 red인지 다시 체크
	//			 * red black tree의 조건을 만족하도록 만든다.
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
	/** 삭제해야 할 노드 찾지 못함. */
	if (deleteNode == _nil)
		return;

	/**
	* TODO: deleteNode의 child가 2개라면 deleteNode의 leftmost child or rightmost child 노드의 값 가져오고
	* 그 노드를 삭제한 후 밸런싱 수행 -> 삭제 전 그 노드의 color, child 보관
	* Leftmost or rightmost child node는 그 node의 두 child가 모두 nil이거나 right or left child 하나만 가지고 있다.
	*/
	NodeColor originalColor = deleteNode->color;
	RedBlackNode<T>* deleteCopy = deleteNode;
	RedBlackNode<T>* childNode = nullptr;
	// child가 하나인 경우
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
		/** Leftmost child를 찾고 deleteCopy를 그 node로 변경. */
		deleteCopy = deleteNode->child[(int)EChildDir::RIGHT];
		while (deleteCopy->child[(int)EChildDir::LEFT] != _nil)
		{
			deleteCopy = deleteCopy->child[(int)EChildDir::LEFT];
		}
		originalColor = deleteCopy->color; // 삭제될 노드의 색깔 저장
		childNode = deleteCopy->child[(int)EChildDir::RIGHT];

		/**
		* 만약 deleteNode에서 두 level 이상 내려갔다면 deleteNode의 위치에 deleteNode의 child node를 넣고
		* deleteNode의 자식 노드를 deleteCopy에 넣어준다. */
		if (deleteCopy != deleteNode->child[(int)EChildDir::RIGHT])
		{
			TransPlant(deleteCopy, deleteCopy->child[(int)EChildDir::RIGHT]);
			deleteCopy->child[(int)EChildDir::RIGHT] = deleteNode->child[(int)EChildDir::RIGHT];
			deleteCopy->child[(int)EChildDir::RIGHT]->parent = deleteCopy;
		}
		else childNode->parent = deleteCopy;

		TransPlant(deleteNode, deleteCopy); // 실제 삭제될 노드의 값을 deleteNode에 가져온다.
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
			// Uncle도 red일 경우 parent, uncle 모두 black으로 바꾼 뒤 grandparent를 red로 바꾸고 grandparent 기준으로 다시 밸런싱 진행
			InNode->parent->color = NodeColor::Black;
			uncle->color = NodeColor::Black;
			InNode->parent->parent->color = NodeColor::Red;
			InNode = InNode->parent->parent;
		}
		else
		{
			// parent의 방향과 다를 시 parent 기준으로 회전하여 parent와 InNode가 같은 방향에 있게 만든다 (InNode가 parent의 parent가 됨)
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
