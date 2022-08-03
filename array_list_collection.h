//name:JC Maes
//file:arraylistcollection.h
//date: 10/14/2020

#ifndef ARRAYLISTCOLLECTIONH
#define ARRAYLISTCOLLECTIONH

#include "collection.h"

template<typename K, typename V>
class ArrayListCollection : public Collection<K,V>
{
public:

void add(const K& A_key, const V& A_val);
void remove(const K& A_key);
bool find(const K& search_key, V& the_val) const;
void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
void keys(ArrayList<K>& all_keys) const;
void sort(ArrayList<K>& all_keys_sorted) const;
size_t size() const;

private:
	ArrayList<std::pair<K,V>> kv_list;
};

template<typename K, typename V>
void ArrayListCollection<K,V>::add(const K& A_key, const V& A_val)
{
	kv_list.add(std::pair<K,V>(A_key, A_val));
}

template<typename K, typename V>
void ArrayListCollection<K,V>::remove(const K& A_key)
{
	for (int i = 0; i < kv_list.size(); i++)
	{
		std::pair<K,V> tmp;
		kv_list.get(i,tmp);

		if (tmp.first == A_key)
			kv_list.remove(i);
	}	
}	


template<typename K, typename V>
bool ArrayListCollection<K,V>::find(const K& search_key, V& the_val) const 
{
	for (int i = 0; i < kv_list.size(); i++)
	{
		std::pair<K,V> tmp;
		kv_list.get(i,tmp);
		if (tmp.first == search_key)
		{
			the_val = tmp.second;
			return true;
		}
	}
	return false;
}

template<typename K, typename V>
void ArrayListCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
	for (int i = 0; i < kv_list.size(); i++)
	{
		std::pair<K,V> tmp;
		kv_list.get(i,tmp);
		if (tmp.first >= k1 && tmp.first <= k2)
		{
			keys.add(tmp.first);
		}
	}
}
template<typename K, typename V>
void ArrayListCollection<K,V>::keys(ArrayList<K>& all_keys) const
{
	for (int i = 0; i < kv_list.size(); i++)
	{
		std::pair<K,V> tmp;
		kv_list.get(i,tmp);
		all_keys.add(tmp.first);
	}
}

template<typename K, typename V>
void ArrayListCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
	keys(all_keys_sorted);
	all_keys_sorted.sort();
}

template<typename K, typename V>
size_t ArrayListCollection<K,V>::size() const
{
	return kv_list.size();
}


#endif
