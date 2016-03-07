#ifndef FINAL_REDUCER_HPP_
#define FINAL_REDUCER_HPP_

#include <vector>
#include "Executable.hpp"
#include "Synchronizable.hpp"

using namespace std;

/**
 * \file FinalReducer.hpp
 * \class FinalReducer
 * 
 * \brief FinalReducer entity which is able to perform the FINAL reduce process
 **/
template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
class FinalReducer : public Executable, public Synchronizable<RecordContentType, K1, V1, K2, V2>
{
	private:
		vector<ResultsWrapper<pair<K2, V2>>>& blocks;	// vector of ResultsWrapper(s) to process into the final reduce
		ResultsWrapper<pair<K2, V2>>& final_result;		// wrapper for final reduce results
		
		// superclass name usage
		using Synchronizable<TARGS>::master;
		
	public:
		/**
		 * \brief Default constructor
		 * 
		 * \param _master reference to the Master created the FinalReducer
		 * \param _reducer_id unique ID for the FinalReducer
		 * \param _blocks vector of ResultsWrapper(s) to process into the final reduce
		 * \param _final_result wrapper for final reduce results
		 **/
		FinalReducer(Master<TARGS>& _master, const unsigned int _reducer_id, vector<ResultsWrapper<pair<K2, V2>>>& _blocks, ResultsWrapper<pair<K2, V2>>& _final_result) 
			: Synchronizable<TARGS>(_reducer_id, _master), blocks(_blocks), final_result(_final_result) { }
		
		/**
		 * \brief apply the reduce function
		 **/
		void execute();
		
	private:
		/**
		 * \brief concatenate all the ResultsWrapper(s) into a unique block on which process the final reduce
		 **/
		pair<K2, V2>* concat(unsigned int* concat_size);
};

#include "FinalReducer.tpp"

#endif /* FINAL_REDUCER_HPP_ */
