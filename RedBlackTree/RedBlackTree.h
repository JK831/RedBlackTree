#pragma once

enum class NodeColor
{
	Black,
	Red,
};

struct RedBlackNode
{
	NodeColor color;
	RedBlackNode* left;
	RedBlackNode* right;
};


class RedBlackTree
{
	RedBlackNode* _root;
	RedBlackNode* _nil;
};

