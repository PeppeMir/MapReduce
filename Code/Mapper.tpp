/**
 * \file Mapper.tpp
 * \brief implementation of the methods of the class \p Mapper present into the file \link Mapper.hpp
 **/

template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
void Mapper<TARGS>::execute()
{			
	// process the current map-Task applying the user-map to each Record indexed by its indexes
	const unsigned int end_idx = master.map_tasks[synch_id].end_index();
	for (unsigned int curr_idx = master.map_tasks[synch_id].start_index(); curr_idx <= end_idx; curr_idx++)
	{
		// extract the i-th initial (K1, V1) from the i-th Record
		const pair<K1, V1> curr_kv_pair = master.skeleton.keyvalue_format->extract_keyvalue_pair(&master.skeleton.record_vector[curr_idx]);

		// apply the user-map function on the (K1, V1), obtaining results into "local_interm_pairs"
		master.skeleton.usr_map(curr_kv_pair.first, curr_kv_pair.second, local_interm_pairs);
	}
	
	#if defined(REPORT) && defined(INTERM_KEYS_COUNTING)
		master.skeleton.num_interm_keys_mappers[synch_id] = local_interm_pairs.size();
	#endif
	
	#ifdef VERBOSE
		cout << "Mapper ID=" << pthread_self() << ": map_task successfully processed! Sorting local intermediate (K2, V2) vector..." << endl;
	#endif	
	
	#if defined(REPORT) && defined(LOCAL_SORT_TIMING)
		Timing t;
		t.start_timing();
	#endif
	
	// reverse-sort (note the "rbegin" and the "rend") the thread-local intermediate pairs vector
    sort(local_interm_pairs.rbegin(), local_interm_pairs.rend(), 
		[this](const pair<K2, V2>& pair1, const pair<K2, V2>& pair2) -> bool
		{ 
			return (master.skeleton.compare_fun(pair1.first, pair2.first) < 0);
		});

	#if defined(REPORT) && defined(LOCAL_SORT_TIMING)
		t.stop_timing();
		master.skeleton.local_mapper_sorting_time[synch_id] = t.get_millisecond_time();
	#endif
	
	// synchronize with the Master
	Synchronizable<TARGS>::synchronize_with_master(master.mappers_num);

	#ifdef VERBOSE
		cout << "Mapper ID=" << pthread_self() << ": local intermediate (K2, V2) vector successfully sorted and shared with the Master, returning ..." << endl;
	#endif
}
