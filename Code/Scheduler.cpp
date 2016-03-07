#include "Scheduler.hpp"

unsigned short Scheduler::get_next_free_processor()
{
	unsigned short proc_id;
	
	#ifdef XEON
		proc_id = Scheduler::xeon_sched[next_proc_idx];
		next_proc_idx = (next_proc_idx + 1) % NUM_PROC_XEON;
	#elif defined(PHI)
		proc_id = Scheduler::phi_sched[next_proc_idx];
		next_proc_idx = (next_proc_idx + 1) % NUM_PROC_PHI;
	#else
		proc_id = next_proc_idx;
		next_proc_idx = (next_proc_idx + 1) % Scheduler::get_onln_processors_number();
	#endif
	
	return proc_id; 
}

void Scheduler::reset_status()
{
	next_proc_idx = 0;
}

unsigned short Scheduler::get_onln_processors_number()
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

bool Scheduler::set_thread_affinity_master(const unsigned int thread_num)
{
	#ifdef PHI
	if (thread_num < 59)
		return Scheduler::set_thread_affinity(Scheduler::phi_sched[thread_num]);
	#elif defined(XEON)
	if (thread_num < 8)
		return Scheduler::set_thread_affinity(Scheduler::xeon_sched[thread_num]);
	#endif
	
	return Scheduler::set_thread_affinity(0);
}

bool Scheduler::set_thread_affinity(const unsigned short proc_id)
{
    // set to zero all the bits of the mask
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);

    // set to one the bit corresponding to the passed core identifier
    CPU_SET(proc_id, &cpuset);

    // get running thread identifier and set affinity for it
    const pthread_t tid = pthread_self();
    if (pthread_setaffinity_np(tid, sizeof(cpu_set_t), &cpuset) == 0)
    {
		#ifdef VERBOSE
		   cout << "Thread ID=" << tid << ": running on ProcID=" << sched_getcpu() << endl;
		#endif
		return true;
	}
	else
	{
		#ifdef VERBOSE
			cerr << "Thread ID=" << tid << ": error during setting affinity on ProcID=" << proc_id << endl;
		#endif
		return false;
	}
}

const unsigned short Scheduler::xeon_sched[NUM_PROC_XEON] = 
	{ 0, 2, 4 ,6 ,8, 10, 12, 14, 
	  1, 3, 5, 7, 9, 11, 13, 15 };
	
const unsigned short Scheduler::phi_sched[NUM_PROC_PHI] =
	{ 1, 5, 9, 13, 17, 21, 25, 29, 33, 37, 41, 45, 49, 53, 57, 61, 65, 69, 73, 77, 81, 85, 89, 93, 97, 101, 105, 109, 113, 117, 121, 125, 129, 133, 137, 141, 145, 149, 153, 157, 161, 165, 169, 173, 177, 181, 185, 189, 193, 197, 201, 205, 209, 213, 217, 221, 225, 229, 233,
	2, 6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58, 62, 66, 70, 74, 78, 82, 86, 90, 94, 98, 102, 106, 110, 114, 118, 122, 126, 130, 134, 138, 142, 146, 150, 154, 158, 162, 166, 170, 174, 178, 182, 186, 190, 194, 198, 202, 206, 210, 214, 218, 222, 226, 230, 234,
	3, 7, 11, 15, 19, 23, 27, 31, 35, 39, 43, 47, 51, 55, 59, 63, 67, 71, 75, 79, 83, 87, 91, 95, 99, 103, 107, 111, 115, 119, 123, 127, 131, 135, 139, 143, 147, 151, 155, 159, 163, 167, 171, 175, 179, 183, 187, 191, 195, 199, 203, 207, 211, 215, 219, 223, 227, 231, 235,
	4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236,
	237, 238, 239, 0 };
