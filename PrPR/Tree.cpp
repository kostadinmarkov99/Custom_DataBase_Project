#include "Tree.h"

TreeNode* Tree::find(TreeNode* r, const std::string& key) const
{
	if (!r)  return nullptr;

	if (r->value == key) return r;

	return r->value > key ? find(r->left, key) : find(r->right, key);
}

TreeNode* Tree::insert(TreeNode*& r, std::string& val)
{
	if (!r)
	{
		r = new TreeNode(val);
		return r;
	}
	if (val < r->value) r->left = insert(r->left, val);
	else r->right = insert(r->right, val);
	if (val == r->value) return r;

}

TreeNode* Tree::findParent(TreeNode* r, const std::string& key)
{
	if (!r) return nullptr;
	if (r->left->value == key || r->right->value == key)
	{
		return r;
	}
	findParent(r->left, key);
	findParent(r->right, key);
}

TreeNode* Tree::removeNode(TreeNode* root, std::string data) {
	if (root == nullptr) {
		return nullptr;
	}
	if (data < root->value) {  // data is in the left sub tree.
		root->left = removeNode(root->left, data);
	}
	else if (data > root->value) { // data is in the right sub tree.
		root->right = removeNode(root->right, data);
	}
	else {
		// case 1: no children
		if (root->left == nullptr && root->right == nullptr) {
			delete(root); // wipe out the memory, in C, use free function
			root = nullptr;
		}
		// case 2: one child (right)
		else if (root->left == nullptr) {
			TreeNode* temp = root; // save current node as a backup
			root = root->right;
			delete temp;
		}
		// case 3: one child (left)
		else if (root->right == nullptr) {
			TreeNode* temp = root; // save current node as a backup
			root = root->left;
			delete temp;
		}
		// case 4: two children
		else {
			TreeNode* temp = FindMin(root->right); // find minimal value of right sub tree
			root->value = temp->value; // duplicate the node
			root->right = removeNode(root->right, temp->value); // delete the duplicate node
		}
	}
	return root; // parent node can update reference
}

TreeNode* Tree::FindMin(TreeNode* root) {
	if (root == nullptr) {
		return nullptr; // or undefined.
	}
	if (root->left != NULL) {
		return FindMin(root->left); // left tree is smaller
	}
	return root;
}

bool Tree::remove(TreeNode*& r, const std::string& name)
{
	if (r == nullptr) return false;
	TreeNode* toDelete = find(name);
	TreeNode* parent = findParent(r, name);

	int children = !toDelete->left + !toDelete->right;
	if (children == 0)
	{
		if (parent->left->value == name) parent->left = nullptr;
		else parent->right = nullptr;

		delete toDelete;
		return true;
	}

	else if (children == 1)
	{
		TreeNode* pos = parent->left->value == name ? parent->left : parent->right;
		TreeNode* par = findParent(r, name);

		toDelete->left ? pos = toDelete->left : pos = toDelete->right;
		delete toDelete;
		return true;
	}

	TreeNode* del;
	TreeNode* maxLeft = toDelete->left;
	while (maxLeft->right)
	{
		del = maxLeft;
		maxLeft = maxLeft->right;
	}

	maxLeft->right = nullptr;
	toDelete->value = maxLeft->value;
	delete maxLeft;

	return true;
}

TreeNode* Tree::copy(TreeNode* r, TreeNode* other)
{
	if (!r) return nullptr;

	TreeNode* c = new TreeNode();

	c->left = copy(r->left, other);
	c->right = copy(r->right, other);

	return c;
}

void Tree::clear(TreeNode* r)
{
	if (!r) return;

	clear(r->left);
	clear(r->right);

	delete r;
}

size_t Tree::getfullCount(TreeNode*& root)
{
	if (root == NULL) {
		return 0;
	}
	else {
		return 1 + getfullCount(root->left) + getfullCount(root->right);
	}
}

void Tree::printLevel(TreeNode* r)
{
	if (!r) return;

	std::queue<TreeNode*> q;

	q.push(r);

	TreeNode* p;

	while (!q.empty())
	{
		p = q.front();
		q.pop();

		std::cout << p->value << " ";

		if (p->left)
			q.push(p->left);
		if (p->right)
			q.push(p->right);
	}
}

void Tree::printLeafNodes(TreeNode* root)
{
	if (!root)
		return;

	if (!root->left && !root->right)
	{
		std::cout << root->value << " ";
		return;
	}

	if (root->left)
		printLeafNodes(root->left);

	if (root->right)
		printLeafNodes(root->right);
}

TreeNode* Tree::insertNode(TreeNode*& t, std::string x)
{
	if (t == NULL)
	{
		t = new TreeNode;
		t->value = x;
		t->left = t->right = NULL;
	}
	else if (x < t->value)
		t->left = insertNode(t->left, x);
	else if (x > t->value)
		t->right = insertNode(t->right, x);
	return t;
}

Tree::Tree() : root(nullptr) {}
Tree::Tree(const Tree& other)
{
	copy(root, other.root);
}
Tree::Tree(TreeNode*& r)
{
	root = r;
}
Tree::~Tree()
{
	clear(root);
}

void Tree::printLevel()
{
	return printLevel(root);
}

Tree& Tree::operator=(Tree& other)
{
	copy(root, other.root);
	return *this;
}

TreeNode* Tree::getRoot()
{
	return this->root;
}

TreeNode* Tree::find(const std::string& key) const
{
	return find(root, key);
}
TreeNode* Tree::insert(std::string& addedNode)
{
	return insert(root, addedNode);
	//return root;
}
TreeNode* Tree::insertNode(std::string& key)
{
	return insertNode(root, key);
}
bool Tree::remove(const std::string& removedName)
{
	return remove(root, removedName);
}
std::string Tree::serialize(TreeNode* root) {
	if (root == nullptr) {
		return "";
	}
	std::string s = root->value +
		"(" + serialize(root->left) + ")";
	if (root->right != nullptr) {
		s += "(" + serialize(root->right) + ")";
	}
	return s;
}

TreeNode* Tree::removeNode(std::string data)
{
	return removeNode(root, data);
}

size_t Tree::getfullCount()
{
	return getfullCount(root);
}

TreeNode* Tree::deserialize(std::string data) {
	std::string s = data;
	if (s.size() == 0) {
		return nullptr;
	}
	if (s[0] == ')') return nullptr;
	int j = 0;
	while (j < s.size() && s[j] != '(') j++;
	TreeNode* root = new TreeNode((s.substr(0, j)));
	int left = 0, i = j;
	// find separation between left and right definition
	while (i < s.size()) {
		if (s[i] == '(') {
			left++;
		}
		else if (s[i] == ')') {
			left--;
		}
		if (left == 0) {
			break;
		}
		i++;
	}
	if (j < s.size() - 1) {
		root->left = deserialize(s.substr(j + 1, i - 1 - j));
	}
	if (i + 1 < s.size() - 1) {
		root->right = deserialize(s.substr(i + 2, s.size() - i - 2));
	}
	return root;
}

void Tree::printLeafNodes()
{
	std::cout << root->value << " ";
	printLeafNodes(root);
}