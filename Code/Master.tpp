/**
 * \file Master.tpp
 * 
 * \brief implementation of the methods of the class \p Master present into the file \link Master.hpp
 **/

template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
void Master<TARGS>::wait_for_synchronization(const unsigned int synch_num)
{
	// wait on the condition variable until all the threads have not finished the work
	unique_lock<mutex> endwork_mutex_lock(endwork_mutex);
		
	// loop needed to avoid spurious wakeups
	while (joined_threads != synch_num)
		endwork_cond_var.wait(endwork_mutex_lock);
	
	// perform additional active-wait synchronization check
	skeleton.thread_manager.wait_all_idle();
}

/****************************************************************************************************************
 **************************************** MAP PHASE INITIALIZATION **********************************************
 ****************************************************************************************************************/
template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
inline void Master<TARGS>::init_map_phase(vector<Task>& _map_tasks, vector<ResultsWrapper<pair<K2, V2>>>& _interm_pairs)
{
	// set the number of Mappers to create (one per thread)
	mappers_num = skeleton.thread_num;
		
	// store intra-class pointer field to map-Tasks vector
	map_tasks = _map_tasks.data();
	  
    // reserve space for "mappers_num" sub-vectors into the passed vector &&
	// reserve space into the vector of Mappers (one per-thread)
	_interm_pairs.reserve(mappers_num);
	mappers.reserve(mappers_num);	
    for (unsigned int i = 0; i < mappers_num; i++)
    {		
		// allocate space for thread-local sub-vector of intermediate (K2, V2) which will be obtained
		// computing the map function on the map-Tasks
		_interm_pairs.push_back(ResultsWrapper<pair<K2, V2>>());
			
		// create and new Mapper passing as parameters:
		// 1) the synchronization id of the Mapper	
		// 2) (BY VALUE!) the map-Tasks to process	
		// 3) the i-th sub-vector of intermediate (K2, V2) to fill with the results obtained by the user-map function application
		mappers.push_back(new Mapper<TARGS>(*this, i, _interm_pairs[i]));	
	}
    
	#ifdef VERBOSE
		cout << "Master: Map phase successfully initialized (" << mappers_num << " Mapper(s)) ..." << endl;
	#endif
}

/****************************************************************************************************************
 **************************************************** MAP PHASE *************************************************
 ****************************************************************************************************************/
template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
inline void Master<TARGS>::start_map_phase()
{	
	#ifdef VERBOSE
		cout << "Master: started Map phase ..." << endl;
	#endif
		    	
	// set to zero the number of joined threads before the map-Tasks assignment
	joined_threads = 0;
	
	// then use the ThreadManager to execute each Mapper on an idle thread (they are in active-wait)
	for (unsigned int i = 0; i < mappers_num; i++)
		skeleton.thread_manager.execute_on_idle_thread(mappers[i]);

	#ifdef VERBOSE
		cout << "Master: all Mapper threads (" << mappers_num << ") started: waiting for synchronization ..." << endl;
	#endif

	// wait on the condition variable until all the threads have not finished the work
	wait_for_synchronization(mappers_num);

    #ifdef VERBOSE
		cout << "Master: all Mapper threads joined, quitting map phase ..." << endl;
    #endif
}

/****************************************************************************************************************
 ********************************************** MERGE & SORT PHASE **********************************************
 ****************************************************************************************************************/
template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
inline void Master<TARGS>::merge_and_sort(vector<ResultsWrapper<pair<K2, V2>>>& interm_pairs, vector<pair<K2, V2>>& sorted_interm_pairs_vector)
{
	#ifdef VERBOSE
		cout << "Master: sorting vector of vectors of sorted intermediate (K2, V2) ..." << endl;
    #endif
	
	// compute the exact size of the vector which will contain ALL the sorted intermediate (K2, V2) pairs
	unsigned int merged_size = 0;
	const unsigned int interm_pairs_size = interm_pairs.size();
	for (unsigned int i = 0; i < interm_pairs_size; i++)
		merged_size += interm_pairs[i].size();
	
	// reserve space for the vector of sorted intermediate pairs
	sorted_interm_pairs_vector.reserve(merged_size);
	
	#ifdef REPORT
		skeleton.interm_key_num = merged_size;
	#endif
	
	// allocate space for the vector which will contain the sorted (K2, V2) and sort the vector of vectors of intermediate (K2, V2)
	// Notice that the content of each individual sub-vector must be already sorted before this call, as specified in the class Sorter
    /*Sorter<pair<K2, V2>>::k_way_merge_sort(interm_pairs, sorted_interm_pairs_vector, 
    	[this](const pair<K2, V2>& pair1, const pair<K2, V2>& pair2) -> bool
		{
			return (skeleton.compare_fun(pair1.first, pair2.first) < 0);
		});*/
    
    Sorter<pair<K2, V2>>::k_way_merge_sort_heap(interm_pairs, sorted_interm_pairs_vector,
		[this](const pair<pair<K2, V2>, unsigned int>* pair1, const pair<pair<K2, V2>, unsigned int>* pair2) -> bool
		{
			return (skeleton.compare_fun((pair1->first).first, (pair2->first).first) > 0);
		});

}

/****************************************************************************************************************
 ******************************************** REDUCE PHASE INITIALIZATION ***************************************
 ****************************************************************************************************************/
template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
inline void Master<TARGS>::init_reduce_phase(vector<Task>& _reduce_tasks, vector<pair<K2, V2>>& _sorted_interm_pairs_vector, vector<ResultsWrapper<pair<K2, V2>>>& _reduce_results)
{	
	// set the number of Reducers to create
	reducers_num = _reduce_tasks.size();
	
	// store intra-class pointer field to reduce-Tasks vector 
	reduce_tasks = _reduce_tasks.data();
	sorted_interm_pairs_vector = _sorted_interm_pairs_vector.data();
	
	// reserve space for "reducers_num" sub-vectors into the passed vector of results &&
	// create space into the vector of Reducers (one-per-Reduce task)
	_reduce_results.reserve(reducers_num);
	reducers.reserve(reducers_num);
	for (unsigned int i = 0; i < reducers_num; i++)
	{
		// allocate space for thread-local sub-vector of intermediate (K2, V2) 
		_reduce_results.push_back(ResultsWrapper<pair<K2, V2>>());
		
		// create and new Reducer passing as parameters:
		// 1) the synchronization id of the Reducer
		// 2) the i-th sub-vector of V2 results to fill with the results obtained by the user-reduce function application
		reducers.push_back(new Reducer<TARGS>(*this, i, _reduce_results[i]));		
	}
}


/****************************************************************************************************************
 ************************************************* REDUCE PHASE (1) *********************************************
 ****************************************************************************************************************/
template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
inline void Master<TARGS>::start_reduce_phase(vector<ResultsWrapper<pair<K2, V2>>>& reduce_results, ResultsWrapper<pair<K2, V2>>& final_result)
{				
	#ifdef VERBOSE
		cout << "Master: started Reduce phase with " << reducers_num << " reduce-Tasks and ThreadManager of size " << skeleton.thread_manager.num_ready_threads() << " ..." << endl;
	#endif
	
	// set to zero the number of joined threads before the reduce-Tasks assignment
	joined_threads = 0;
	
	// then use the ThreadManager to execute the Reducer on an idle thread (they are in active-wait)
	for (unsigned int i = 0; i < reducers_num; i++)
		skeleton.thread_manager.execute_on_idle_thread(reducers[i]);

	#ifdef VERBOSE
		cout << "Master: all Reducer threads (" << reducers_num << ") started: waiting for synchronization ..." << endl;
	#endif
	
	// wait on the condition variable until all the Reduce threads haven't finished the work
	wait_for_synchronization(reducers_num);
	
	#ifdef VERBOSE
		cout << "Master: all Reducer threads joined, ";
		if (reducers_num != 1)
			cout << "starting final Reduce phase ..." << endl;
		else
			cout << "skipping final Reduce phase (only a reduce-Task has been processed) ..." << endl;
    #endif
	
	// perform final reducing
	if (reducers_num != 1)
	{
		joined_threads = 0;
		skeleton.thread_manager.execute_on_idle_thread(new FinalReducer<TARGS>(*this, 0, reduce_results, final_result));
		wait_for_synchronization(1);
	}
    //
    // here the vector "reduce_results" will contain a single block as result of the tree-applyied Reduce phase
    //
    
    #ifdef VERBOSE
		cout << "Master: all Reducer threads joined, quitting reduce phase ..." << endl;
    #endif
}
