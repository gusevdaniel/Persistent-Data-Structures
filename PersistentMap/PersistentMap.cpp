#include <iostream>
#include <vector>

using namespace std;

template <typename T>
struct Node {
	T key;
	T value;
	Node<T>* parent; 
	Node<T>* left;
	Node<T>* right;
	int color;
};

//класс
template <typename T, typename U>
class PersistentMap
{
public:
	PersistentMap();	
	int size();
	U find(int version, T key);
	void insert(int version, T key, U value);
	void showTree(int version);
private:
	typedef Node<T>* NodePtr;
	vector<NodePtr> ROOT;
	NodePtr TNULL;

	void addInTree(NodePtr root, T key, U value);
	void copyTree(NodePtr root, int version, T key);

	void printHelper(NodePtr root, int i, string indent, bool last) {
		if (root != TNULL) {
			cout << indent;
			if (last) {
				cout << "R----";
				indent += "     ";
			}
			else {
				cout << "L----";
				indent += "|    ";
			}

			string sColor = root->color ? "RED" : "BLACK";
			cout << root->key << "(" << sColor << ")" << endl;
			printHelper(root->left, i, indent, false);
			printHelper(root->right, i, indent, true);
		}
	}

	// rotate left at node x
	void leftRotate(NodePtr x, NodePtr root) {
		NodePtr y = x->right;
		x->right = y->left;
		if (y->left != TNULL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			root = y;
		}
		else if (x == x->parent->left) {
			x->parent->left = y;
		}
		else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	// rotate right at node x
	void rightRotate(NodePtr x, NodePtr root) {
		NodePtr y = x->left;
		x->left = y->right;
		if (y->right != TNULL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			root = y;
		}
		else if (x == x->parent->right) {
			x->parent->right = y;
		}
		else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	NodePtr searchTreeHelper(NodePtr node, T key) {
		if (node == TNULL || key == node->key) {
			return node;
		}

		if (key < node->key) {
			return searchTreeHelper(node->left, key);
		}
		return searchTreeHelper(node->right, key);
	}
};

//конструктор
template <typename T, typename U>
PersistentMap<T, U>::PersistentMap()
{
	TNULL = new Node<T>;
	TNULL->color = 0;
	TNULL->left = nullptr;
	TNULL->right = nullptr;

	NodePtr root;
	root = TNULL;

	ROOT.push_back(root);
}

template <typename T, typename U>
void PersistentMap<T, U>::addInTree(NodePtr root, T key, U value)
{
	if (searchTreeHelper(root, key) != TNULL) {
		searchTreeHelper(root, key)->value = value;
	}
	else {
		NodePtr node = new Node<T>;
		node->parent = nullptr;
		node->key = key;
		node->value = value;
		node->left = TNULL;
		node->right = TNULL;
		node->color = 1;

		NodePtr y = nullptr;
		NodePtr x = root;

		while (x != TNULL) {
			y = x;
			if (node->key < x->key) {
				x = x->left;
			}
			else {
				x = x->right;
			}
		}

		node->parent = y;
		if (y == nullptr) {
			ROOT[this->ROOT.size() - 1] = node;
		}
		else if (node->key < y->key) {
			y->left = node;
		}
		else {
			y->right = node;
		}

		if (node->parent == nullptr) {
			node->color = 0;
			return;
		}

		if (node->parent->parent == nullptr) {
			return;
		}
	}
}

template <typename T, typename U>
void PersistentMap<T, U>::copyTree(NodePtr root, int i, T key)
{
	root->parent = nullptr;
	root->left = TNULL;
	root->right = TNULL;
	root->key = this->ROOT[i]->key;
	root->value = this->ROOT[i]->value;
	root->color = this->ROOT[i]->color;

	NodePtr x = this->ROOT[i];

	while (x != TNULL) {
		if (key < x->key) {
			if (x->left != TNULL)
			{
				NodePtr node = new Node<T>;
				node->parent = root;
				node->key = x->left->key;
				node->value = x->left->value;
				node->color = x->left->color;
				node->left = TNULL;
				node->right = TNULL;

				root->left = node;
				root->right = x->right;

				root = root->left;
				x = x->left;
			}
			else {
				root->right = x->right;
				break;
			}
		}
		else {
			if (x->right != TNULL)
			{
				NodePtr node = new Node<T>;
				node->parent = root;
				node->key = x->right->key;
				node->value = x->right->value;
				node->color = x->right->color;
				node->left = TNULL;
				node->right = TNULL;

				root->right = node;
				root->left = x->left;

				root = root->right;
				x = x->right;
			}
			else {
				root->left = x->left;
				break;
			}
		}
	}

	while (root->parent != nullptr)
	{
		root = root->parent;
	}

	this->ROOT.push_back(root);
}

template <typename T, typename U>
void PersistentMap<T, U>::insert(int i, T key, U value)
{
	int root_size = (this->ROOT.size() - 1);

	if (i == 0 && this->ROOT[i] == TNULL) {
		addInTree(this->ROOT[i], key, value);
	}
	else if (i > root_size) 
	{
		NodePtr root = new Node<T>;
		copyTree(root, i - 1, key);
		addInTree(root, key, value);
	}
	else if (i >= 0 && i <= root_size) 
	{
		if (searchTreeHelper(this->ROOT[i], key)->value != value) {
			NodePtr root = new Node<T>;
			copyTree(root, i, key);
			addInTree(root, key, value);
		}
	}
}

//вывод версии дерева
template <typename T, typename U>
void PersistentMap<T, U>::showTree(int version)
{
	if (version <= (this->ROOT.size() - 1)) {
		if (ROOT[version]) {
			cout << "Version: " << version << endl;
			printHelper(this->ROOT[version], version, "", true);
			cout << endl;
		}
	}
}

//поиск значения по ключу
template <typename T, typename U>
U PersistentMap<T, U>::find(int version, T key)
{
	if (version <= (this->ROOT.size() - 1))
	{
		return searchTreeHelper(this->ROOT[version], key)->value;
	}
	else {
		return TNULL->value;
	}
}

template <typename T, typename U>
int PersistentMap<T, U>::size()
{
	if (this->ROOT[0] != TNULL) {
		return this->ROOT.size();
	}
	else {
		return 0;
	}
}

int main()
{
	PersistentMap<int, int> pMap;
	pMap.insert(0, 8, 11);
	pMap.showTree(0);

	pMap.insert(1, 5, 22);
	pMap.showTree(1);

	pMap.insert(2, 18, 28);
	pMap.insert(3, 15, 35);
	pMap.insert(4, 17, 47);
	pMap.insert(5, 25, 55);
	pMap.insert(6, 40, 60);
	pMap.insert(7, 80, 78);
	pMap.insert(7, 90, 88);
	pMap.insert(7, 8, 88);
	pMap.insert(7, 8, 11);

	cout << "Value of key 40: " <<pMap.find(7, 40) << endl;
	cout << "Value of key 8: " << pMap.find(7, 8) << endl;
	cout << "Value of key 8: " << pMap.find(9, 8) << endl;

	cout << pMap.find(100, 40) << endl;
	cout << pMap.find(7, 42) << endl;

	cout << pMap.size() << endl;

	return 0;
}