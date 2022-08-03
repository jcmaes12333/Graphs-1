//JC Maes
//file:BinSearchCollection.h
// 10/22/2020

#ifndef BINSEARCHCOLLECTIONH
#define BINSEARCHCOLLECTIONH
#include "array_list_collection.h"

template<typename K, typename V>
class BinSearchCollection : public ArrayListCollection<K,V>
{
public:
void add(const K& A_key, const V& A_val);
void remove(const K& A_key);
bool find(const K& search_key, V& the_val) const;
void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
void sort(ArrayList<K>& all_keys_sorted) const;
size_t size() const;
private:
	ArrayList<std::pair<K,V>> kv_list;

	bool binsearch(const K& key, int& index) const;
};

template<typename K, typename V>
bool BinSearchCollection<K,V>::binsearch(const K& key, int& index) const
{
	if (kv_list.size() == 0)
	{
		index = 0;
		return false;
	}
	int start = 0;
	int end = kv_list.size()-1;
	int mid = 0;
	std::pair<K,V> tmp;
	while (start <= end)
	{
		mid = (start + (end))/2;
		kv_list.get(mid,tmp);
		if(tmp.first == key)
		{
			index = mid;
			return true;
		}
		
		if (tmp.first < key)
		{
			start = mid+1;
		}
	        else
		{	
			end = mid-1;
		}	
	}
	index = start;
	if (index > kv_list.size())
		index = kv_list.size();
	return false;
}
template<typename K, typename V>
void BinSearchCollection<K,V>::add(const K& A_key, const V& A_val)
{
	int index;
	if(!binsearch(A_key, index))
		kv_list.add(index, std::pair<K,V>(A_key, A_val));
	
}
template<typename K, typename V>
void BinSearchCollection<K,V>::remove(const K& A_key)
{
	int index;
	if(binsearch(A_key,index))
		kv_list.remove(index);
}
template<typename K, typename V>
bool BinSearchCollection<K,V>::find(const K& search_key, V& the_val) const
{
	int index;
	std::pair<K,V> tmp;
	if(binsearch(search_key,index))
	{
		kv_list.get(index,tmp);
		the_val = tmp.second;
		return true;
	}
	return false;
}
template<typename K, typename V>
void BinSearchCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
	if (kv_list.size() != 0){
	int index1,index2;
	//std::cout<< k1 << " "  << k2 << std::endl;
	binsearch(k1,index1);
	binsearch(k2,index2);
	//std::cout << index1 << " " << index2 << std::endl;
	if (index2 > kv_list.size()-1)
		index2 = kv_list.size()-1;
	for (int i = index1; i <= index2; ++i)
	{
		std::pair<K,V> tmp;
		kv_list.get(i,tmp);
		keys.add(tmp.first);
	}
	}
}

template<typename K, typename V>
void BinSearchCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
	
	for (int i = 0; i < kv_list.size(); ++i){
		std::pair<K,V> tmp;
		kv_list.get(i,tmp);
		all_keys_sorted.add(tmp.first);
	}
	return;
}

template<typename K, typename V>
size_t BinSearchCollection<K,V>::size() const
{
	return kv_list.size();
}

#endif
