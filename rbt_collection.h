//----------------------------------------------------------------------
// Name: JC Maes 
// File: rbt_collection.h
// Date: Fall, 2020
// Desc: A Key-Value Collection implementation using a Red-Black tree.
//----------------------------------------------------------------------


#ifndef RBT_COLLECTION_H
#define RBT_COLLECTION_H


#include "string.h"
#include "collection.h"
#include "array_list.h"


template<typename K, typename V>
class RBTCollection : public Collection<K,V>
{
public:

  // create an empty collection
  RBTCollection();
  
  // copy constructor
  RBTCollection(const RBTCollection<K,V>& rhs);

  // assignment operator
  RBTCollection<K,V>& operator=(const RBTCollection<K,V>& rhs);

  // delete collection
  ~RBTCollection();
  
  // add a new key-value pair into the collection 
  void add(const K& a_key, const V& a_val);

  // remove a key-value pair from the collectiona
  void remove(const K& a_key);

  // find and return the value associated with the key
  bool find(const K& search_key, V& the_val) const;

  // find and return each key >= k1 and <= k2 
  void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
  
  // return all of the keys in the collection 
  void keys(ArrayList<K>& all_keys) const;

  // return all of the keys in ascending (sorted) order
  void sort(ArrayList<K>& all_keys_sorted) const;

  // return the number of key-value pairs in the collection
  size_t size() const;

  // return the height of the tree
  size_t height() const;

  // for testing:

  // check if tree satisfies the red-black tree constraints
  bool valid_rbt() const;
  
  // pretty-print the red-black tree (with heights)
  void print() const;

  
private:
  
  // RBT node structure
  enum color_t {RED, BLACK};
  struct Node {
    K key;
    V value;
    Node* left;
    Node* right;
    Node* parent;
    color_t color;
  };

  // root node
  Node* root;

  // number of k-v pairs stored in the collection
  size_t node_count;

  // helper to empty entire hash table
  void make_empty(Node* subtree_root);

  // copy helper
  void copy(Node* lhs_subtree_root, const Node* rhs_subtree_root); 
    
  // helper to recursively find range of keys
  void find(const Node* subtree_root, const K& k1, const K& k2,
            ArrayList<K>& keys) const;

  // helper to build sorted list of keys (used by keys and sort)
  void keys(const Node* subtree_root, ArrayList<K>& all_keys) const;

  // rotate right helper
  void rotate_right(Node* k2);

  // rotate left helper
  void rotate_left(Node* k2);

  // restore red-black constraints in add
  void add_rebalance(Node* x);

  // restore red-black constraints in remove
  void remove_rebalance(Node* x, bool going_right);
  
  // height helper
  size_t height(Node* subtree_root) const;
  
  // ------------
  // for testing:
  // ------------
  
  // validate helper
  bool valid_rbt(Node* subtree_root) const;

  // validate helper
  size_t black_node_height(Node* subtree_root) const;
  
  // recursive pretty-print helper
  void print_tree(std::string indent, Node* subtree_root) const;
};



 // create an empty collection
template<typename K, typename V>
RBTCollection<K,V>::RBTCollection()
{
  root = NULL;
  node_count = 0;
}
  
  // copy constructor
template<typename K, typename V>
RBTCollection<K,V>::RBTCollection(const RBTCollection<K,V>& rhs)
{
  node_count = 0;
  *this = rhs;
}
template<typename K, typename V>
RBTCollection<K,V>&  RBTCollection<K,V>::operator=(const RBTCollection<K,V>& rhs)
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
RBTCollection<K,V>::~RBTCollection()
{
  make_empty(root);
}  
  // add a new key-value pair into the collection 
template<typename K, typename V>
void RBTCollection<K,V>::add(const K& a_key, const V& a_val)
{
  node_count++;
  Node* tmpN = new Node;//New node to be inserted in RBT
  tmpN->key = a_key;
  tmpN->value = a_val;
  tmpN->color = RED;
  tmpN->left = NULL;
  tmpN->right = NULL;
  Node* tmpX = root;//After traversing the list, will be the null location for tmpN to be inserted
  Node* tmpP = NULL;//X's parent
  while (tmpX != NULL)
  {
    add_rebalance(tmpX);//rebalance tree along the way
    tmpP = tmpX;
    if (a_key < tmpX->key)
      tmpX = tmpX->left;
    else 
      tmpX = tmpX->right;//traversing the RBT
  }
  tmpN->parent = tmpP;//setting new nodes parent
  if (tmpP == NULL)
    root = tmpN;//root case
  else if (a_key < tmpP->key)
    tmpP->left = tmpN;
  else
    tmpP->right = tmpN;//making parent point to new node.
  add_rebalance(tmpN);//rebalance again now that element has been added
  root->color = BLACK;//root should always be black
}
  // remove a key-value pair from the collectiona
template<typename K, typename V>
void RBTCollection<K,V>::remove(const K& a_key)
{
  Node* sentinel = new Node;//keeps track of root
  sentinel->right = root;
  sentinel->color = RED;
  Node* x = root;//current node
  Node* p = sentinel;//current parent
  bool found = false;
  while (x != NULL && found == false)
  {
    if (a_key < x->key)
    {
      remove_rebalance(x, false);//call rebalance in each chase
      x = x->left;//traverse
    }
    else if (a_key > x->key)
    {
      remove_rebalance(x, true);
      x = x->right;
    }
    else
    {
      remove_rebalance(x, x->left);
      found = true;
    }
    p = x->parent;
    sentinel->right = root;
  }
  if (!found)
    return;//key is not in tree
  if (x->left == NULL || x->right == NULL)
  {
    //delete simple case
    Node* tmp;
    if (x->left == NULL)
      tmp = x->right;
    else
      tmp = x->left;//get replacement
    if (tmp == NULL)
    {
      //no child case
      if (p != NULL)
      {
        if (p->right == x)
	  p->right = tmp;
	else
	  p->left = tmp;
      }
      if (x == root)
      {
	      root = NULL;
	      sentinel->right = root;
      }
      x = NULL;//delete x	
    }
    else
    {	//one child case
      if (root == x)
      {
	      //root case
        root = tmp;
	sentinel->right = root;
      }
      x->key = tmp->key;//x takes successors key, value
      x->value = tmp->value;
      if (x->right == tmp)
	x->right = tmp->right;
      else
        x->left = tmp->left;//x points to tmps successor, works for null to
      tmp = NULL;//delete tmp (successor)
    }
  }
  else
  {
    //case with two children
    Node* s = x->right;
    remove_rebalance(s, false);
    while (s->left)
    {
      s = s->left;
      remove_rebalance(s, false);
    }//find successor
    sentinel->right = root;//make sure root is in tact
    x->key = s->key;
    x->value = s->value;//copy values
    if (x->right == s)
    { 
      x->right = s->right;
      if (x->right != NULL)
	      x->right->color = s->color;
    }
    if (s->parent->left == s)
    {
			//std::cout << s->parent->key << std::endl;
      Node* tmp2 = s->parent;
      tmp2->left = s->left;
    }
    else
    {
			//std::cout << s->parent->key << std::endl;
      Node* tmp2 = s->parent;
      tmp2->right = s->right; 
    }
    s = NULL;
    //print();
    if (x->color == RED)
    {
	    if (x->left != NULL)				       x->left->color = BLACK;
	    if (x->right != NULL)
	      x->right->color = BLACK;//fixes violation
    }
  }
  root = sentinel->right;
  if (root != NULL)
    root->color = BLACK;
  delete sentinel;
  node_count--;
}

  // find and return the value associated with the key
template<typename K,typename V>
bool RBTCollection<K,V>::find(const K& search_key, V& the_val) const
{
  Node* tmp = root;//simple find value used in previous assignments
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

  // find and return each key >= k1 and <= k2 
template<typename K,typename V>
void RBTCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
  find(root, k1, k2, keys);//call to private find-range helper
}
  
  // return all of the keys in the collection 
template<typename K,typename V>
void RBTCollection<K,V>::keys(ArrayList<K>& all_keys) const
{
  keys(root, all_keys);//call to private keys helper
}

  // return all of the keys in ascending (sorted) order
template<typename K,typename V>
void RBTCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
  keys(root, all_keys_sorted);//tree is already sorted, we just have to grab the keys in order
}

  // return the number of key-value pairs in the collection
template<typename K,typename V>
size_t RBTCollection<K,V>::size() const
{
  return node_count;//kept track of throughout collection
}

  // return the height of the tree
template<typename K, typename V>
size_t RBTCollection<K,V>::height() const
{
  height(root);
}

  // for testing:

  
template<typename K, typename V>
void RBTCollection<K,V>::make_empty(Node* subtree_root)
{
  if(subtree_root == NULL)
    return;
  make_empty(subtree_root->right);//simple make_empty
  make_empty(subtree_root->left);
  delete subtree_root;
}

 
  // copy helper
template<typename K, typename V>
void RBTCollection<K,V>::copy(Node* lhs_subtree_root, const Node* rhs_subtree_root)
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
      lhs_subtree_root = root;//null case
    }
    lhs_subtree_root->key = rhs_subtree_root->key;
    lhs_subtree_root->value = rhs_subtree_root->value;
    lhs_subtree_root->color = rhs_subtree_root->color;
    lhs_subtree_root->parent = rhs_subtree_root->parent;//copy all elements of rhs
    lhs_subtree_root->left = new Node;
    lhs_subtree_root->right = new Node;//make new nodes to call copy recursively
    copy(lhs_subtree_root->right, rhs_subtree_root->right);
    if (rhs_subtree_root->right == NULL)
      lhs_subtree_root->right = NULL;
    copy(lhs_subtree_root->left, rhs_subtree_root->left);
    if (rhs_subtree_root->left == NULL)
      lhs_subtree_root->left = NULL;
  }
}
    
  // helper to recursively find range of keys
template<typename K, typename V>
void RBTCollection<K,V>::find(const Node* subtree_root, const K& k1, const K& k2,ArrayList<K>& keys) const
{
  if (subtree_root == NULL)
    return;
  find(subtree_root->left, k1, k2, keys);
  if (subtree_root->key >= k1 && subtree_root->key <= k2)
    keys.add(subtree_root->key);
  find(subtree_root->right, k1, k2, keys);//recursively find keys in sorted order
}

  // helper to build sorted list of keys (used by keys and sort)
template<typename K, typename V>
void RBTCollection<K,V>::keys(const Node* subtree_root, ArrayList<K>& all_keys) const
{
  if (subtree_root == NULL)
    return;
  //all_keys.add(subtree_root->key);
  keys(subtree_root->left, all_keys);
  all_keys.add(subtree_root->key);
  keys(subtree_root->right, all_keys);//recursively find keys in sorted order
}

template<typename K, typename V>
void RBTCollection<K,V>::rotate_right(Node* k2){	
  Node* k1 = k2->left;//right rotation
  k2->left = k1->right;
  if (k2->left != NULL)
    k2->left->parent = k2;
  k1->parent = k2->parent;
  if (k1->parent != NULL)
  {
    if (k1->key > k1->parent->key)
      k1->parent->right = k1;
    else
      k1->parent->left = k1;
    if (k1->parent->right == k2)
      k1->parent->right = NULL;
    else if (k1->parent->left == k2)
      k1->parent->left = NULL;
  }//keeps track of parents
  k1->right = k2;
  k2->parent = k1;
  if (root == k2)
  {
    root = k1;
    root->color = BLACK;
  }//keeps track of root
}

  // rotate left helper
template<typename K, typename V>
void RBTCollection<K,V>::rotate_left(Node* k2)
{
	//left rotation
  Node* k1 = k2->right;
  k2->right = k1->left;
  if (k2->right != NULL)
    k2->right->parent = k2;
  k1->parent = k2->parent;
  if (k1->parent != NULL)
  {
    if (k1->key > k1->parent->key)
      k1->parent->right = k1;
    else
      k1->parent->left = k1;
    if (k1->parent->right == k2)
      k1->parent->right = NULL;
    else if (k1->parent->left == k2)
      k1->parent->left = NULL;
  }//keeps track of parent
  k1->left = k2;
  k2->parent = k1;
  if (root == k2)
  {
    root = k1;
    root->color = BLACK;
  }//keeps track of root
}

  // restore red-black constraints in add
template<typename K, typename V>
void RBTCollection<K,V>::add_rebalance(Node* x){
  if (x->left != NULL && x->right != NULL)
  {
    if (x->color == BLACK && x->left->color == RED && x->right->color == RED)
    {
      if (x != root)
	      x->color = RED;
      x->left->color = BLACK;
      x->right->color = BLACK;//color flip
    }
  }
  Node* p = x->parent;
  Node* g = NULL;
  if (p != NULL)
  {
    g = p->parent;//grandparent
    if (g != NULL)
    {
      if (x->color == RED && p->color == RED)
      {
	//x and parent can not both be red
	if (p == g->right)
	{
          if(x == p->left)
	  {//right-left case
	    rotate_right(p);
	    x = p;
	    p = x->parent;
	  }
	  //right-right case
	  rotate_left(g);
	  if (root == g)
  	    root = p;
	  p->color = BLACK;
	  if (p->left != NULL)
 	    p->left->color = RED;
	}
	else
	{
	  if (x == p->right)
	  {
  	    //left-right case
	    rotate_left(p);
	    x = p;
	    p = x->parent;
	  }
	  //left-left case
	  rotate_right(g);
	  if (root == g)
	    root = p;
  	  p->color = BLACK;
	  if (p->right != NULL)
  	    p->right->color = RED;
	}
      }
    }
  }
}

  // restore red-black constraints in remove
template<typename K, typename V>
void RBTCollection<K,V>::remove_rebalance(Node* x, bool going_right)
{
  if (x->color == RED)//return if the node we're removing is red
  {
   return;
  }
  bool b1,b2,b3;//conditionals required various checks, so I figured I'd figure out these conditions beforehand so I could still follow an if > else-if > else-if structure
  b1 = b2 = b3 = false;
  Node* p = x->parent;
  Node* t = NULL;
  if (x->right != NULL || x->left != NULL)
  {
    if (x->right == NULL)
    {
      if (x->left->color == RED)
        b1 = true;
    }
    else if (x->left == NULL)
    {
      if (x->right->color == RED)
        b1 = true;
    }
    else
    {
      if (x->left->color == RED || x->right->color == RED)
      b1 = true;
    }
  }
  if (b1)//if x has a red child
  {
    if (going_right && x->left != NULL)
    {
      if (x->right != NULL)
      {
        if (x->left->color == RED && x->right->color == BLACK)
        {
          x->color = RED;//adjusting colors
          x->left->color = BLACK;
          rotate_right(x);
          return;
        }
      }
    }
    else if(!going_right && x->right != NULL)
    {
      if (x->left != NULL)
      {
        if (x->right->color == RED && x->left->color == BLACK)
        {
          x->color = RED;
          x->right->color = BLACK;
          rotate_left(x);
          return;
        }
      }
    }
  }
  else if (p != NULL)//No need to check other conditionals if b1 is true
  {
    if (p->right == x)
      t = p->left;
    else
      t = p->right;//sibling
    if (t != NULL)
    {
      if ((t->left == NULL && t->right == NULL)||(t->left != NULL && t->right != NULL))
      {
        if (t->left != NULL)
        {
          if (t->left->color == BLACK && t->right->color == BLACK)
          {
            b2 = true;//sibling has 2 black children
          }	
        }
        else
        {
          b2 = true;//or sibling is null
        }
      }	
      if (t ->left != NULL || t->right != NULL)
      {
        if (t->left != NULL)
        {
           if (t->left->color == RED)
             b3 = true;//if sibling has a red child
        }
        if (t->right != NULL)
        {
        if (t->right->color == RED)
        b3 = true;//if sibling has a red child
        }
       }
       if (b2)//if sibling has 2 black children or 2 null children, only checked if the first if is passed over
       {
         x->color = RED;
         t->color = RED;
         p->color = BLACK;
         return;
       }
       else if(b3)//sibling has a red child
       {
         if (t->left != NULL)
         {
           if (t == p->right && t->left->color == RED)
           {
             //if it is a right child and left is red
             rotate_right(t);
             rotate_left(p);
             return;
           }
         }
         if (t->right != NULL)
         {
           if (t == p->left && t->right->color == RED)
           {
             rotate_left(t);//if it is a left child, and its right child is red
             rotate_right(p);
             return;
           }
         }
    	 if (t == p->right)//t is a right child, but its left child is not red
    	 {	
     	   t->color = RED;// need to fix color for single rotation
     	   t->right->color = BLACK;
     	   rotate_left(p);
    	 }
    	 else//t is a left child, but its right child is not red
    	 {
     	   t->color = RED;//need to fix color for single rotation
     	   t->left->color = BLACK;
           rotate_right(p);
    	 }
    	 p->color = BLACK;
    	 x->color = RED;
       }
    }
  }
}

template<typename K, typename V>
size_t RBTCollection<K,V>::height(Node* subtree_root) const
{
 if (subtree_root == NULL)
  return 0;
 int l = height(subtree_root->left);
 int r = height(subtree_root->right);//recursive height function used in previous assignments
 if (r > l)
  return r+1;
 else
  return l+1;
}

 

//----------------------------------------------------------------------
// Provided Helper Functions:
//----------------------------------------------------------------------

template<typename K, typename V>
bool RBTCollection<K,V>::valid_rbt() const
{
  return !root or (root->color == BLACK and valid_rbt(root));
}


template<typename K, typename V>
bool RBTCollection<K,V>::valid_rbt(Node* subtree_root) const
{
  if (!subtree_root)
    return true;
  //std::cout << "in\n";
  color_t rc = subtree_root->color;
  color_t lcc = subtree_root->left ? subtree_root->left->color : BLACK;
  color_t rcc = subtree_root->right ? subtree_root->right->color : BLACK;
  //std::cout << "weird\n";  
  size_t lbh = black_node_height(subtree_root->left);
  size_t rbh = black_node_height(subtree_root->right);
  //std::cout << "HEREAHAHSDHASHD\n";
  bool lv = valid_rbt(subtree_root->left);
  bool rv = valid_rbt(subtree_root->right);
  // check equal black node heights, no two consecutive red nodes, and
  // left and right are valid RBTs
  return (lbh == rbh) and (rc != RED or (lcc != RED and rcc != RED)) and lv and rv;
}


template<typename K, typename V>
size_t RBTCollection<K,V>::black_node_height(Node* subtree_root) const
{
  if (!subtree_root)
    return 1;
  //std::cout << "here\n";
  size_t hl = black_node_height(subtree_root->left);
  size_t hr = black_node_height(subtree_root->right);
  size_t h = hl > hr ? hl : hr;
  if (subtree_root->color == BLACK)
	  return 1 + h;
  else
	  return h;
}

template<typename K, typename V>
void RBTCollection<K,V>::print() const
{
	print_tree("",root);
}

template<typename K, typename V>
void RBTCollection<K,V>::print_tree(std::string indent, Node* subtree_root) const
{
	if(!subtree_root)
		return;
	std::string color = "[BLACK]";
	if (subtree_root->color == RED)
		color = "[RED]";
	std::cout << indent << subtree_root->key << " " << color << " (h=" << height(subtree_root) << ")" << std::endl;
	print_tree(indent + "  ", subtree_root->left);
	print_tree(indent + "  ", subtree_root->right);
}
#endif
