#ifndef FILE_RECORD_READER_HPP_
#define FILE_RECORD_READER_HPP_

#include <iostream>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <thread>
#include <mutex>
#include "RecordFormat.hpp"
#include "Scheduler.hpp"

using namespace std;

/**
 * \file FileRecordReader.hpp
 * \class FileRecordReader
 * 
 * \brief Class able to read the content of the file contained into a certain directory and extract Record
 * 		  from them.
 **/
template<typename RecordContentType>
class FileRecordReader
{
	private:
		static RecordFormat<RecordContentType>* record_format; 						// format to use in order to extract Record(s) from an input file
		static mutex merged_mutex;													// mutex to protect accesses to the vector of vector of Records
		static vector<vector<Record<RecordContentType>>*> merged_record_vector; 	// vector of vectors of Records: each vector represent the Records extracted from a single file
		
	public:
		/**
		 * \brief start the process of the files contained into the directory identified by the passed path
		 * 
		 * \param dir_files_path the File System's path of the directory which contains the file(s) to process
		 * \param record_format the format to use to extract Records from the files
		 * \param record_vector the vector to fill using the Records extract from the found files
		 **/
		static void process_files(const string& dir_files_path, RecordFormat<RecordContentType>* record_format, const unsigned int thread_num, vector<Record<RecordContentType>>& record_vector);
			
	private:
		/**
		 * \brief start the sequential process of the file identified by the passed path
		 * 
		 * \param file_path the File System's path of the file to process
		 * \param record_vector the vector to fill using the Records extract from the processed file
		 **/
		static void process_file(const string& file_path, vector<Record<RecordContentType>>& record_vector);
		
		/**
		 * \brief start the parallel process of the files identified by the paths contained into the passed vector of file names 
		 * 
		 * \param file_names the vector containing a (file name, file size) for each found file to process
		 * \param index_vector the vector of indexes of the above vector. Each thread extract indexes in order to access the vector
		 * \param index_mutex mutex used to protect accesses on the vector of indexes
		 **/
		static void process_file_parallel(const unsigned short proc_id, vector<pair<string, long>>* file_names, vector<unsigned int>* index_vector, mutex* index_mutex);

		/**
		 * \brief Extract a pair (file name, file size) for each regular file found into the directory specified by the path passed
		 * 		  as parameter. The size of the file is expressed in MByte.
		 * 
		 * \param the File System's path of the directory in which search for files
		 * \param a vector of pairs (file name, file size) to fill for each regular file found into the directory specified
		 * 		  by the path passed as argument.
		 **/
		static void file_names_from_dir_path(const string& dir_files_path, vector<pair<string, long>>& file_names);
		
		/**
		 * \brief Checks if the first passed pair is less of the second passed pair 
		 * 
		 * \param p1 the first pair to compare
		 * \param p2 the second pair to compare
		 * \return true if the first passed pair is less than the second passed pair; false otherwise.
		 **/
		static bool pair_compare_less_than(const pair<string, long>& p1, const pair<string, long>& p2);
};

#include "FileRecordReader.tpp"

#endif /* FILE_RECORD_READER_HPP_ */
