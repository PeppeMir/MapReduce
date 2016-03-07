#ifndef SORTER_HPP_
#define SORTER_HPP_

#include <iostream>
#include <cstdlib>
#include <vector>
#include <functional>
#include "ImplicitHeap.hpp"

using namespace std;

/**
 * \file Sorter.hpp
 * \class Sorter
 * 
 * \brief Class which represents a Sorter for vector of ResultsWrappers.
 **/
template<typename T>
class Sorter
{
	public:
		static unsigned int find_first_non_empty(vector<ResultsWrapper<T>>& v);

		/**
		 * \brief k-way merge sort implementation
		 * 		  The method requires the ResultsWrappers are already sorted in decreasing order. 
		 **/
		static void k_way_merge_sort(vector<ResultsWrapper<T>>& to_sort, vector<T>& sorted, function<bool(const T&, const T&)> less_than_comp_fun); 
		
		/**
		 * \brief k-way merge sort implementation using a min-heap
		 *  	  The method requires the ResultsWrappers are already sorted in decreasing order. 
		 **/
		static void k_way_merge_sort_heap(vector<ResultsWrapper<T>>& to_sort, vector<T>& sorted, function<bool(const pair<T, unsigned int>*, const pair<T, unsigned int>*)> greater_than_comp_fun);
};

#include "Sorter.tpp"

#endif /* SORTER_HPP_ */
