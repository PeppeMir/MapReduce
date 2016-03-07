template<typename T>
unsigned int Sorter<T>::find_first_non_empty(vector<ResultsWrapper<T>>& v)
{
	const unsigned int sz = v.size();
	for (unsigned int i = 0; i < sz; i++)
		if (v[i].size() != 0)
			return i;
			
	return sz;
}


template<typename T>
void Sorter<T>::k_way_merge_sort(vector<ResultsWrapper<T>>& to_sort, vector<T>& sorted, function<bool(const T&, const T&)> less_than_comp_fun)
{
	// for each index of the vector which will contain the sorted items
	const unsigned int to_sort_size = to_sort.size();
	const unsigned int sorted_size = sorted.capacity();
	for (unsigned int srt_idx = 0; srt_idx < sorted_size; srt_idx++)
	{
		// find the index of the first sub-vector which contains at least an item
		unsigned int to_sort_index = find_first_non_empty(to_sort);
		
		// compare the last items of each sub-vector in order to find the index of the sub-vector
		// containing the smallest item, using the ordering defined by the "less_than_comp_fun" for the comparison  
		for (unsigned int j = to_sort_index + 1; j < to_sort_size; j++)
		{
			if (to_sort[j].size() != 0 && less_than_comp_fun(to_sort[j].last(), to_sort[to_sort_index].last()))
				to_sort_index = j;
		}
		
		// extract, using the above found index, the last item and insert it into the sorted vector  
		sorted.push_back(to_sort[to_sort_index].last());
		to_sort[to_sort_index].delete_last();
	}
}

template<typename T>
void Sorter<T>::k_way_merge_sort_heap(vector<ResultsWrapper<T>>& to_sort, vector<T>& sorted, function<bool(const pair<T, unsigned int>*, const pair<T, unsigned int>*)> greater_than_comp_fun)
{	
	// NB: the vector "sorted" MUST be pre-allocated using the "reserve" function
	// for each index of the vector which will contain the sorted items
	const unsigned int to_sort_size = to_sort.size();
	const unsigned int sorted_size = sorted.capacity();
	
	// create a min-Heap able to contain "to_sort_size" pairs and that uses the "greater_than_comp_fun"
	// function to sort the pairs itself in increasing order
	ImplicitHeap<pair<T, unsigned int>> min_heap(to_sort_size, greater_than_comp_fun);
	
	// initialize the min-Heap inserting a pair for each already sorted sub-vector.
	// The i-th inserted pair contains:
	// - as first component, the last item of the i-th sub-vector (i.e. "to_sort[i].back()");
	// - as second componenet, the index of the i-th sub-vector (i.e. "i")
	for (unsigned int i = 0; i < to_sort_size; i++)
	{
		// create the pair and insert it into the min-Heap
		pair<T, unsigned int>* p = new pair<T, unsigned int>(std::move(to_sort[i].last()), i);
		min_heap.insert(p);
		
		// delete the item from the vector
		to_sort[i].delete_last();
	}
		
	// for each position of the vector which will contain the sorted items
	for (unsigned int srt_idx = 0; srt_idx < sorted_size; srt_idx++)
	{
		// extract from the min-Heap (cost O(1)) the pair having the smallest item as first component
		const pair<T, unsigned int>* extracted_pair = min_heap.extract();
				
		// insert the first component of the above extracted pair into the "str_idx"-th position 
		// of the vector which will contain the sorted items   
		sorted.push_back(extracted_pair->first);
		
		// use the second component of the above extracted pair to identify the sub-vector from which extract
		// the last item: then this item is insert into the min-Heap (O(logN) cost) in order to replace the other one
		const unsigned int next_index = extracted_pair->second;
		if (to_sort[next_index].size() > 0)
		{			
			// create the new pair and insert it into the min-Heap
			pair<T, unsigned int>* new_pair = new pair<T, unsigned int>(std::move(to_sort[next_index].last()), next_index);
			min_heap.insert(new_pair);
			
			// remove from the vector the item above inserted into the min-Heap
			to_sort[next_index].delete_last();
		}
	
		// free memory allocated for the extracted pair
		delete extracted_pair;
	}
}
