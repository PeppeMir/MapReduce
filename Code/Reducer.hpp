#ifndef REDUCER_HPP_
#define REDUCER_HPP_

#include <vector>
#include <list>
#include "Task.hpp"
#include "Executable.hpp"
#include "Synchronizable.hpp"
#include "ResultsWrapper.hpp"

#define TARGS RecordContentType, K1, V1, K2, V2

#define USR_REDUCE_TIMING
#define INTERM_KEYS_GROUPS_COUNTING

using namespace std;

/**
 * \file Reducer.hpp
 * \class Reducer
 * 
 * \brief Reducer entity which is able to perform the reduce process
 **/
template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
class Reducer : public Executable, public Synchronizable<RecordContentType, K1, V1, K2, V2>
{
	private:
		ResultsWrapper<pair<K2, V2>>& results;	// Reducer-local reduce wrapper for results
		
		// superclass name usage
		using Synchronizable<TARGS>::master;
		using Synchronizable<TARGS>::synch_id;
		
	public:
		/**
		 * \brief Default constructor
		 * 
		 * \param _master reference to the Master created the Reducer
		 * \param _reducer_id unique ID for the Reducer
		 * \param _results Reducer-local reduce wrapper for result
		 **/
		Reducer(Master<TARGS>& _master, const unsigned int _reducer_id, ResultsWrapper<pair<K2, V2>>& _results)
			: Synchronizable<TARGS>(_reducer_id, _master), results(_results) { }
		
		/**
		 * \brief apply the reduce function
		 **/
		void execute();
};

#include "Reducer.tpp"

#endif /* REDUCER_HPP_ */
