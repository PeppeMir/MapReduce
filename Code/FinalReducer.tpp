template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
inline pair<K2, V2>* FinalReducer<TARGS>::concat(unsigned int* concat_size)
{
	*concat_size = 0;
	const unsigned int blocks_dim = blocks.size();
	for (unsigned int i = 0; i < blocks_dim; i++)
		*concat_size += blocks[i].size();
	
	// allocate memory for the concatenation
	pair<K2, V2>* ptr_concat_blocks = new pair<K2, V2>[*concat_size];		
	for (unsigned int global= 0, i = 0; i < blocks_dim; i++)
	{
		const unsigned int block_size = blocks[i].size();
		const pair<K2, V2>* block_ptr = blocks[i].data();
		for (unsigned j = 0; j < block_size; j++, global++)
			ptr_concat_blocks[global] = std::move(block_ptr[j]);
	}

	return ptr_concat_blocks;
}


template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
void FinalReducer<TARGS>::execute()
{	
	// concatenate blocks/vectors
	unsigned int concat_size = 0;
	pair<K2, V2>* ptr_concat_blocks = concat(&concat_size);
	
	#ifdef VERBOSE
		cout << "FinalReducer ID=" << pthread_self() << ": processing final block of size=" << concat_size << " ..." << endl;
	#endif
	
	list<V2> values_group;
	pair<K2, V2>* curr_pair;
	pair<K2, V2>* next_pair;
	
	// apply reduce on the whole block
	unsigned int curr_idx = 0;
	const unsigned int end_idx = concat_size - 1;
	do
	{
		// get the current (K2, V2)
		curr_pair = &(ptr_concat_blocks[curr_idx++]);
		  
		// create a list and insert into it the V2 
		values_group = std::move(list<V2>(1, curr_pair->second));
			 
		while (curr_idx <= end_idx)
		{
			// if the K2 of the next pair is different than the K2 of the current pair, break the loop;
			// otherwise consider add the next pair's V2 into the list and go ahead
			next_pair = &(ptr_concat_blocks[curr_idx]);
			if (master.skeleton.compare_fun(curr_pair->first, next_pair->first) != 0)
				break;
				
			values_group.push_back(next_pair->second);
			curr_idx++;
		}
		 		
		// apply user-defined reduce function on the group of V2s associated to the current processed K2				 
		// and copy the obtained list into the results array
		master.skeleton.usr_reduce(curr_pair->first, values_group, final_result);		 
	} 
	while (curr_idx <= end_idx);		// loops until we are not over the end index
	
	// synchronize with the Master
	Synchronizable<TARGS>::synchronize_with_master(1);
	
	// free memory for concatenated block
	delete[] ptr_concat_blocks;
	
	#ifdef VERBOSE
		cout << "Reducer ID=" << pthread_self() << ": reduce phase completed and results successfully shared with the Master, returning ..." << endl;
	#endif
}
