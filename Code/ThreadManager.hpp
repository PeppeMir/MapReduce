#ifndef THREAD_MANAGER_HPP_
#define THREAD_MANAGER_HPP_

#include <iostream>
#include <vector>
#include "ActiveWaitThread.hpp"
#include "Executable.hpp"
#include "Scheduler.hpp"

using namespace std;

/**
 * \file ThreadManager.hpp
 * \class ThreadManager
 * 
 * \brief Class implementing a ThreadPool library for \ref ActiveWaitThreads.
 **/
class ThreadManager
{
	private:
		bool initialized = false;
		unsigned int thread_num;
		unsigned int next_thread_idx;
		vector<ActiveWaitThread*> aw_threads_vec;

	public:
		/**
		 * \brief initialize the ThreaPool using the specified number of threads and Scheduler
		 * \param _thread_num the number of ActiveWaitThread(s) to setup for the pool
		 * \param scheduler the scheduler to use for the ActiveWaitThread processors-pinning
		 **/
		void init(const unsigned int _thread_num, Scheduler& scheduler);
		
		/**
		 * \brief execute the passed Executable task on the next idle ActiveWaitThread
		 * \param executable the task to execute
		 **/
		void execute_on_idle_thread(Executable* executable);
		
		/**
		 * \return the number of ActiveWaitThread(s) initialized into the ThreadPool
		 **/
		unsigned int num_ready_threads();
		
		/**
		 * \brief wait until all the ActiveWaitThread of the ThreadPool will be idle (until they are excuting their work)
		 **/
		void wait_all_idle();
		
		/**
		 * \brief destroy the internal status of the ThreadPool stopping the ActiveWaitThread(s).
		 * 		  The method waits until an ActiveWaitThread is idle before stopping it.
		 **/
		void shut_down();
};	

#endif /* THREAD_MANAGER_HPP_ */
