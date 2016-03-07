#include "ActiveWaitThread.hpp"
		
thread::id ActiveWaitThread::get_id()
{
	return aw_thread.get_id(); 
}
		
void ActiveWaitThread::assign_job(Executable* _executable)
{
	status_mutex.lock();
	
	if (!idle)
		cerr << "Impossible to assing job to Thread ID=" << get_id() << " because it is not idle ..." << endl;
	else
	{
		// set Executable job to execute
		executable = _executable;
	
		// set status as "busy" (not idle)
		idle = false;
	}
	
	status_mutex.unlock();
}

bool ActiveWaitThread::is_started()
{
	bool pvt_started;	
	LOCK_UNLOCK(pvt_started = started)
	
	return pvt_started;
}

bool ActiveWaitThread::is_idle()
{
	bool pvt_idle;
	LOCK_UNLOCK(pvt_idle = idle)
	
	return pvt_idle;
}

void ActiveWaitThread::start(unsigned short proc_id)
{	
	// set the affinity of the thread in order to run it on the processor
	// identidied by the passed parameter
	if (!Scheduler::set_thread_affinity(proc_id))
		return;
		   
	// private status variables
	bool pvt_started, pvt_idle;

	// soon after starting the status is "started and idle"
	LOCK_UNLOCK(started = true; pvt_started = started; idle = true; pvt_idle = idle)
	
	// until the private flag "started" is true
	while (pvt_started)
	{
		// update private "idle" status variable
		LOCK_UNLOCK(pvt_idle = idle)
		
		// if the thread is idle
		if (pvt_idle)
		{
			// update private "started" variable and reloop
			LOCK_UNLOCK(pvt_started = started)
		}
		else
		{
			// if the thread is not idle, execute Executable job
			executable->execute();
					
			// delete Executable object
			delete executable;
			
			// set status to "idle" (not busy, reassignable), update
			// private "started" variable and reloop
			LOCK_UNLOCK(idle = true; pvt_started = started)
		}
	}
}

void ActiveWaitThread::stop()
{
	// set the status to "idle and stopped (not started)"
	LOCK_UNLOCK(idle = true; started = false)
	
	// join the std::thread
	aw_thread.join();
}	
