#include <iostream>
#include <vector>

using namespace std;

struct Versions {
	int last_version;
	int current_version;
	int future_version;
};

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

	U get(T key);
	U get(int version, T key);

	void deleteKey(T key);
	void deleteKey(int version, T key);

	void insert(T key, U value);
	void insert(int version, T key, U value);

	void showTree(int version);

	void undo();
	void redo();
private:
	typedef Node<T>* NodePtr;
	vector<NodePtr> ROOT;
	NodePtr TNULL;
	Versions HISTORY;

	void addInTree(NodePtr root, T key, U value);
	void partialCopyTree(NodePtr root, int version, T key);

	void printHelper(NodePtr root, string indent, bool last) {
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
			printHelper(root->left, indent, false);
			printHelper(root->right, indent, true);
		}
	}

	void endUpdate(NodePtr node) {
		if (node != TNULL) {
			NodePtr u;
			if (node != node->left->parent) {
				if (node->left != TNULL) {
					u = new Node<T>;
					u->parent = node;
					u->left = node->left->left;
					u->right = node->left->right;
					u->key = node->left->key;
					u->value = node->left->value;
					u->color = node->left->color;
					node->left = u;
				}
			}

			if (node != node->right->parent) {
				if (node->right != TNULL) {
					u = new Node<T>;
					u->parent = node;
					u->left = node->right->left;
					u->right = node->right->right;
					u->key = node->right->key;
					u->value = node->right->value;
					u->color = node->right->color;
					node->right = u;
				}
			}

			endUpdate(node->left);
			endUpdate(node->right);
		}
	}

	void leftRotate(NodePtr x) {
		NodePtr y = x->right;
		x->right = y->left;
		if (y->left != TNULL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			int root_size = (this->ROOT.size() - 1);
			this->ROOT[root_size] = y;
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

	void rightRotate(NodePtr x) {
		NodePtr y = x->left;
		x->left = y->right;
		if (y->right != TNULL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			int root_size = (this->ROOT.size() - 1);
			this->ROOT[root_size] = y;
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

	void fixInsert(NodePtr k, NodePtr root) {
		NodePtr u, e;

		while (k->parent->color == 1) {
			if (k->parent == k->parent->parent->right) {
				e = k->parent->parent->left;

				if (e->parent != k->parent->parent) {
					if (e != TNULL) {
						u = new Node<T>;
						u->parent = k->parent->parent;
						u->left = e->left;
						u->right = e->right;
						u->key = e->key;
						u->value = e->value;
						u->color = e->color;
					}
					else {
						u = e;
					}

					k->parent->parent->left = u;
				}
				else
				{
					u = e;
				}

				if (u->color == 1) {
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				}
				else {
					if (k == k->parent->left) {
						k = k->parent;
						rightRotate(k);
					}
					k->parent->color = 0;
					k->parent->parent->color = 1;
					leftRotate(k->parent->parent);
				}
			}
			else {
				e = k->parent->parent->right;

				if (e->parent != k->parent->parent) {
					if (e != TNULL) {
						u = new Node<T>;
						u->parent = k->parent->parent;
						u->left = e->left;
						u->right = e->right;
						u->key = e->key;
						u->value = e->value;
						u->color = e->color;
					}
					else {
						u = e;
					}

					k->parent->parent->right = u;
				}
				else
				{
					u = e;
				}

				if (u->color == 1) {
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				}
				else {
					if (k == k->parent->right) {
						k = k->parent;
						leftRotate(k);
					}
					k->parent->color = 0;
					k->parent->parent->color = 1;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root) {
				break;
			}
		}
		root->color = 0;
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

	NodePtr minimum(NodePtr node) {
		while (node->left != TNULL) {
			node = node->left;
		}
		return node;
	}

	void rbTransplant(NodePtr u, NodePtr v) {
		if (u->parent == nullptr) {
			int root_size = (this->ROOT.size() - 1);
			this->ROOT[root_size] = v;
		}
		else if (u == u->parent->left) {
			u->parent->left = v;
		}
		else {
			u->parent->right = v;
		}
		v->parent = u->parent;
	}

	void fixDelete(NodePtr x) {
		NodePtr s, root;
		int root_size = (this->ROOT.size() - 1);
		root = this->ROOT[root_size];

		while (x != root && x->color == 0) {
			if (x == x->parent->left) {
				s = x->parent->right;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					leftRotate(x->parent);
					s = x->parent->right;
				}

				if (s->left->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				}
				else {
					if (s->right->color == 0) {
						// case 3.3
						s->left->color = 0;
						s->color = 1;
						rightRotate(s);
						s = x->parent->right;
					}

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->right->color = 0;
					leftRotate(x->parent);
					x = root;
				}
			}
			else {
				s = x->parent->left;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->left->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				}
				else {
					if (s->left->color == 0) {
						// case 3.3
						s->right->color = 0;
						s->color = 1;
						leftRotate(s);
						s = x->parent->left;
					}

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->left->color = 0;
					rightRotate(x->parent);
					x = root;
				}
			}
		}
		x->color = 0;
	}

	void deleteKeyHelper(NodePtr node, T key) {
		// find the node containing key
		NodePtr z = TNULL;
		NodePtr x, y;
		while (node != TNULL) {
			if (node->key == key) {
				z = node;
			}

			if (node->key <= key) {
				node = node->right;
			}
			else {
				node = node->left;
			}
		}

		if (z == TNULL) {
			cout << "Couldn't find key in the tree" << endl;
			return;
		}

		endUpdate(z);

		y = z;
		int y_original_color = y->color;
		if (z->left == TNULL) {
			x = z->right;
			rbTransplant(z, z->right);
		}
		else if (z->right == TNULL) {
			x = z->left;
			rbTransplant(z, z->left);
		}
		else {
			y = minimum(z->right);
			y_original_color = y->color;
			x = y->right;
			if (y->parent == z) {
				x->parent = y;
			}
			else {
				rbTransplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}

			rbTransplant(z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}
		delete z;
		if (y_original_color == 0) {
			fixDelete(x);
		}
	}
};

//конструктор
template <typename T, typename U>
PersistentMap<T, U>::PersistentMap() 
{
	HISTORY.last_version = 0;
	HISTORY.current_version = 0;
	HISTORY.future_version = 0;

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

		fixInsert(node, root);
	}
}

template <typename T, typename U>
void PersistentMap<T, U>::partialCopyTree(NodePtr root, int i, T key)
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
		partialCopyTree(root, i - 1, key);
		addInTree(root, key, value);

		this->HISTORY.last_version = this->HISTORY.current_version;
		this->HISTORY.current_version = i;
		this->HISTORY.future_version = i;
	}
	else if (i >= 0 && i <= root_size) 
	{
		if (searchTreeHelper(this->ROOT[i], key)->value != value) {
			NodePtr root = new Node<T>;
			partialCopyTree(root, i, key);
			addInTree(root, key, value);

			this->HISTORY.last_version = this->HISTORY.current_version;
			this->HISTORY.current_version = root_size + 1;
			this->HISTORY.future_version = root_size + 1;
		}
	}
}

template <typename T, typename U>
void PersistentMap<T, U>::insert(T key, U value)
{
	int current_version = this->HISTORY.current_version;

	if (this->ROOT[0] == this->TNULL) {
		insert(current_version, key, value);
	}
	else {
		insert(current_version + 1, key, value);
	}

}

//вывод версии дерева
template <typename T, typename U>
void PersistentMap<T, U>::showTree(int version)
{
	if (version <= (this->ROOT.size() - 1)) {
		if (ROOT[version]) {
			cout << "Version: " << version << endl;
			printHelper(this->ROOT[version], "", true);
			cout << endl;
		}
	}
}

//поиск значения по ключу
template <typename T, typename U>
U PersistentMap<T, U>::get(int version, T key)
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
U PersistentMap<T, U>::get(T key) {
	return get(this->HISTORY.current_version, key);
}

template <typename T, typename U>
void PersistentMap<T, U>::deleteKey(int version, T key) {
	if (version <= (this->ROOT.size() - 1)) {
		if (searchTreeHelper(this->ROOT[version], key)->key != this->TNULL->key) {
			NodePtr root = new Node<T>;
			partialCopyTree(root, version, key);
			deleteKeyHelper(root, key);

			this->HISTORY.last_version = this->HISTORY.current_version;
			this->HISTORY.current_version = this->ROOT.size();
			this->HISTORY.future_version = this->ROOT.size();
		}
	}
}

template <typename T, typename U>
void PersistentMap<T, U>::deleteKey(T key) {
	deleteKey(this->HISTORY.current_version, key);
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

template <typename T, typename U>
void PersistentMap<T, U>::undo() {
	this->HISTORY.current_version = this->HISTORY.last_version;
}

template <typename T, typename U>
void PersistentMap<T, U>::redo() {
	this->HISTORY.current_version = this->HISTORY.future_version;
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
	pMap.showTree(6);
	pMap.showTree(7);
	pMap.showTree(6);

	pMap.insert(7, 90, 88);
	pMap.showTree(8);

	pMap.insert(7, 8, 88);
	pMap.showTree(9);

	pMap.insert(7, 8, 11);

	cout << "Version 7, key 40: " <<pMap.get(7, 40) << endl;
	cout << "Version 7, key 8: " << pMap.get(7, 8) << endl;
	cout << "Version 9, key 8: " << pMap.get(9, 8) << endl;
	cout << endl;
	cout << "Version 100, key 40: " << pMap.get(100, 40) << endl;
	cout << "Version 7, key 42: " << pMap.get(7, 42) << endl;
	cout << endl;
	cout << "Size: " << pMap.size() << endl;
	cout << endl;

	pMap.showTree(7);
	pMap.deleteKey(7, 17);
	pMap.showTree(10);
	pMap.deleteKey(7, 25);
	pMap.showTree(11);
	pMap.showTree(7);

	pMap.deleteKey(7, 99);


	PersistentMap<int, int> myMap;
	myMap.insert(8, 11);
	myMap.insert(5, 22);
	myMap.insert(18, 28);
	myMap.insert(15, 35);
	myMap.insert(17, 47);
	myMap.insert(25, 55);
	myMap.insert(40, 60);
	myMap.insert(80, 78);

	myMap.undo();
	//myMap.redo();

	myMap.insert(90, 98);

	return 0;
}