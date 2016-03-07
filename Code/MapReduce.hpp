#ifndef MAPREDUCE_HPP_
#define MAPREDUCE_HPP_

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <list>
#include <sstream>
#include <functional>
#include "FileRecordReader.hpp"
#include "KeyValueFormat.hpp"
#include "TaskGenerator.hpp"
#include "Master.hpp"
#include "Timing.hpp"

#define ABORT { cout << "MapReduce: abort computation." << endl; exit(-1); }
#define PARAM_ABORT(msg) { cout << "MapReduce: missing Skeleton parameter: " << msg << " not set or uncorrectly set" << endl; exit(-1); }

#define TARGS RecordContentType, K1, V1, K2, V2

#define FILE_READER_TIMING
#define TASK_GEN_TIMING
#define THREAD_POOL_TIMING
#define MAP_INIT_TIMING
#define MAP_TIMING
#define SORT_TIMING
#define REDUCE_INIT_TIMING
#define REDUCE_TIMING
#define TOTAL_TIMING

#define REPORT_HEADER "NUM_RECORD,NUM_THREAD,FILE_READING_TIME,THREAD_POOL_INIT_TIME,MAP_TASK_GEN_TIME,MAP_INIT_TIME,MAPPERS_TIME,INTERM_KEYS_MAPPER_AVGNUM,LOCAL_SORT_AVGTIME,SORT_TIME,INTERM_KEYS_NUM,REDUCE_TASK_GEN_TIME,REDUCE_INIT_TIME,REDUCERS_TIME,TOTAL_TIME"

using namespace std;

/**
 * \file MapReduce.hpp
 * \class MapReduce
 * 
 * \brief Class representing the entity which must be declared by the user in order to use the MapReduce Skeleton.
 * 		  Different parameters can be set to change the behaviour of the Skeleton.
 **/
template
    <
    typename RecordContentType,  // type representing the content of the Records
    typename K1,                 // user-defined map key input type
    typename V1,                 // user.defined map value input type
    typename K2,                 // user-defined map key output type (in-List) && user reduce key input type
    typename V2                  // user-defined map value output type (in-List) && user reduce value input/output type (in-List)
    >
class MapReduce
{
    private:
        const string dir_files_path;                                            					// File System's path of the directory containing the input file(s) to process

        const unsigned int thread_num;                                          					// number of thread to use
        unsigned int onln_proc_num;                                         						// number of online processors (architeture-dependant)
		
        RecordFormat<RecordContentType>* record_format;												// pointer to a format which indicates how to extract Record from the input file(s)
        KeyValueFormat<RecordContentType, K1, V1>* keyvalue_format;									// pointer to a format which indicates how to extract (K1, V1) from a Record	
		vector<Record<RecordContentType>> record_vector;            								// vector of extracted Records
        
        function<int(const K2&, const K2&)> compare_fun;											// user-defined intermediate pair's key comparison function
        function<void(const K1&, const V1&, ResultsWrapper<pair<K2, V2>>&)> usr_map;				// user-defined map function
        function<void(const K2&, const list<V2>&, ResultsWrapper<pair<K2, V2>>&)> usr_reduce;		// user-defined reduce function
		
		ThreadManager thread_manager;																// object managing a pool of threads performing active wait on a processor
		Scheduler scheduler;																		// object managing the affinity/pinning of the threads on the processors
		
		// fields for Skeleton's blocks timing
		float file_read_time = 0.0;
		float thread_pool_time = 0.0;
		float map_task_gen_time = 0.0;
		float map_init_phase_time = 0.0;
		float map_phase_time = 0.0;		
		float sorting_time = 0.0;
		float reduce_task_gen_time = 0.0;
		float reduce_init_phase_time = 0.0;
		float reduce_phase_time = 0.0;
		float tot_time = 0.0;
		
		#ifdef REPORT
	
		// fields for intermediate keys counting
		vector<unsigned int> num_interm_keys_mappers;
		unsigned int interm_key_num = 0;
		
		// fields for Mapper-local sorting timing
		vector<float> local_mapper_sorting_time;
		
		#endif
		
    // make available MapReduce's private members to Master classes
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    friend class Master;

    // make available MapReduce's private members to Mapper classes
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    friend class Mapper;

    // make available MapReduce's private members to Reducer classes
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    friend class Reducer;

    // make available MapReduce's private members to LogarithmicReducer classes
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    friend class FinalReducer;
            
    public:
        /**
          * \brief MapReduce Skeleton constructor.
          *  
          * \param _dir_files_path  the File System absolute path of the directory which contains the input file(s)
          * 					    from which extract the Records to process using the MapReduce Skeleton
          * \param _thread_num 	    the number of thread to use
          * \param _record_format   the format that will be used to extract Records from the input file(s)
          * \param _keyvalue_format the format that will be used to extract the initial (K1, V1) from each Record
          * \param _compare_fun     the user defined comparison function that will be used to compare K2s
          * \param _usr_map			the user defined map function
          * \param _usr_reduce		the user defined reduce function
         **/
        MapReduce(
				  const string& _dir_files_path, 
				  const unsigned int _thread_num, 
				  RecordFormat<RecordContentType>* _record_format,
				  KeyValueFormat<RecordContentType, K1, V1>* _keyvalue_format,
				  function<int(const K2&, const K2&)> _compare_fun, 
				  function<void(const K1&, const V1&, ResultsWrapper<pair<K2, V2>>&)> _usr_map,
				  function<void(const K2&, const list<V2>&, ResultsWrapper<pair<K2, V2>>&)> _usr_reduce
				 )
					: dir_files_path(_dir_files_path), thread_num(_thread_num), 
							record_format(_record_format), keyvalue_format(_keyvalue_format), 
								compare_fun(_compare_fun), usr_map(_usr_map), usr_reduce(_usr_reduce) { }
        
        /**
          * \brief Starts the MapReduce skeleton
         **/
        bool run();

    private:
        /**
          * \brief Checks Skeleton's parameters validity
         **/
        inline void check_parameters_validity();
        
        #ifdef CHECK
        
        inline void check_tasks(vector<Task>& tasks);
        
        inline void check_v_pairs(vector<pair<K2, V2>>& pairs_v);

        inline void check_v_pairs(ResultsWrapper<pair<K2, V2>>& pairs_v);
		
        inline void check_vv_pairs(vector<ResultsWrapper<pair<K2, V2>>>& pairs_vv);
        
		#endif
		
		#ifdef REPORT
		
        inline string generate_report_string();
        
        #endif
};

#include "MapReduce.tpp"

#endif /* MAPREDUCE_HPP_ */
