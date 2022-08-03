#ifndef HASHTABLECOLLECTIONH
#define HASHTABLECOLLECTIONH

#include "collection.h"

template<typename K, typename V>
class HashTableCollection : public Collection<K,V>
{
public:
HashTableCollection();
HashTableCollection(const HashTableCollection<K,V>& rhs);
~HashTableCollection();
HashTableCollection& operator=(const HashTableCollection<K,V>& rhs);
void add(const K& A_key, const V& A_val);
void remove(const K& A_key);
bool find(const K& search_key, V& the_val) const;
void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
void keys(ArrayList<K>& all_keys) const;
void sort(ArrayList<K>& all_keys_sorted) const;
size_t size() const;
size_t min_chain_length();
size_t max_chain_length();
double avg_chain_length();
private:
struct Node {
 K key;
 V value;
 Node* next;
};

Node** hash_table;
size_t length;
size_t table_capacity;
double load_factor_threshold = 0.75;
void resize_and_rehash();
};

template<typename K, typename V>
HashTableCollection<K,V>::HashTableCollection()
{	
	length = 0;
	table_capacity = 16;
	hash_table = new Node*[table_capacity];
	for (int i = 0; i < table_capacity; i++)
		hash_table[i] = NULL;
}
template<typename K, typename V>
HashTableCollection<K,V>::HashTableCollection(const HashTableCollection<K,V>& rhs)
{
	table_capacity = 16;	
	*this = rhs;
}
template<typename K, typename V>
HashTableCollection<K,V>::~HashTableCollection()
{
	/*for (int i = 0; i < table_capacity; i++)
	{
		Node* tmp = hash_table[i];
		if(tmp != NULL)
		{
		 while (tmp->next != NULL)
		 {
		  Node* tmp2 = tmp->next;
		  tmp->next = tmp2->next;
		  delete tmp2;
		 }
		 delete tmp;
		}
	}
	delete hash_table;*/
}
template<typename K, typename V>
HashTableCollection<K,V>& HashTableCollection<K,V>:: operator=(const HashTableCollection<K,V>& rhs)
{
	if (this != &rhs)
	{
	 length = 0;
	 ArrayList<K> all_keys;
	 rhs.keys(all_keys);
 	 hash_table = new Node*[table_capacity];
	for (int i = 0; i < table_capacity; i++)
		hash_table[i] = NULL;
	 for (int i = 0; i < rhs.size(); i++)
	 {
		V tmpV;
		K tmpK;
	       	all_keys.get(i,tmpK);
		rhs.find(tmpK,tmpV);
		add(tmpK,tmpV);
	 }
	}
	return *this;	
}
template<typename K, typename V>
void HashTableCollection<K,V>::add(const K& A_key, const V& A_val)
{
	length++;
	double load_factor = (double)length/(double)table_capacity;
	if (load_factor > load_factor_threshold)
	{
		resize_and_rehash();
	}
	std::hash<K> hash_fun;
	int hash_index = hash_fun(A_key) % table_capacity;
	if (hash_table[hash_index] == NULL)
	{
		hash_table[hash_index] = new Node;
		hash_table[hash_index]->key = A_key;
		hash_table[hash_index]->value = A_val;
		hash_table[hash_index]->next = NULL;
	}
	else
	{
		Node* tmp = new Node;
		tmp->key = A_key;
		tmp->value = A_val;
		tmp->next = hash_table[hash_index];
		hash_table[hash_index] = tmp;
	}
}

template<typename K, typename V>
void HashTableCollection<K,V>::remove(const K& A_key)
{
	std::hash<K> hash_fun;
	int hash_index = hash_fun(A_key) % table_capacity;
	Node* tmp = hash_table[hash_index];
	while(tmp != NULL)
	{
		if (tmp->key == A_key)
		{
			delete tmp;
			length--;
			return;
		}
		tmp = tmp->next;
	}
}
template<typename K, typename V>
bool HashTableCollection<K,V>::find(const K& search_key, V& the_val) const
{
	std::hash<K> hash_fun;
	int hash_index = hash_fun(search_key) % table_capacity;
	Node* tmp = hash_table[hash_index];
	if (tmp == NULL)
		return false;
	if(tmp != NULL)
	{
	 while (tmp != NULL)
	 {
 	  if(tmp->key == search_key)
	  {
		  the_val = tmp->value;
		  return true;
	  }
	  tmp = tmp->next;
	 }
	}
	return false;

}
template<typename K, typename V>
void HashTableCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
	for(int i = 0; i < table_capacity; i++)
	{
		Node* tmp = hash_table[i];
		while(tmp != NULL)
		{
			if (tmp->key >= k1 && tmp->key <= k2)
			{
				keys.add(tmp->key);
			}
			tmp = tmp->next;
		}
	}
}
template<typename K, typename V>
void HashTableCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
	keys(all_keys_sorted);
	all_keys_sorted.sort();
}
template<typename K, typename V>
size_t HashTableCollection<K,V>::size() const
{
	return length;
}
template<typename K, typename V>
void HashTableCollection<K,V>::resize_and_rehash()
{
	int new_table_capacity = table_capacity * 2;
	std::hash<K> hash_fun;
	ArrayList<K> all_keys;
	//std::cout << "table_capacity is: " << new_table_capacity << std::endl;
	/*if (hash_table != NULL)
	{
		keys(all_keys);
	for (int i = 0; i < all_keys.size(); i++)
	{
		K tmp;
		all_keys.get(i, tmp);
		//std::cout << "key " << i << ": " << tmp << std::endl; 
	}
	}*/
	Node** new_hash_table = new Node*[new_table_capacity];
	for (int i = 0; i < new_table_capacity; i++)
	{
		new_hash_table[i] = NULL;
	}
	for (int i = 0; i < table_capacity; i++)
	{
		Node* tmp = hash_table[i];
		while(tmp != NULL)
		{
			//std::cout << "new key is: " << tmp->key << std::endl;
			//std::cout << "in\n";
			int new_index = hash_fun(tmp->key) % new_table_capacity;
			if(new_hash_table[new_index] == NULL)
			{
				//std::cout << "in\n";
				new_hash_table[new_index] = new Node;
				new_hash_table[new_index]->key = tmp->key;
				new_hash_table[new_index]->value = tmp->value;
				new_hash_table[new_index]->next = NULL;
				//std::cout << "out\n";	
			}
			else
			{
				//std::cout << "in2\n";
				Node* tmp2 = new Node;
				tmp2->key = tmp->key;
			       	tmp2->value = tmp->value;
				tmp2->next = new_hash_table[new_index];
				new_hash_table[new_index] = tmp2;
				//std::cout << "out2\n";
			}
			tmp = tmp->next;
			//std::cout << "out\n";
		}
	}
	for (int i = 0; i < table_capacity; i++)
	{
		Node* tmp = hash_table[i];
		if (tmp != NULL)
		{
		 while (tmp->next != NULL)
	 	 {
		  Node* tmp2 = tmp->next;
	       	  tmp->next = tmp2->next;
		  delete tmp2;
		 }
		 delete tmp;
		}
	}
	hash_table = new_hash_table;
	table_capacity = new_table_capacity;
	//std::cout << "out\n";
}
template<typename K, typename V>
size_t HashTableCollection<K,V>::min_chain_length()
{
	
}
template<typename K, typename V>
size_t HashTableCollection<K,V>::max_chain_length()
{
	
}
template<typename K, typename V>
double HashTableCollection<K,V>::avg_chain_length()
{
	
}
template<typename K, typename V>
void HashTableCollection<K,V>::keys(ArrayList<K>& all_keys) const
{
	for(int i = 0; i < table_capacity; i++)
	{
		Node* tmp = hash_table[i];
		while(tmp != NULL)
		{
		 all_keys.add(tmp->key);
		 tmp = tmp->next;
		}
	}
}
#endif
