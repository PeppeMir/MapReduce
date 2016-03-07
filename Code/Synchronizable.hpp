#ifndef SYNCHRONIZABLE_HPP_
#define SYNCHRONIZABLE_HPP_

#include <condition_variable>
#include <mutex>
#include "Master.hpp"

#define TARGS RecordContentType, K1, V1, K2, V2

// FORWARD-DECLARATION of class "Master" to resolve circular include
template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
class Master;

/**
 * \file Synchronizable.hpp
 * \class Synchronizable
 * 
 * \brief Class which represents a Synchronizable entity, so an entity able to synchronize the parallel
 * 		  computation with the Master.
 **/
template<typename RecordContentType, typename K1, typename V1, typename K2, typename V2>
class Synchronizable
{
	protected:
		const unsigned int synch_id;
		Master<TARGS>& master;
		
	public:
		/**
		 * \brief Default constructor
		 **/
		Synchronizable(const unsigned int _synch_id, Master<TARGS>& _master) : synch_id(_synch_id), master(_master) { }
		
		/**
		 * \brief sychronize with the Master
		 * \param notify_num the number of threads to reach before awake the Master
		 **/
		void synchronize_with_master(const unsigned int notify_num)
		{
			// increase the number of joined threads and wake up the Master, if the thread is the last one
			unique_lock<mutex> lock(master.endwork_mutex);
			
			// increase the # of joined thread 
			master.joined_threads++;
			
			// the last thread which executes this code will notify the Master, which is sleeping on the condtion variable
			if (master.joined_threads == notify_num)
				master.endwork_cond_var.notify_one();
		}
};

#endif	/* SYNCHRONIZABLE_HPP_ */
