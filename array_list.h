//----------------------------------------------------------------------
// FILE: array_list.h
// NAME: JC Maes
// DATE: 10/14/2020
// DESC: Implements a resizable array version of the list
//       class. Elements are added by default to the last available
//       index in the array. 
//----------------------------------------------------------------------

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include "list.h"

template<typename T>
class ArrayList : public List<T>
{
public:
  ArrayList();
  ArrayList(const ArrayList<T>& rhs);
  ~ArrayList();
  ArrayList& operator=(const ArrayList<T>& rhs);

  void add(const T& item);
  bool add(size_t index, const T& item);
  bool get(size_t index, T& return_item) const;
  bool set(size_t index, const T& new_item);
  bool remove(size_t index);
  size_t size() const;
  void printArray();
  void selection_sort();
  void insertion_sort();
  void merge_sort();
  void quick_sort();
  void sort();

private:
  T* items;
  size_t capacity;
  size_t length;

  // helper to resize items array
  void resize();
  void merge_sort(size_t start, size_t end);
  void quick_sort(size_t start, size_t end);
};


template<typename T>
ArrayList<T>::ArrayList()
  : capacity(10), length(0)
{
  items = new T[capacity];
}


template<typename T>
ArrayList<T>::ArrayList(const ArrayList<T>& rhs)
  : capacity(10), length(0), items(nullptr)
{
  
  *this = rhs;
}

template<typename T>
ArrayList<T>::~ArrayList(){ //destructor
	delete[] items;
}
template<typename T>
ArrayList<T>&  ArrayList<T>:: operator=(const ArrayList<T>& rhs){ //called when someone sets an array list = to another array list. Rather than just copying the location of the right hadn side array, it creates a new array list with the same values as the right hand array.
	if(&rhs == this)
		return *this;
	capacity = rhs.capacity;
	length = rhs.length;
	items = new T[capacity];
	/*for (int i = 0; i < 10; i++){
		items[i] = NULL;
	}*/
	for (int i = 0; i < length; i++){
		items[i] = rhs.items[i];
	}
	return *this;

}

template<typename T>
void ArrayList<T>:: add(const T& item){ //adds to the end of the array list
	if(length == capacity){
		resize();
	}
	
		items[length] = item;
		length++;
}

template<typename T>
bool ArrayList<T>:: add(size_t index, const T& item){//adds a new element at the given index
	bool isAdded = false;
	if(items[index] == item){
		return isAdded;
	}
	length++;
	if(length == capacity){
		resize();
	}
	if(length > 1){
	for (int i = length-1; i > index; i--){
		items[i] = items[i-1];
	}
	}
	
	items[index] = item;
	
	isAdded = true;
	

	return isAdded;

}

template<typename T>
bool ArrayList<T>::get(size_t index, T& return_item) const{ //returns value at given index into return_item
	if(index > length-1){
		return false;
	}

	return_item = items[index];
	return true;
}

template<typename T>
bool ArrayList<T>::set(size_t index, const T& new_item){ //sets the value of the given index in the array list
	
	if (index > length-1) {
		return false;
	}
	
	items[index] = new_item;
	return true;
}

template<typename T>
bool ArrayList<T>::remove(size_t index){// removes the item at the index of the current array list

	if (index > length-1) {
		return false;
	}
	if (length == 0) {
		return false;
	}


	for (int i = index; i< length-1;i++){
		items[i] = items[i+1];
	}
	length--;
	return true;
}

template<typename T>
size_t ArrayList<T>::size() const{//helper function that returns size
	return length;
}

template<typename T>
void ArrayList<T>::resize(){//doubles the capacity of an array when it reaches its previous capacity
	T* newArray = new T[capacity *2];
	for (int i = 0; i < length; i++) {
		newArray[i] = items[i];
	}
	delete[] items;
	items = newArray;
	capacity = capacity*2;
	

	
}

template<typename T>
void ArrayList<T>::printArray(){ //helper function to print the array
	for (int i = 0; i < length; ++i){
		std::cout << items[i] << std::endl;
	}

}

template<typename T>
void ArrayList<T>::selection_sort(){ //performs a selection sort on the current array list
	int unsortedLength = length;
	int largeIndex;
	T tmp;
	while (unsortedLength > 1){ //if unsortedLength = 1 or 0, there is nothing to sort
		largeIndex = 0;
		for (int i = 1; i < unsortedLength; i++){ //finds the largest item and stores in lardIndex
			if (items[largeIndex] < items[i])
				largeIndex = i;
		}
		tmp = items[largeIndex];//store value temporarily
		items[largeIndex] = items[unsortedLength-1]; //swap values
		items[unsortedLength-1] = tmp;
		--unsortedLength;
	
	}



}
template<typename T>
void ArrayList<T>::insertion_sort(){ //performs an insertion sort on the current array list
	int lastSorted = 1;//index of the last sorted value

	while (lastSorted != length && length != 0){
		for (int i = 0; i < lastSorted; ++i){
			if (items[lastSorted] < items[i]) //swaps values if less than
			{
				T tmp = items[lastSorted];
				items[lastSorted] = items[i];
				items[i] = tmp;
			}
		}
		++lastSorted;
	}


}

template <typename T>
void ArrayList<T>::merge_sort()
{
	if(length >= 1)
		merge_sort(0, length-1);//calling helper function
	return;

}
template <typename T>
void ArrayList<T>::merge_sort(size_t start, size_t end)
{
	//std::cout << "- - - - - - -" << std::endl;
	if (start < end) {
	//printArray();
	int mid = (start+end) / 2; //middle index
	merge_sort(start, mid);//recursive calls
	merge_sort(mid + 1, end);
	T tmp[(end - start) + 1];//temporary array
	int first1 = start;
	int first2 = mid + 1;
	int i = 0;
	while (first1 <= mid && first2 <= end)
	{
		if (items[first1] < items[first2]) //fills temporary array from small to large
			tmp[i++] = items[first1++];
		else
			tmp[i++] = items[first2++];
	}
	while (first1 <= mid) 
		tmp[i++] = items[first1++];//adding the rest of list before the pivot
	while (first2 <= end)
		tmp[i++] = items[first2++];//addiing the rest of list after the pivot
	for (i = 0; i <= (end - start); i++)
		items[start + i] = tmp[i];//replace all elements of items with items in tmp
	}
//	printArray();

}	

template <typename T>
void ArrayList<T>::quick_sort()
{
	if(length >= 1)
		quick_sort(0,length-1);//call private helped function
	return;

}
template <typename T>
void ArrayList<T>::quick_sort(size_t start, size_t end)
{
	//printArray();
	//std::cout << "- - - - - - -" << std::endl;
	if (start < end){
		T tmp;//variable for swapping elements
		T pivot = items[start];//item in pivot
		size_t last = start;
		for (int i = start + 1; i <= end; i++){
			if (items[i] < pivot){
				last++;
				tmp = items[i];
				items[i] = items[last];
				items[last] = tmp;		
			}
		}
		tmp = items[start];//swapping the items 
		items[start] = items[last];
		items[last] = tmp;
		//std::cout << start << " " << end << " " << last << " " << std::endl;
		quick_sort(start, last);//recursive calls on the list before the pivot and list after
		quick_sort(last+1, end);
	}
	return;
}

template<typename T>
void ArrayList<T>::sort()
{
	quick_sort();
}

#endif
