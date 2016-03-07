/**
 * \file MapReduce.tpp
 * 
 * \brief implementation of the methods of the class \p MapReduce present into the file \link MapReduce.hpp
 **/
 
template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
bool MapReduce<TARGS>::run()
{
	// declare Timing object
	Timing total_timer, single_timer;
	
	#ifdef TOTAL_TIMING
		total_timer.start_timing();
	#endif
	   
	// check Skeleton's parameters validity
    check_parameters_validity();
    
	cout << "**** MapReduce Skeleton running [ #OnlineProcessors=" << onln_proc_num << " #UsedThreads=" << thread_num << " ] ****" << endl;
	
	// set the affinity on the current thread using the Scheduler
	Scheduler::set_thread_affinity_master(thread_num);
 	
 	/******************************************************************/
    #ifdef FILE_READER_TIMING
    	single_timer.start_timing();
	#endif
	
	// PHASE 1): extract Records from the input file(s) filling the vector of Records
    FileRecordReader<RecordContentType>::process_files(dir_files_path, record_format, thread_num, record_vector);

	// if the vector of Records is empty, abort the computation since an error occurred
    if (record_vector.size() == 0)
		ABORT
		   
    #ifdef FILE_READER_TIMING
    	single_timer.stop_timing();
    	file_read_time = single_timer.get_millisecond_time();
    	cout << "FileRecordReader took " << file_read_time << " ms" << endl;
	#endif
	/******************************************************************/
	
	/******************************************************************/
    #ifdef TASK_GEN_TIMING
    	single_timer.start_timing();
	#endif
	
	// PHASE 2): logically split the vector of Records into map-Task(s) and insert them into a vector of Tasks
	vector<Task> map_tasks;
	TaskGenerator::generate_tasks<Record<RecordContentType>>(thread_num, record_vector, map_tasks);
	
 	// if the vector of map-Task is empty, abort the computation since an error has occurred during map-Tasks creation
	if (map_tasks.size() == 0)
		ABORT
		
    #ifdef TASK_GEN_TIMING
    	single_timer.stop_timing();
    	map_task_gen_time = single_timer.get_millisecond_time();
    	cout << "TaskGenerator map-Tasks generation took " << map_task_gen_time << " ms" << endl;
	#endif
	
	#ifdef CHECK
		cout << "MapReduce: check created map-Task(s):" << endl;
		check_tasks(map_tasks);
	#endif
	/*******************************************************/
	
    // build Master in order to assign the map-Tasks to the Mappers and reduce-Tasks to the Reducers
    Master<TARGS> master(*this);
    
    
   	/******************************************************************/
    #ifdef THREAD_POOL_TIMING
		single_timer.start_timing();
    #endif
    
    // initialize the ThreadManager in order to initialize "thread_num" threads 
    // each thread is able to wait for a job performing active-wait 
	thread_manager.init(thread_num, scheduler);
    
    #ifdef THREAD_POOL_TIMING
        single_timer.stop_timing();
    	thread_pool_time = single_timer.get_millisecond_time();
    	cout << "ThreadManager initialization took " << thread_pool_time << " ms" << endl;
    #endif
	/******************************************************************/    
    
	/******************************************************************/
    #ifdef MAP_INIT_TIMING
		single_timer.start_timing();
    #endif
    
    // PHASE 3.0): initialize Master for Map phase
    vector<ResultsWrapper<pair<K2, V2>>> interm_pairs;
    master.init_map_phase(map_tasks, interm_pairs);

    #ifdef MAP_INIT_TIMING
		single_timer.stop_timing();
		map_init_phase_time = single_timer.get_millisecond_time();
    	cout << "Map phase initialization took " << map_init_phase_time << " ms" << endl;
    #endif
	/******************************************************************/

        
	/******************************************************************/
    #ifdef MAP_TIMING
		single_timer.start_timing();
    #endif
    
    // PHASE 3.1): Map phase: given a vector of map-Tasks, obtain a vector of vectors of sorted intermediate (K2, V2)
	master.start_map_phase();
	
    #ifdef MAP_TIMING
		single_timer.stop_timing();
		map_phase_time = single_timer.get_millisecond_time();
    	cout << "Mappers took " << map_phase_time << " ms" << endl;
    #endif
	
	#ifdef CHECK
		cout << "MapReduce: check Map output:" << endl;
		check_vv_pairs(interm_pairs);
	#endif
	/******************************************************************/


	/******************************************************************/
	#ifdef SORT_TIMING
		single_timer.start_timing();
    #endif
    
   	// PHASE 4): Sorting phase: given a vector of vectors of already sorted intermediate (K2, V2), sort and merge those
	// sub-vectors in a single sorted vector
	vector<pair<K2, V2>> sorted_interm_pairs_vector; 
    master.merge_and_sort(interm_pairs, sorted_interm_pairs_vector);
    
    #ifdef SORT_TIMING
		single_timer.stop_timing();
		sorting_time = single_timer.get_millisecond_time();
    	cout << "Sorting phase took " << sorting_time << " ms" << endl;
    #endif
    
    #ifdef CHECK
		cout << "MapReduce: check sorting output:" << endl;
		check_v_pairs(sorted_interm_pairs_vector);
	#endif
	/******************************************************************/

	/******************************************************************/
	#ifdef TASK_GEN_TIMING
    	single_timer.start_timing();
	#endif
	
	// PHASE 5): logically split the vector of sorted intermediate (K2, V2) into reduce-Task(s) and insert them into a vector of Tasks
	vector<Task> reduce_tasks; 
   	TaskGenerator::generate_tasks<pair<K2, V2>>(thread_num, sorted_interm_pairs_vector, reduce_tasks);

    #ifdef TASK_GEN_TIMING
    	single_timer.stop_timing();
    	reduce_task_gen_time = single_timer.get_millisecond_time();
    	cout << "TaskGenerator reduce-Tasks generation took " << reduce_task_gen_time << " ms" << endl;
	#endif
		
    #ifdef CHECK
		cout << "MapReduce: check created reduce-Task(s):" << endl;
		check_tasks(reduce_tasks);
    #endif
	/******************************************************************/

	/******************************************************************/
	#ifdef REDUCE_INIT_TIMING
		single_timer.start_timing();
    #endif
    
	// PHASE 6.0): initialize Master for Reduce phase
    vector<ResultsWrapper<pair<K2, V2>>> reduce_results;
    master.init_reduce_phase(reduce_tasks, sorted_interm_pairs_vector, reduce_results);

    #ifdef REDUCE_INIT_TIMING
		single_timer.stop_timing();
		reduce_init_phase_time = single_timer.get_millisecond_time();
    	cout << "Reduce phase initialization took " << map_init_phase_time << " ms" << endl;
    #endif
	/******************************************************************/

	/******************************************************************/
	#ifdef REDUCE_TIMING
		single_timer.start_timing();
	#endif
	
	// PHASE 6.1): Reduce phase: given a vector of reduce-Tasks and the vector of sorted intermediate (K2, V2), obtain 
	//			 a vector containing a vector of V2 for each different intermediate key K2
	ResultsWrapper<pair<K2, V2>> final_result;
	master.start_reduce_phase(reduce_results, final_result);
	
	#ifdef REDUCE_TIMING
		single_timer.stop_timing();
		reduce_phase_time = single_timer.get_millisecond_time();
		cout << "Reducers took " << reduce_phase_time << " ms" << endl;
	#endif
	
	#ifdef CHECK
		cout << "MapReduce: check Reduce output:" << endl;
		check_v_pairs(final_result);
	#endif
	/******************************************************************/

	#ifdef VERBOSE
		cout << "MapReduce: shutting down ThreadManager ..." << endl;
	#endif
	
	// shutting down the ThreadManager
	thread_manager.shut_down();
		
	#ifdef TOTAL_TIMING
		total_timer.stop_timing();
		tot_time = total_timer.get_millisecond_time();
		cout << "MapReduce Skeleton took " << tot_time << " ms" << endl;
	#endif
		
	#ifdef REPORT
		const string header = REPORT_HEADER;
		const string values = generate_report_string();
		int res = system(("./repo_gen.sh " + header + " " + values).c_str());
		if (res == 0)
		{
			cout << "**** Report file successfully updated ****" << endl;
			return true;
		}
		else
		{
			cerr << "**** Unable to update the Report file (error code " << res << ") ****" << endl;
			return false;
		}
	#endif
	
	return true;
}	

template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
inline void MapReduce<TARGS>::check_parameters_validity()
{
	// the number of thread must be positive
    if (thread_num == 0)
        PARAM_ABORT("Thread number")
	
	// check if the format used to extract the Records from the input file(s) has been set
    if (record_format == nullptr)
        PARAM_ABORT("Record format")

	// check if the format used to extract the initial (key, value) from the Records has been set
    if (keyvalue_format == nullptr)
        PARAM_ABORT("Key-Value format")

	// check if the comparison function used to sort the intermediate (key, value) by key has been set
    if (compare_fun == nullptr)
        PARAM_ABORT("Intermediate (K2, V2)'s keys comparison function")
	
	// check if the user-defined map function has been set
    if (usr_map == nullptr)
        PARAM_ABORT("User-defined Map function")

	// check if the user-defined reduce function has been set
    if (usr_reduce == nullptr)
        PARAM_ABORT("User-defined Reduce function")
    
    // get the number of online processors and check if the specified number of thread is greater than that number
    onln_proc_num = Scheduler::get_onln_processors_number();
    if (thread_num > onln_proc_num)
    {
		cerr << "MapReduce: the number of chosen threads (" << thread_num << ") is greater than the architecture availabily (" << onln_proc_num << ") ... Abort." << endl; 
		exit(-1);
	}
		
	#ifdef REPORT
		num_interm_keys_mappers = std::move(vector<unsigned int>(thread_num, 0));
		local_mapper_sorting_time = std::move(vector<float>(thread_num));
	#endif
}

#ifdef CHECK

template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
inline void MapReduce<TARGS>::check_tasks(vector<Task>& tasks)
{
	for (unsigned int i = 0; i < tasks.size(); i++)
		tasks[i].print();
}

template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
inline void MapReduce<TARGS>::check_v_pairs(vector<pair<K2, V2>>& pairs_v)
{
	for (unsigned int k = 0; k < pairs_v.size(); k++)
		cout << "(" << pairs_v[k].first << ", " << pairs_v[k].second << ")" << endl;
}

template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
inline void MapReduce<TARGS>::check_v_pairs(ResultsWrapper<pair<K2, V2>>& pairs_v)
{
	for (unsigned int k = 0; k < pairs_v.size(); k++)
		cout << "(" << pairs_v[k].first << ", " << pairs_v[k].second << ")" << endl;
}

template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
inline void MapReduce<TARGS>::check_vv_pairs(vector<ResultsWrapper<pair<K2, V2>>>& pairs_vv)
{
	for (unsigned int i = 0; i < pairs_vv.size(); i++)
	{					
		cout << "sub-vector #" << i << ":";
		for (unsigned int j = 0; j < pairs_vv[i].size(); j++)
			cout << " (" << pairs_vv[i].at(j).first << "," << pairs_vv[i].at(j).second << ")";
		cout << endl;
	}
}

#endif

#ifdef REPORT

template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
inline string MapReduce<TARGS>::generate_report_string()
{
	// declare fields for averages
	float avg_local_mapper_sorting_time = 0.0;
	unsigned int avg_interm_keys_mappers = 0;
	
	// compute sums
	for (unsigned int i = 0; i < thread_num; i++)
	{
		avg_interm_keys_mappers += num_interm_keys_mappers[i];
		avg_local_mapper_sorting_time += local_mapper_sorting_time[i];
	}
	
	// compute averages
	avg_local_mapper_sorting_time /= thread_num;
	avg_interm_keys_mappers /= thread_num;

	// header format:
	// NUM_RECORD,NUM_THREAD,FILE_READING_TIME,
	// THREAD_POOL_INIT_TIME,MAP_TASK_GEN_TIME,MAP_INIT_TIME,
	// MAPPERS_TIME,INTERM_KEYS_MAPPER_AVGNUM,
	// LOCAL_SORT_AVGTIME,SORT_TIME,INTERM_KEYS_NUM,REDUCE_TASK_GEN_TIME,
	// REDUCE_INIT_TIME,REDUCERS_TIME,TOTAL_TIME
	stringstream ss;
	ss << record_vector.size() << "," << thread_num << "," << file_read_time << ","
	   << thread_pool_time << "," << map_task_gen_time << "," << map_init_phase_time << "," 
	   << map_phase_time << "," << avg_interm_keys_mappers << "," 
	   << avg_local_mapper_sorting_time << "," << sorting_time << "," << interm_key_num << "," << reduce_task_gen_time << "," 
	   << reduce_init_phase_time << "," << reduce_phase_time << "," << tot_time;
	return ss.str();
}

#endif
