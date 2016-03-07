#ifndef MAPPER_HPP_
#define MAPPER_HPP_

#include <algorithm>
#include <list>
#include <condition_variable>
#include <mutex>
#include "Executable.hpp"
#include "Synchronizable.hpp"
#include "Task.hpp"
#include "Scheduler.hpp"
#include "Timing.hpp"
#include "ResultsWrapper.hpp"

#define TARGS RecordContentType, K1, V1, K2, V2

#define INIT_KEY_EXTRACT_TIMING
#define USR_MAP_TIMING
#define LOCAL_SORT_TIMING
#define INTERM_KEYS_COUNTING

// FORWARD-DECLARATION of class "MapReduce" to resolve circular include
template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
class MapReduce;

// FORWARD-DECLARATION of class "Master" to resolve circular include
template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
class Master;

using namespace std;

/**
 * \file Mapper.hpp
 * \class Mapper
 * 
 * \brief Mapper entity which is able to perform the map process
 **/
template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
class Mapper : public Executable, public Synchronizable<RecordContentType, K1, V1, K2, V2>
{
	private:
		ResultsWrapper<pair<K2, V2>>& local_interm_pairs;	// Mapper-local map wrapper for results
		
		// superclass name usage
		using Synchronizable<TARGS>::master;
		using Synchronizable<TARGS>::synch_id;
		
    public:
    	/**
		 * \brief Default constructor
		 * 
		 * \param _master reference to the Master created the Mapper
		 * \param _mapper_id unique ID for the Mapper
		 * \param _local_interm_pairs Mapper-local map wrapper for result
		 **/
		Mapper(Master<TARGS>& _master, const unsigned int _mapper_id, ResultsWrapper<pair<K2, V2>>& _local_interm_pairs) 
			: Synchronizable<TARGS>(_mapper_id, _master), local_interm_pairs(_local_interm_pairs)
		{
		}
		
		/**
		 * \brief apply the map function
		 **/
		void execute();
};

#include "Mapper.tpp"

#endif /* MAPPER_HPP_ */
