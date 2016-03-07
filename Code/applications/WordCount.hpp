/**
  * (K1,V1) ==> list(K2, V2)
 **/
void wc_mapf(const int& line_num, const string& doc_line, ResultsWrapper<pair<string, int>>& results_wrapper)
{
	// (line_num, doc_line) ==> list( (word, 1) )
	#ifdef VERBOSE
		cout << "Mapper ID=" << pthread_self() << ": extracting words from (" << line_num << ", " << doc_line << ") ..." << endl;
	#endif
	
    istringstream iss(doc_line);
    do
    {
        string word;
        iss >> word;
        if (word != "")
        {
            //cout << "Word: " << word << endl;
            results_wrapper.emit(pair<string, int>(std::move(word), 1));
            
            #ifdef VERBOSE
				cout << "Mapper ID=" << pthread_self() << ": emitted (" << results_wrapper.last().first << ", 1) ..." << endl;
			#endif
        }

    } while (iss);
}

/**
  * (K2, list(V2)) ==> list(V2)
 **/
void wc_reducef(const string& word, const list<int>& values, ResultsWrapper<pair<string, int>>& results_wrapper)
{
	// (word, [1,1,...,1]) ==> list( m ), with m = sum(1,1,...,1)
	#ifdef VERBOSE
		cout << "Reducer ID=" << pthread_self() << ": reducing 1's list for the word \"" << word << "\" ..." << endl;
	#endif
	
    int result = 0;
    for (std::list<int>::const_iterator iterator = values.begin(), end = values.end(); iterator != end; ++iterator)
        result += *iterator;
	
	results_wrapper.emit(pair<string, int>(std::move(word), result));
	    
    #ifdef VERBOSE
		cout << "Reducer ID=" << pthread_self() << ": reduce result is (" << word << ", " << result << ") ..." << endl;
	#endif
}

int wc_compare(const string& s1, const string& s2)
{
    return s1.compare(s2);
}

void word_count(const string dir_file_path, unsigned int thread_num)
{
	// set formats used to extract Record from the input file and initial (key, value) from each Record
    LineBasedRecordFormat<string> lbrf;
    RecordSelfContentKeyValueFormat<string> kvf;
        
    // invoke Skeleton constructor
	MapReduce<string, int, string, string, int> wc(dir_file_path, thread_num, &lbrf, &kvf, wc_compare, wc_mapf, wc_reducef);
    
    // run the MapReduce computation
    wc.run();
}
