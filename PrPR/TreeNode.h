#include <iostream>
#include <queue>

struct TreeNode
{
	std::string value;
	TreeNode* left, * right;

	bool operator<(const TreeNode& other);
	bool operator==(const TreeNode& other);
	TreeNode();
	TreeNode(int s);
	TreeNode(const std::string& value);
};