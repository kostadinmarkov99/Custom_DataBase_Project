#include "TreeNode.h"

TreeNode::TreeNode(int s)
{
	value = "NoName";
	left = right = nullptr;
}
TreeNode::TreeNode(const std::string& value)
{
	this->value = value;
	left = right = nullptr;
}

TreeNode::TreeNode()
{
	value = "NoName";
	left = right = nullptr;
}

bool TreeNode::operator<(const TreeNode& other)
{
	return value < other.value; // lekxicographical compair
}

bool TreeNode::operator==(const TreeNode& other)
{
	return value == other.value; // leksicographical compair
}