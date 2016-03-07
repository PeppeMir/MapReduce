void cos_mapf(const int& row_n, const int& num, ResultsWrapper<pair<int, double>>& results_wrapper)
{	
	double acc = 0.0;
	for (long long i = 0; i < pow(num, 3); i++)
		acc += cos(acc);
	
	results_wrapper.emit(pair<int, double>(num, acc));	
}

void cos_reducef(const int& num, const list<double>& fatts, ResultsWrapper<pair<int, double>>& results_wrapper)
{
	double acc = 0.0;
	for (unsigned int j = 0; j < fatts.size(); j++)
	{
		acc += cos(acc);
		for (long long i = 0; i < 500000; i++)
			acc += (cos(acc) * sin(acc) + sin(acc) * cos(acc));
	}
	
	results_wrapper.emit(pair<int, double>(num, acc));
}

int cos_compare(const int& a, const int& b)
{
	if (a < b)
		return -1;
	else
		if (a > b)
			return 1;
		else
			return 0;
}

void cos_cos(const string dir_file_path, unsigned int thread_num)
{
	// set formats used to extract Record from the input file and initial (key, value) from each Record
    LineBasedRecordFormat<int> lbrf;
    RecordSelfContentKeyValueFormat<int> kvf;
    
    // invoke Skeleton constructor 
	MapReduce<int, int, int, int, double> cos_cos(dir_file_path, thread_num, &lbrf, &kvf, cos_compare, cos_mapf, cos_reducef);

    // run the MapReduce computation
    cos_cos.run();
}
