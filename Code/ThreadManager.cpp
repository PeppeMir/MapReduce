#include "ThreadManager.hpp"

void ThreadManager::init(const unsigned int _thread_num, Scheduler& scheduler)
{
	// skip if it is already initialized
	if (initialized)
		return;
		
	// set the number of thread used by the pool and reserve the space for the threads
	thread_num = _thread_num;
	aw_threads_vec.reserve(_thread_num);
	
	// start creation of the active-wait threads
	for (unsigned int i = 0; i < _thread_num; i++)
		aw_threads_vec.push_back(new ActiveWaitThread(scheduler.get_next_free_processor()));
	
	// wait until all the active-wait threads are not started
	for (unsigned int i = 0; i < _thread_num; i++)
	{
		while (!aw_threads_vec[i]->is_started() || !aw_threads_vec[i]->is_idle())
			continue;
	}
	
	#ifdef VERBOSE
		cout << "ThreadManager: checkup created thread status:" << endl;
		for (unsigned int i = 0; i < _thread_num; i++)
			cout << "==> Thread ID=" << aw_threads_vec[i]->get_id() << " started=" << aw_threads_vec[i]->is_started() << " idle=" << aw_threads_vec[i]->is_idle() << endl;
		cout << "Checkup done." << endl;
	#endif
	
	// set the next thread index to which assign the next job
	next_thread_idx = 0;
	
	// set the status as "initialized"
	initialized = true;
	
	#ifdef VERBOSE
		cout << "ThreadManager: initialization of " << num_ready_threads() << " thread(s) performed successfully ..." << endl;
	#endif
}

void ThreadManager::execute_on_idle_thread(Executable* executable)
{			
	// return with error if not initialized
	if (!initialized)
	{
		cerr << "ThreadManager: unable to launch the thread because the status has not been initialized ..." << endl;
		return;
	}
	
	#ifdef VERBOSE
		cout << "ThreadManager: try to assign job to thread-index=" << next_thread_idx << " in [0," << aw_threads_vec.size() - 1 << "]" << endl;
	#endif
	
	// wait until the thread is not idle
	while (!aw_threads_vec[next_thread_idx]->is_idle())
		continue;
	
	//cerr << "ThreadManager: unable to execute because no idle threads are available ..." << endl;

	// otherwise assign the Executable job to it
	aw_threads_vec[next_thread_idx]->assign_job(executable);
	
	#ifdef VERBOSE
		cout << "ThreadManager: job assigned to thread-index=" << next_thread_idx << endl;
	#endif
	
	// prepare next thread index
	next_thread_idx = (next_thread_idx + 1) % aw_threads_vec.size();
}

unsigned int ThreadManager::num_ready_threads()
{
	return aw_threads_vec.size();
}
		
void ThreadManager::wait_all_idle()
{
	#ifdef VERBOSE
		cout << "ThreadManager: waiting for still busy threads ..." << endl;
	#endif
	
	bool busy_found = true;
	while (busy_found)
	{
		busy_found = false;
		for (unsigned int i = 0; i < thread_num; i++)
			if (!aw_threads_vec[i]->is_idle())
			{
				busy_found = true;
				break;
			}
	}
	
	// reset the next thread index to intial value
	next_thread_idx = 0;
	
	#ifdef VERBOSE
		cout << "ThreadManager: all threads are now idle ..." << endl;
	#endif
}

void ThreadManager::shut_down()
{
	if (initialized)
	{
		// set status to "not initialized"
		initialized = false;

		// wait until all threads are idle
		wait_all_idle();
		
		#ifdef VERBOSE
			cout << "ThreadManager: stopping all the (idle) threads ..." << endl;
		#endif
		
		// stop and deallocate all the launched active-wait threads
		for (unsigned int i = 0; i < thread_num; i++)
		{
			aw_threads_vec[i]->stop();
			delete aw_threads_vec[i];
		}
	}
}
