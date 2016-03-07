/**
  * (K1,V1) ==> list(K2, V2)
 **/
void uid_mapf(const int& line_num, const int& num, ResultsWrapper<pair<int, int>>& results_wrapper)
{
	// (line_num, num) ==> list( (num, 1) )
	#ifdef VERBOSE
		cout << "Mapper ID=" << pthread_self() << ": extracting number from (" << line_num << ", " << num << ") ..." << endl;
	#endif
	
    results_wrapper.emit(pair<int, int>(num, 1));
            
	#ifdef VERBOSE
		cout << "Mapper ID=" << pthread_self() << ": emitted (" << results_wrapper.last().first << ", 1) ..." << endl;
	#endif
}

/**
  * (K2, list(V2)) ==> list(V2)
 **/
void uid_reducef(const int& num, const list<int>& values, ResultsWrapper<pair<int, int>>& results_wrapper)
{
	// (num, [1,1,...,1]) ==> list( m ), with m = sum(1,1,...,1)
	#ifdef VERBOSE
		cout << "Reducer ID=" << pthread_self() << ": reducing 1's list for the number \"" << num << "\" ..." << endl;
	#endif
	
    int result = 0;
    for (int v : values)
        result += v;
	
	results_wrapper.emit(pair<int, int>(num, result));
	    
    #ifdef VERBOSE
		cout << "Reducer ID=" << pthread_self() << ": reduce result is (" << num << ", " << result << ") ..." << endl;
	#endif
}

int uid_compare(const int& n1, const int& n2)
{
    if (n1 < n2)
		return -1;
	else
		if (n1 > n2)
			return 1;
		else
			return 0;
}

void user_id_count(const string dir_file_path, unsigned int thread_num)
{
	// set formats used to extract Record from the input file and initial (key, value) from each Record
    LineBasedRecordFormat<int> lbrf;
    RecordSelfContentKeyValueFormat<int> kvf;
        
    // invoke Skeleton constructor
	MapReduce<int, int, int, int, int> uidc(dir_file_path, thread_num, &lbrf, &kvf, uid_compare, uid_mapf, uid_reducef);
    
    // run the MapReduce computation
    uidc.run();
}
