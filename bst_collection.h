#ifndef BSTCOLLECTIONH
#define BSTCOLLECTIONH

#include "collection.h"

int counter = 0;

template<typename K, typename V>
class BSTCollection : public Collection<K,V>
{
public:
BSTCollection();
BSTCollection(const BSTCollection<K,V>& rhs);
~BSTCollection();
BSTCollection& operator=(const BSTCollection<K,V>& rhs);
void add(const K& A_key, const V& A_val);
void remove(const K& A_key);
bool find(const K& search_key, V& the_val) const;
void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
void keys(ArrayList<K>& all_keys) const;
void sort(ArrayList<K>& all_keys_sorted) const;
size_t height() const;
size_t size() const;
private:
struct Node {
	K key;
	V value;
	Node* left;
	Node* right;
};
Node* root;
size_t node_count;
void make_empty(Node* subtree_root);
void copy(Node* lhs_subtree_root, const Node* rhs_subtree_root);
Node* remove(Node* subtree_root, const K& a_key);
void find (const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const;
void keys(const Node* subtree_root, ArrayList<K>& all_keys) const;
size_t height(const Node* subtree_root) const;
};

template<typename K, typename V>
BSTCollection<K,V>::BSTCollection(){
	root = NULL;
	node_count = 0;
}

template<typename K, typename V>
BSTCollection<K,V>::BSTCollection(const BSTCollection<K,V>& rhs){
	node_count = 0;
	*this = rhs;
}
template<typename K, typename V>
BSTCollection<K,V>::~BSTCollection(){
	make_empty(root);
}
template<typename K, typename V>
BSTCollection<K,V>& BSTCollection<K,V>:: operator=(const BSTCollection<K,V>& rhs)
{
	if (this != &rhs)
	{
	root = NULL;
	copy(root, rhs.root);
	node_count = rhs.size();
	}
	return *this;
}
template<typename K, typename V>
void BSTCollection<K,V>::add(const K& A_key, const V& A_val)
{
	node_count++;
	if (root == NULL)
	{
		root = new Node;
		root->key = A_key;
		root->value = A_val;
		root->left = root->right = NULL;
		return;
	}
	Node* tmp = root;
	Node* tmp2 = NULL;
	while (tmp != NULL)
	{
	 tmp2 = tmp;
 	 if (A_key < tmp->key)
 	 {
 		tmp = tmp->left;
	 }
	 else
	 {
	 	tmp = tmp->right;
	 }
	}
	if (A_key < tmp2->key)
	{
		tmp = new Node;
		tmp->key = A_key;
		tmp->value = A_val;
		tmp->left = NULL;
		tmp->right = NULL;
		tmp2->left = tmp;
	}
	else
	{
		tmp = new Node;
		tmp->key = A_key;
		tmp->value = A_val;
		tmp->left = NULL;
		tmp->right = NULL;
		tmp2->right = tmp;
	}

}

template<typename K, typename V>
void BSTCollection<K,V>::remove(const K& A_key)
{
	root = remove(root, A_key);
	node_count--;
}
template<typename K, typename V>
bool BSTCollection<K,V>::find(const K& search_key, V& the_val) const
{
	Node* tmp = root;
	while (tmp != NULL)
	{
		if (tmp->key == search_key)
		{
			the_val = tmp->value;
			return true;
		}
		else if (tmp->key < search_key)
			tmp = tmp->right;
		else
			tmp = tmp->left;
	}
	return false;
}
template<typename K, typename V>
void BSTCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
	find(root,k1,k2,keys);
}
template<typename K, typename V>
void BSTCollection<K,V>::keys(ArrayList<K>& all_keys) const
{
	keys(root, all_keys);
}
template<typename K, typename V>
void BSTCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
	keys(root, all_keys_sorted);
	all_keys_sorted.sort();
}
template<typename K, typename V>
size_t BSTCollection<K,V>::height() const
{
	height(root);
}
template<typename K, typename V>
void BSTCollection<K,V>::keys(const Node* subtree_root, ArrayList<K>& all_keys) const
{
	//std::cout << "here\n";
	if (subtree_root == NULL)
		return;
	all_keys.add(subtree_root->key);
	keys(subtree_root->left, all_keys);
	keys(subtree_root->right, all_keys);
}
template<typename K, typename V>
void BSTCollection<K,V>::find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const
{	
	if (subtree_root == NULL)
		return;
	find(subtree_root->left, k1, k2, keys);
	if (subtree_root->key >= k1 && subtree_root->key <= k2)
		keys.add(subtree_root->key);
	find(subtree_root->right, k1, k2, keys);
}
template<typename K, typename V>
typename BSTCollection<K,V>::Node* BSTCollection<K,V>::remove(Node* subtree_root, const K& a_key)
{
	if (subtree_root == NULL)
		return subtree_root;
	if (a_key < subtree_root->key)
		subtree_root->left = remove(subtree_root->left, a_key);
	else if(a_key > subtree_root->key)
		subtree_root->right = remove(subtree_root->right, a_key);
	else if (a_key == subtree_root->key)
	{
		if (subtree_root->right == NULL && subtree_root->left == NULL)			             {
			Node* tmp = subtree_root->left;
			delete subtree_root;
			subtree_root = tmp;
		}
		else if (subtree_root->left == NULL)
		{
			Node* tmp = subtree_root->right;
			delete subtree_root;
			subtree_root = tmp;
		}
		else
		{
			Node* tmp = subtree_root->right;
			while (tmp->left != NULL)
			{
				tmp = tmp->left;
			}
			subtree_root->key = tmp->key;
			subtree_root->value = tmp->value;
			subtree_root->right = remove(subtree_root->right, tmp->key);
			}
	}
	return subtree_root;
}
template<typename K, typename V>
void BSTCollection<K,V>::copy(Node* lhs_subtree_root, const Node* rhs_subtree_root)
{
	if (rhs_subtree_root == NULL)
	{
	 lhs_subtree_root = NULL;
	}
	else
	{
	 if (root == NULL)
	 {
		 root = new Node;
		 lhs_subtree_root = root;
	 }
	 lhs_subtree_root->key = rhs_subtree_root->key;
	 lhs_subtree_root->value = rhs_subtree_root->value;
	 lhs_subtree_root->left = new Node;
	 lhs_subtree_root->right = new Node;
	 copy(lhs_subtree_root->right, rhs_subtree_root->right);
	 copy(lhs_subtree_root->left, rhs_subtree_root->left);	
	}
}
template<typename K, typename V>
void BSTCollection<K,V>::make_empty(Node* subtree_root)
{
	if(subtree_root == NULL)
		return;
	make_empty(subtree_root->right);
	make_empty(subtree_root->left);
	delete subtree_root;
}
template<typename K, typename V>
size_t BSTCollection<K,V>::height(const Node* subtree_root) const
{
	if (subtree_root == NULL)
		return 0;
	int l = height(subtree_root->left);
	int r = height(subtree_root->right);
	if (r > l)
		return r+1;
	else
		return l+1;

}
template<typename K, typename V>
size_t BSTCollection<K,V>::size() const
{
	return node_count;
}


#endif

	

