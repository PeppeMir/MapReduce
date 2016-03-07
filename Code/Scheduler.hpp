#ifndef SCHEDULER_HPP_
#define SCHEDULER_HPP_

#include <unistd.h>
#include <sched.h>
#include <iostream>
#include "pthread.h"

#define NUM_PROC_XEON 16
#define NUM_PROC_PHI 240

using namespace std;

/**
 * \file Scheduler.hpp
 * \class Scheduler
 * 
 * \brief Class representing the entity which is able to define a thread affinity policy depending on 
 * 		  the targeted architecture.
 **/
class Scheduler
{
	private:
        unsigned short next_proc_idx = 0;        						
        const static unsigned short xeon_sched[NUM_PROC_XEON];
		const static unsigned short phi_sched[NUM_PROC_PHI];
			
	public:
		/**
		 * \return the ID of the next processor on which pin the thread, depending on the targeted architecture
		 **/
		unsigned short get_next_free_processor();
		
		/**
		 * \brief reset the internal status of the Scheduler
		 **/
		void reset_status();
		
		/**
		 * \return the number of online processors on the current machine
		 **/
		static unsigned short get_onln_processors_number();
		
        /**
         * \brief Sets the affinity of the running MASTER thread. Depending on the type of architecture
         * 		  and the passed number of used threads, the thread will be pinned on a different processor.
         * 
         * \param thread_num the number of thread used in order to take the decision
         * \return true if the affinity is successfully set; false otherwise.
        **/
		static bool set_thread_affinity_master(const unsigned int thread_num);

        /**
         * \brief Sets the affinity of the running thread using the core identified passed as argument.
         * 
         * \param proc_id the identifier of the logic processor on which set the running thread affinity
         * \return true if the affinity is successfully set; false otherwise.
        **/
		static bool set_thread_affinity(const unsigned short proc_id);
};

#endif /* SCHEDULER_HPP_ */
