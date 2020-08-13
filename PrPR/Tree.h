#pragma once
#include <iostream>
#include <queue>
#include "TreeNode.h"

class Tree
{
private:
	TreeNode* root;

	TreeNode* find(TreeNode* r, const std::string& key) const;
	TreeNode* insert(TreeNode*& r, std::string& val);
	TreeNode* findParent(TreeNode* r, const std::string& key);
	TreeNode* removeNode(TreeNode* root, std::string data);
	TreeNode* FindMin(TreeNode* root);
	bool remove(TreeNode*& r, const std::string& name);
	TreeNode* copy(TreeNode* r, TreeNode* other);
	void clear(TreeNode* r);
	size_t getfullCount(TreeNode*& root);
	void printLevel(TreeNode* r);
	void printLeafNodes(TreeNode* root);
	TreeNode* insertNode(TreeNode*& t, std::string x);

public:
	Tree();
	Tree(const Tree& other);
	Tree(TreeNode*& r);
	~Tree();

	void printLevel();

	Tree& operator=(Tree& other);

	TreeNode* getRoot();

	TreeNode* find(const std::string& key) const;
	TreeNode* insert(std::string& addedNode);
	TreeNode* insertNode(std::string& key);
	bool remove(const std::string& removedName);
	std::string serialize(TreeNode* root);
	TreeNode* removeNode(std::string data);
	size_t getfullCount();
	TreeNode* deserialize(std::string data);
	void printLeafNodes();
};