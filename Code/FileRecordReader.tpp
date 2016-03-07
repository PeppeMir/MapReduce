/**
 * \file FileRecordReader.tpp
 * \brief implementation of the methods of the class \p FileRecordReader present into the file \link FileRecordReader.hpp
 **/

template<typename RecordContentType>
void FileRecordReader<RecordContentType>::process_files(const string& dir_files_path, RecordFormat<RecordContentType>* record_format, const unsigned int thread_num, vector<Record<RecordContentType>>& record_vector)
{
	// obtain vector of pairs (file_path, file_size)
	vector<pair<string, long>> file_names;
	file_names_from_dir_path(dir_files_path, file_names);
	
	// exit if no files have been found in the directory
	const unsigned int num_of_files = file_names.size();
	if (num_of_files == 0)
	{
		cerr << "FileRecordReader: no files found in the directory specified at \"" << dir_files_path << "\" ..." << endl;
		return;
	}
	
	// initialize RecordFormat to use to extract Record from the files 
	FileRecordReader<RecordContentType>::record_format = record_format;
	
	// if there is only a file in the directory, use SEQUENTIAL code to process it
	if (num_of_files == 1)
	{
		process_file(file_names[0].first, record_vector);
		return;
	}
	
	// ... otherwise, process the files in PARALLEL using more threads

	// sort the array in deacreasing order of file size
	sort(file_names.rbegin(), file_names.rend(), pair_compare_less_than);
			
	// exploit the maximum number of processors to process the input file(s): 
	// if the number of files is less than the number of thread to use, then use a thread for each file;
	// otherwise, run the desired number of threads and distribute the files among them  
	const unsigned int num_used_threads = (num_of_files < thread_num) ? num_of_files : thread_num;
	
	// fill vector with values { num_of_files - 1, num_of_files - 2, ..., 0 }
	mutex index_mutex;
	vector<unsigned int> index_vector(num_of_files);
	for (unsigned int j = 0; j < num_of_files; j++)
		index_vector[j] = num_of_files - j - 1;

	// launch threads to extract in parallel the Records from the files
	Scheduler sched;
	vector<thread> file_threads;
	file_threads.reserve(num_used_threads);
	for (unsigned int i = 0; i < num_used_threads; i++)
	{
		const unsigned short proc_id = sched.get_next_free_processor();
		file_threads.push_back(thread(FileRecordReader<RecordContentType>::process_file_parallel, proc_id, &file_names, &index_vector, &index_mutex));
	}
	
	#ifdef VERBOSE
		cout << "FileRecordReader: using " << num_used_threads << " thread(s) to process " << num_of_files << " file(s) ..." << endl;
	#endif

	// wait for threads termination (results present in FileRecordReader<RecordContentType>::merged_record_vector)
	for (unsigned int i = 0; i < num_used_threads; i++)
		file_threads[i].join();
		
	#ifdef VERBOSE
		cout << "FileRecordReader: all threads joined ... merging results ..." << endl;
	#endif
	
	// merge into an unique vector all the Records contained into the vectors filled by the threads 
	// TODO: avoid "push_back()" calls using fixed size vector?
	const unsigned int vv_size = FileRecordReader<RecordContentType>::merged_record_vector.size();
	for (unsigned int i = 0; i < vv_size; i++)	
	{
		const vector<Record<RecordContentType>>* v = FileRecordReader<RecordContentType>::merged_record_vector[i];
		const unsigned int v_size = v->size();
		for (unsigned int j = 0; j < v_size; j++)
			record_vector.push_back((*v)[j]);
	}
	
	#ifdef VERBOSE
		cout << "FileRecordReader: merging done. Built RAM vector of " << record_vector.size() << " Records ..." << endl;
	#endif
}

template<typename RecordContentType>
void FileRecordReader<RecordContentType>::process_file(const string& file_path, vector<Record<RecordContentType>>& record_vector)
{
	// try to open the file
    ifstream file(file_path);
    if (!file.is_open())
    {
        cerr << "Unable to open the file at \"" << file_path << "\" ..." << endl;
        return;
    }

	#ifdef VERBOSE
		cout << "Extracting Records from file at \"" << file_path << "\" ..." << endl;
	#endif
	
    // until EOF has not been reached
    while (!file.eof())
    {
        // extract a Record from the file
        const Record<RecordContentType>* record = FileRecordReader<RecordContentType>::record_format->extract_record(&file);

        // store the extracted record into the vector of records
        if (record != nullptr)
        {
            record_vector.push_back(*record);
            delete record;
		}
    }

    // close the file
    file.close();
	
	#ifdef VERBOSE
		cout << "FileRecordReader: built RAM-vector of " << record_vector.size() << " Record:" << endl;
    #endif
}

template<typename RecordContentType>
void FileRecordReader<RecordContentType>::process_file_parallel(const unsigned short proc_id, vector<pair<string, long>>* file_names, vector<unsigned int>* index_vector, mutex* index_mutex)
{
	// try to set thread affinity on processor identified by the passed processor ID
    if (!Scheduler::set_thread_affinity(proc_id))
       return;
	
	string file_name;
	vector<Record<RecordContentType>>* record_vector;
	while (true)
	{
		// LOCK for file names vector access
		index_mutex->lock();
		
		// if no more pairs (file_name, file_size) are available, break the cycle
		if (file_names->size() == 0)
		{
			index_mutex->unlock();
			break;
		}
		
		// extract a pair (file_name, file_size) from the end of the vector 
		file_name = file_names->back().first;
		file_names->pop_back();
		
		// UNLOCK for indexes vector access
		index_mutex->unlock();
		
		// process the single file extracting Records from it and fill the "record_vector" using them
		process_file(file_name, *record_vector);
	}
	
	// LOCK for shared vector access
	FileRecordReader<RecordContentType>::merged_mutex.lock();
	
	// push the extracted Records into the shared vector 
	FileRecordReader<RecordContentType>::merged_record_vector.push_back(record_vector);
	
	// UNLOCK for shared vector access
	FileRecordReader<RecordContentType>::merged_mutex.unlock();
}

template<typename RecordContentType>
void FileRecordReader<RecordContentType>::file_names_from_dir_path(const string& dir_files_path, vector<pair<string, long>>& file_names)
{
	DIR *dir;
	struct dirent *file;
	
	// try to open the directory specified by the path
	if ((dir = opendir(dir_files_path.c_str())) == NULL) 
		cerr << "FileRecordReader: could not open directory at \"" << dir_files_path << "\" ..." << endl;
	else
	{
		#ifdef VERBOSE
			cout << "FileRecordReader: search for file(s) in directory \"" << dir_files_path << "\" ..." << endl;	
		#endif
			
		// process all the files within the directory
		struct stat sb;
		while ((file = readdir(dir)) != NULL) 
		{
			// build absolute path for the file
			const string abs_file_path = dir_files_path + file->d_name;
			
			// get statistics for the current processed file
			if (stat(abs_file_path.c_str(), &sb) == 0)
			{				
				// check if the file is a directory and, if so, skip it
				if (S_ISDIR(sb.st_mode))
				{
					#ifdef VERBOSE
						//cout << "FileRecordReader: skipped file \"" << file->d_name << "\" (is directory)..." << endl;	
					#endif
					continue;
				}
							
				// get the size of the current processed file
				const long file_size = sb.st_size / 1024 / 1024;
				
				// create the (file_path, files_size) using file's data 
				const pair<string, long> pr(abs_file_path, file_size);

				#ifdef VERBOSE
					cout << "FileRecordReader: found file \"" << file->d_name << "\" (~ " << file_size << " MB) ..." << endl;	
				#endif

				// insert the pair into the vector 
				file_names.push_back(pr);
			}
			else 
			{
				cerr << "FileRecordReader: could not get stat for file \"" << file->d_name << "\": ";
				perror(0);
				break;
			}
		}
	
		// close the above open directory
		closedir(dir);	
	}
}

template<typename RecordContentType>
bool FileRecordReader<RecordContentType>::pair_compare_less_than(const pair<string, long>& p1, const pair<string, long>& p2) 
{
	return p1.second < p2.second;
}


// static fields initialization
template<typename RecordContentType>
RecordFormat<RecordContentType>* FileRecordReader<RecordContentType>::record_format;

template<typename RecordContentType>
mutex FileRecordReader<RecordContentType>::merged_mutex;

template<typename RecordContentType>
vector<vector<Record<RecordContentType>>*> FileRecordReader<RecordContentType>::merged_record_vector;
