#ifndef IMPLICIT_HEAP_HPP_
#define IMPLICIT_HEAP_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

/**
 * \file ImplicitHeap.hpp
 * \class ImplicitHeap
 * 
 * \brief Class which represents a ImplicitHeap, so an heap built implicitly on a std::vector
 **/
template<typename T> 
class ImplicitHeap
{
	public:
		typedef typename vector<T*>::const_iterator const_iterator;
		typedef typename vector<T*>::iterator iterator;

	private:
		vector<T*> heap;								// vector on which build the implicit heap
		function<bool(const T*, const T*)> compare;		// compare function for heap items
		bool created = false;							// creation flag
		
	public:
		/**
		 * \brief Default constructor
		 * 
		 * \param initial_capacity the initial capacity of the heap
		 * \param _compare comparison function for items of the heap
		 * 				   If a min-Heap is desired, the function must return true when the item1 is less than item2.
		 * 				   If a max-Heap is desired, the function must return true when the item1 is greater than item2.
		 **/
		ImplicitHeap(const unsigned int initial_capacity, function<bool(const T*, const T*)> _compare) 
			: compare(_compare)
		{			
			// allocate initial capacity into the vector
			heap.reserve(initial_capacity);
		}
		
		/**
		 * \brief Default destructor
		 **/
		~ImplicitHeap()
		{
			const unsigned int heap_size = heap.size();
			for (unsigned int i = 0; i < heap_size; i++)
				delete heap[i];
		}
		
		/**
		 * \brief insert the passed item into the heap
		 * \param the item to insert into the heap
		 **/
		void insert(T* item);
		
		/**
		 * \return extract the minimum (min-heap) or the maximum (max-heap) item from the heap
		 **/
		T* extract();
		
		/**
		 * \return return, without actually extract, the minimum (min-heap) or the maximum (max-heap) item from the heap
		 **/
		T* peek();
		
		/**
		 * \return the number of items into the heap
		 **/
		unsigned int size();
		
		/**
		 * \return a begin iterator for the heap
		 **/
		ImplicitHeap<T>::iterator begin();
		
		/**
		 * \return an end iterator for the heap
		 **/
		ImplicitHeap<T>::iterator end();

		/**
		 * \return a begin const_iterator for the heap
		 **/
		ImplicitHeap<T>::const_iterator cbegin();

		/**
		 * \return an end const_iterator for the heap
		 **/
		ImplicitHeap<T>::const_iterator cend();
};

#include "ImplicitHeap.tpp"

#endif	/* IMPLICIT_HEAP_HPP_ */
