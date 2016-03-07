#ifndef MASTER_HPP_
#define MASTER_HPP_

#include <iostream>
#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <list>

#include "Record.hpp"
#include "ThreadManager.hpp"
#include "Scheduler.hpp"
#include "Mapper.hpp"
#include "Sorter.hpp"
#include "Reducer.hpp"
#include "FinalReducer.hpp"
#include "ResultsWrapper.hpp"

#define TARGS RecordContentType, K1, V1, K2, V2

using namespace std;

/**
 * \file Master.hpp
 * \class Master
 * 
 * \brief master entity which is able to orchestrate the Map and Reduce phases
 **/
template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
class Master
{
	private:
		MapReduce<TARGS>& skeleton;					// pointer to the MapReduce skeleton
					
		unsigned int joined_threads; 				// number of joined threads (Mappers/Reducers synchronization phase)
		mutex endwork_mutex;						// mutex to use to protect accesses to "joined_threads"
		condition_variable endwork_cond_var;		// condition variable associated to the above mutex, used by the Master to "sleep" until all the Mappers/Reducers have finished their work
		pair<K2, V2>* sorted_interm_pairs_vector;	// pointer to the vector of sorted intermediate key-value pairs
		
		unsigned int mappers_num;					// number of used Mappers
		vector<Mapper<TARGS>*> mappers;				// vector of used Mappers
		
		unsigned int reducers_num;					// number of used Reducers
		vector<Reducer<TARGS>*> reducers;			// vector of used Reducers
		
		Task* map_tasks;							// vector of map-Tasks (C-style)
		Task* reduce_tasks;							// vector of reduce-Tasks (C-style)
		
	// make available Master's private members to Mapper classes
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    friend class Mapper;
	
	// make available Master's private members to Reducer classes
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    friend class Reducer;

	// make available Master's private members to FinalReducer classes
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    friend class FinalReducer;
        
    // make available Master's private members to Reducer classes
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    friend class Synchronizable;
        		
    public:
		/**
		 * \brief Default constructor
		 * \param reference to the skeleton which declared the Master
		 **/
		Master(MapReduce<TARGS>& _skeleton) : skeleton(_skeleton)
		{
			#ifdef VERBOSE
				cout << "Master: running on ProcID=" << sched_getcpu() << " ..." << endl;
			#endif
		}
		
		/**
		 * \brief initialize the Map phase
		 * 
		 * \param _map_tasks the vector of map-Task(s) to assign to the Mappers
		 * \param _interm_pairs the vector of ResultsWrapper(s) representing local results container for each Mapper
		 **/
		inline void init_map_phase(vector<Task>& _map_tasks, vector<ResultsWrapper<pair<K2, V2>>>& _interm_pairs);
		
		/**
		 * \brief start the Map phase
		 **/
		inline void start_map_phase();

		/**
		 * \brief merge and sort the output of the Map phase in order to make it available for the Reduce phase
		 * 
		 * \param interm_pairs the vector of ResultsWrapper(s) containing the results of the Map phase obtained from each Mapper
		 * \param sorted_interm_pairs_vector the vector to fill with the sorted intermediate key-value pairs
		 **/
        inline void merge_and_sort(vector<ResultsWrapper<pair<K2, V2>>>& interm_pairs, vector<pair<K2, V2>>& sorted_interm_pairs_vector);

		/**
		 * \brief initialize the Reduce phase
		 * 
		 * \param _reduce_tasks the vector of reduce-Task(s) to assign to the Reducers
		 * \param _sorted_interm_pairs_vector the vector of sorted intermediate key-value pairs
		 * \param _reduce_results the vector of ResultsWrapper(s) representing local results container for each Reducer
		 **/
		inline void init_reduce_phase(vector<Task>& _reduce_tasks, vector<pair<K2, V2>>& _sorted_interm_pairs_vector, vector<ResultsWrapper<pair<K2, V2>>>& _reduce_results);

		/**
		 * \brief start the Reduce phase
		 **/
		inline void start_reduce_phase(vector<ResultsWrapper<pair<K2, V2>>>& reduce_results, ResultsWrapper<pair<K2, V2>>& final_result);

	private:		
		/**
		 * \brief wait for the synchronization with Mappers or Reducers threads
		 **/
		void wait_for_synchronization(const unsigned int synch_num);
};

#include "Master.tpp"

#endif /* MASTER_HPP_ */
