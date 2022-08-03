#ifndef AVLCOLLECTIONH
#define AVLCOLLECTIONH

#include "collection.h"

template<typename K, typename V>
class AVLCollection : public Collection<K,V>
{
public:
AVLCollection();
AVLCollection(const AVLCollection<K,V>& rhs);
~AVLCollection();
AVLCollection& operator=(const AVLCollection<K,V>& rhs);
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
	int height;
        Node* left;
        Node* right;
};
Node* root;
int balance_factor(Node* subtree_root);
size_t node_count;
void print_tree(std::string indent, Node* subtree_root);
void make_empty(Node* subtree_root);
void copy(Node* lhs_subtree_root, const Node* rhs_subtree_root);
Node* add(Node* subtree_root, const K& a_key, const V& a_val);
Node* remove(Node* subtree_root, const K& a_key);
void find (const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const;
void keys(const Node* subtree_root, ArrayList<K>& all_keys) const;
size_t height(const Node* subtree_root) const;
Node* rotate_right(Node* k2);
Node* rotate_left(Node* k2);
Node* rebalance(Node* subtree_root);
};

template<typename K, typename V>
AVLCollection<K,V>::AVLCollection()
{
	root = NULL;
	node_count = 0;
}
template<typename K, typename V>
AVLCollection<K,V>::AVLCollection(const AVLCollection<K,V>& rhs)
{
	node_count = 0;
	*this = rhs;
}
template<typename K, typename V>
AVLCollection<K,V>::~AVLCollection()
{
	make_empty(root);
}
template<typename K, typename V>
AVLCollection<K,V>& AVLCollection<K,V>:: operator=(const AVLCollection<K,V>& rhs)
{
	if (this != &rhs)
	{
		root = NULL;
		make_empty(root);
		copy(root, rhs.root);
		node_count = rhs.size();
	}
	return *this;
}
template<typename K, typename V>
void AVLCollection<K,V>::add(const K& A_key, const V& A_val)
{
	root = add(root, A_key, A_val);
	node_count++;	
}
template<typename K, typename V>
void AVLCollection<K,V>::remove(const K& A_key)
{
	root = remove(root, A_key);
	node_count--;
}
template<typename K, typename V>
bool AVLCollection<K,V>::find(const K& search_key, V& the_val) const
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
void AVLCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
	find(root, k1,k2, keys);
}
template<typename K, typename V>
void AVLCollection<K,V>::keys(ArrayList<K>& all_keys) const
{
	keys(root, all_keys);
}
template<typename K, typename V>
void AVLCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
	keys(root, all_keys_sorted);
	//all_keys_sorted.sort();
}
template<typename K, typename V>
size_t AVLCollection<K,V>::height() const
{
	if (root == NULL)
		return 0;
	return root->height;
	//height(root);
}
template<typename K,typename V>
size_t AVLCollection<K,V>::size() const
{
	return node_count;
}
template<typename K, typename V>
int AVLCollection<K,V>::balance_factor(Node* subtree_root)
{
	if (subtree_root == NULL)
		return 0;
	Node* lptr = subtree_root->left;
	Node* rptr = subtree_root->right;
	if (lptr == NULL && rptr == NULL)
		return 0;
	if (rptr == NULL && lptr != NULL)
		return lptr->height;
	if (lptr == NULL && rptr != NULL)
		return 0 - rptr->height;
	return (lptr->height - rptr->height);
}
template<typename K, typename V>
void AVLCollection<K,V>::keys(const Node* subtree_root, ArrayList<K>& all_keys) const
{
	if (subtree_root == NULL)
		return;
	//all_keys.add(subtree_root->key);
	keys(subtree_root->left, all_keys);
	all_keys.add(subtree_root->key);
	keys(subtree_root->right, all_keys);
}
template<typename K, typename V>
void AVLCollection<K,V>::find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const
{
	if (subtree_root == NULL)
		return;
	find(subtree_root->left, k1, k2, keys);
	if (subtree_root->key >= k1 && subtree_root->key <= k2)
		keys.add(subtree_root->key);
	find(subtree_root->right, k1, k2, keys);
}
template<typename K, typename V>
typename AVLCollection<K,V>::Node* AVLCollection<K,V>::add(Node* subtree_root, const K& a_key, const V& a_val)
{
	
	if (subtree_root == NULL)
	{
		subtree_root = new Node;
		subtree_root->key = a_key;
		subtree_root->value = a_val;
		subtree_root->left = NULL;
		subtree_root->right = NULL;
		subtree_root->height = 1;
	}
	else
	{
		if(a_key < subtree_root->key)
			subtree_root->left = add(subtree_root->left, a_key, a_val);
		else
			subtree_root->right = add(subtree_root->right, a_key, a_val);
		if (subtree_root->left == NULL)
		       subtree_root->height = 1 + subtree_root->right->height;
		else if(subtree_root->right == NULL)
			subtree_root->height = 1 + subtree_root->left->height;
		else
		{
			if (subtree_root->right->height > subtree_root->left->height)
				subtree_root->height = 1 + subtree_root->right->height;
			else
				subtree_root->height = 1 + subtree_root->left->height;
		}	
	}
	//std::cout << "add subtree height: " << subtree_root->height << std::endl;
	return rebalance(subtree_root);
}
template<typename K, typename V>
typename AVLCollection<K,V>::Node* AVLCollection<K,V>::remove(Node* subtree_root, const K& a_key)
{
	Node* tmp;
	bool right = false;

	if(subtree_root == NULL)
		return subtree_root;

	if (a_key > subtree_root->key)
		subtree_root->right = remove(subtree_root->right, a_key);
	else if (a_key < subtree_root->key)
		subtree_root->left = remove(subtree_root->left, a_key);
	else if (a_key == subtree_root->key)
	{
		if ((subtree_root->right == NULL) || (subtree_root->left == NULL))
		{
			if (subtree_root->left == NULL)
			{
				tmp = subtree_root->right;
				right = true;
			}
			else
				tmp = subtree_root->left;
			if (tmp == NULL)
			{
				subtree_root = NULL;
			}
			else
			{		
				subtree_root->key = tmp->key;
				subtree_root->value = tmp->value;
				if (right)
					subtree_root->right = NULL;
				else
					subtree_root->left = NULL;
			}
		}
		else
		{
			tmp = subtree_root->right;
			while (tmp->left != NULL)
				tmp = tmp->left;
			subtree_root->key = tmp->key;
			subtree_root->value = tmp->value;
			subtree_root->right = remove(subtree_root->right, tmp->key);
		}
	}
	if (subtree_root)
	{
	if (subtree_root->right != NULL || subtree_root->left != NULL)
	{
		if (subtree_root->right == NULL)
			subtree_root->height = 1 + subtree_root->left->height;
		else if (subtree_root->left == NULL)
			subtree_root->height = 1 + subtree_root->right->height;
		else
		{
			if (subtree_root->right->height > subtree_root->left->height)
				subtree_root->height = 1 + subtree_root->right->height;
			else
				subtree_root->height = 1 + subtree_root->left->height;
		}
	}
	else
		subtree_root->height = 1;
	}
	return rebalance(subtree_root);
}
template<typename K, typename V>
void AVLCollection<K,V>::copy(Node* lhs_subtree_root, const Node* rhs_subtree_root)
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
	 if (rhs_subtree_root->right == NULL)
		 lhs_subtree_root->right = NULL;
	 copy(lhs_subtree_root->left, rhs_subtree_root->left);
	 if (rhs_subtree_root->left == NULL)
		 lhs_subtree_root->left == NULL;
	}
}
template<typename K, typename V>
void AVLCollection<K,V>::make_empty(Node* subtree_root)
{
	if(subtree_root == NULL)
		return;
	make_empty(subtree_root->right);
	make_empty(subtree_root->left);
	delete subtree_root;
}
template<typename K, typename V>
void AVLCollection<K,V>::print_tree(std::string indent, Node* subtree_root)
{
	if (!subtree_root)
		return;
	std::cout << indent << subtree_root->key << " (h=" << subtree_root->height << ")" << std::endl;
	print_tree(indent + "  ", subtree_root->left);
	print_tree(indent + "  ", subtree_root->right);
}
template<typename K, typename V>
size_t AVLCollection<K,V>::height(const Node* subtree_root) const
{
	//no need
}
template<typename K, typename V>
typename AVLCollection<K,V>::Node* AVLCollection<K,V>::rotate_right(Node* k2)
{
	//std::cout << "rotate_right\n";
	Node* k1 = k2->left;
	//std::cout << "k1 = " << k1->key << std::endl;
	//std::cout << "k1 left = " << k1->left->key << std::endl;
	
	k2->left = k1->right;
	k1->right = k2;
	if (k2 != NULL && k1 != NULL)
	{
	if (k2->left == NULL && k2->right == NULL)
		k2->height = 1;
	else
	{
	 if (k2->left != NULL)
	 {
		if (k2->right != NULL)
		{
		 if (k2->left->height > k2->right->height)
		 	k2->height = 1 + k2->left->height;
		 else
			k2->height = 1 + k2->right->height;
		}
		else
			k2->height = k2->left->height;
	 }
	 else
		 k2->height = 1 + k2->right->height;
	}
	if (k1->left == NULL && k1->right == NULL)
		k1->height = 1;
	else
	{
	 if (k1->left != NULL)
	 {
	  if (k1->right != NULL)
	  {
	   if (k1->left->height > k1->right->height)
		k1->height = 1 + k1->left->height;
	   else
		k1->height = 1 + k1->right->height;
	  }
	  else
		k1->height = 1 + k1->left->height;
	 }
	 else
		k1->height = 1 + k1->right->height;
	 }
	}
	return k1;
}
template<typename K, typename V>
typename AVLCollection<K,V>::Node* AVLCollection<K,V>::rotate_left(Node* k2)
{
	//std::cout << "rotate_left\n";
	Node* k1 = k2->right;
	k2->right = k1->left;
	k1->left = k2;
	if (k1 != NULL && k2 != NULL)
	{
	if (k2->left == NULL && k2->right == NULL)
		k2->height = 1;
	else
	{
	 //std::cout << "got here\n";
	 if (k2->left != NULL)
	 {
		if (k2->right != NULL)
		{
		 if (k2->left->height > k2->right->height)
		 	k2->height = 1 + k2->left->height;
		 else
			k2->height = 1 + k2->right->height;
		}
		else
			k2->height = k2->left->height;
	 }
	 else
		 k2->height = 1 + k2->right->height;
	}
	if (k1->left == NULL && k1->right == NULL)
		k1->height = 1;
	else
	{
	 if (k1->left != NULL)
	 {
	  if (k1->right != NULL)
	  {
	   if (k1->left->height > k1->right->height)
		k1->height = 1 + k1->left->height;
	   else
		k1->height = 1 + k1->right->height;
	  }
	  else
		k1->height = 1 + k1->left->height;
	 }
	 else
		k1->height = 1 + k1->right->height;
	 }
	}
	return k1;
}
template<typename K, typename V>
typename AVLCollection<K,V>::Node* AVLCollection<K,V>::rebalance(Node* subtree_root)
{
	
	if (subtree_root == NULL)
		return subtree_root;
	//std::cout << "before: \n";
	//print_tree("  ",subtree_root);
//	std::cout << "before rebalance height: " << subtree_root->height << std::endl;
	Node* lptr = subtree_root->left;
	Node* rptr = subtree_root->right;	     
	int bal_factor = balance_factor(subtree_root);
	
	if (lptr && !rptr && lptr->height >1)
	{
		//std::cout << "in\n";
		if(bal_factor == 2 && balance_factor(lptr) == -1)
		{
			subtree_root->left = rotate_left(subtree_root->left);
		}
		subtree_root = rotate_right(subtree_root);			  
       	}
	
	else if (rptr && !lptr && rptr->height > 1)
	{
		if(bal_factor == -2 && balance_factor(rptr) == 1)
		{
			subtree_root->right = rotate_right(subtree_root->right);
		}
		subtree_root = rotate_left(subtree_root); 
		//subtree_root->height;
	}
	else if (bal_factor < -1)
	{
		//std::cout << "here too\n";
		if (balance_factor(rptr) > 0)
		{
			subtree_root->right =rotate_right(subtree_root->right);
			subtree_root =rotate_left(subtree_root);
		}
		else
			subtree_root = rotate_left(subtree_root);
		//subtree_root->height = 1 + subtree_root->left->height;
	}
	else if (bal_factor > 1)
	{
		if (balance_factor(lptr) > 0)
			subtree_root = rotate_right(subtree_root);
		else
		{
			subtree_root->left = rotate_left(subtree_root->left);
			subtree_root = rotate_right(subtree_root);
		}
		//subtree_root->height = 1 + subtree_root->left->height;
	}
	//std::cout << "rebalanced height: " << subtree_root->height << std::endl;
	//std::cout << "after:\n";
	return subtree_root;
}

#endif
