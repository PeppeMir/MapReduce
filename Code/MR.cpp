#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <list>
#include <thread>
#include "LineBasedRecordFormat.hpp"
#include "RecordSelfContentKeyValueFormat.hpp"
#include "MapReduce.hpp"
#include "Timing.hpp"
#include "ResultsWrapper.hpp"

#define TIMING

using namespace std;

#include "applications/WordCount.hpp"
#include "applications/CosCos.hpp"
#include "applications/UserIDCount.hpp"

// **** usage ./MR THREAD_NUM ****
int main(int argc, char** argv)
{
	// check input parameters
	if (argc != 2)
    {
		cerr << "Usage " << argv[0] << " THREAD_NUM" << endl;
		exit(1);
	}
	
    // File System's path of the random generated file(s)
    const string dir_file_path = "./FileGen/generated/";					
        	
	// set the number of thread to use during the computation
	const unsigned int thread_num = atoi(argv[1]);
	
	// invoke an application of MapReduce
    //word_count(dir_file_path, thread_num);
	//cos_cos(dir_file_path, thread_num);
	user_id_count(dir_file_path, thread_num);
	
	return 0;
}
