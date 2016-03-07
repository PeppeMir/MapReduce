/**
 * \file Reducer.tpp
 * 
 * \brief implementation of the methods of the class \p Reducer present into the file \link Reducer.hpp
 **/
 
template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
void Reducer<TARGS>::execute()
{
	#ifdef VERBOSE
		cout << "Reducer ID=" << pthread_self() << ": processing reduce-Task ";
		master.reduce_tasks[synch_id].print();
	#endif
	
	list<V2> values_group;
	pair<K2, V2>* curr_pair;
	pair<K2, V2>* next_pair;
	
    unsigned int curr_idx = master.reduce_tasks[synch_id].start_index();
	const unsigned int end_idx = master.reduce_tasks[synch_id].end_index();
	do
	{
		// get the current (K2, V2)
		curr_pair = &(master.sorted_interm_pairs_vector[curr_idx++]);
		  
		// create a list and insert V2 
		values_group = std::move(list<V2>(1, curr_pair->second));
			 
		while (curr_idx <= end_idx)
		{
			// if the K2 of the next pair is different than the K2 of the current pair, break the loop;
			// otherwise consider add the next pair's V2 into the list and go ahead
			next_pair = &(master.sorted_interm_pairs_vector[curr_idx]);
			if ( master.skeleton.compare_fun(curr_pair->first, next_pair->first) != 0 )
				break;
			
			values_group.push_back(std::move(next_pair->second));
			curr_idx++;
		}
		
		// aply user-defined reduce function on the group of V2s associated to the current processed K2				 
		// the results obtained by the computation are in "results"
		master.skeleton.usr_reduce(curr_pair->first, values_group, results);
	} 
	while (curr_idx <= end_idx);		// loops until we are not over the reduce-Task's end index
	
	// increase the number of joined threads and wake up the Master, if the thread is the last to finish the work
	Synchronizable<TARGS>::synchronize_with_master(master.reducers_num);
	
	#ifdef VERBOSE
		cout << "Reducer ID=" << pthread_self() << ": reduce phase completed and results successfully shared with the Master, returning ..." << endl;
	#endif
}
