#ifndef ACTIVE_WAIT_THREAD_HPP_
#define ACTIVE_WAIT_THREAD_HPP_

#include <mutex>
#include <thread>
#include <iostream>
#include "Executable.hpp"
#include "Scheduler.hpp"

#define LOCK_UNLOCK(code) { status_mutex.lock(); code; status_mutex.unlock(); } 

using namespace std;

/**
 * \file ActiveWaitThread.hpp
 * \class ActiveWaitThread
 * 
 * \brief wrapper for a std::thread able to perform active wait
 **/
class ActiveWaitThread
{
	private:
		bool started = false;			// status flag which indicates if the thread is started
		bool idle = false;				// status flag which indicates if the thread is idle (if it is not executing a job)
		mutex status_mutex;				// mutex used to protect accessed to the status flags of the class
		thread aw_thread;				// wrapped std::thread
		Executable* executable;			// executable job (to set from the extern in order to assign a new job)
		
	public:
		/**
		 * \brief Default constructor. It starts the active wait loop for the wrapped thread, which in turn waits
		 * 		  for a new job.
		 **/
		ActiveWaitThread(unsigned short proc_id) : aw_thread(&ActiveWaitThread::start, this, proc_id)
		{
			// start the std::thread in the constructor
		}
		
		/**
		 * \return the unique identifier of the thread
		 **/
		thread::id get_id();
		
		/**
		 * \brief assign the passed job to the thread
		 **/
		void assign_job(Executable* _executable);
		
		/**
		 * \return true if the thread is running; false otherwise.
		 **/
		bool is_started();
		
		/**
		 * \return true if the thread is idle; false otherwise
		 **/
		bool is_idle();
		
		/**
		 * \brief Start the active wait loop on the processor identified by the passed ID
		 * \param proc_id identifier of the processor on which the thread must pin itself
		 **/
		void start(unsigned short proc_id);
		
		/**
		 * \brief stop the active wait loop of the thread
		 **/
		void stop();	
};

#endif	/* ACTIVE_WAIT_THREAD_HPP_ */
