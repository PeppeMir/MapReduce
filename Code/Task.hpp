#ifndef TASK_HPP_
#define TASK_HPP_

#include <iostream>

using namespace std;

/**
 * \file Task.hpp
 * \class Task
 * 
 * \brief Class which represents a Task, so a logical splits of data bounded by indexes and having a size
 **/
class Task
{
    private:
        unsigned int start_idx;
        unsigned int end_idx;
        unsigned long sz;

    public:
		/**
		 * Copy constructor
		 **/
		Task(const Task& t) : start_idx(t.start_idx), end_idx(t.end_idx), sz(t.sz) { }
		
		/**
		 * Move constructor
		 **/
		Task(Task& t) : start_idx(t.start_idx), end_idx(t.end_idx), sz(t.sz) { }
		
		/**
		 * Copy assignment operator
		 **/
		Task& operator=(const Task& t)
		{
			// self-assignment check expected
			if (this != &t) 
			{ 
				// copy data from other's storage to this storage
				start_idx = t.start_idx;
				end_idx = t.end_idx;
				sz = t.sz;
			}
			
			return *this;
		}

		/**
		 * Move assignment
		 **/
		Task& operator=(Task&& t)
		{
			// self-assignment check
			if (this != &t)
			{  
				// copy data from other's storage to this storage
				start_idx = t.start_idx;
				end_idx = t.end_idx;
				sz = t.sz;
			}
			
			return *this;
		}
		
		/**
		 * Default constructor
		 **/
        Task(const unsigned int _startIndex, const unsigned int _endIndex, const unsigned long _size) 
			: start_idx(_startIndex), end_idx(_endIndex), sz(_size) { }
		
		/**
		 * \return the start index of the Task
		 **/
        unsigned int start_index() const
        {
			return start_idx;
		}

		/**
		 * \return the end index of the Task
		 **/
        unsigned int end_index() const
        {
			return end_idx;
		}

		/**
		 * \return the size of the Task
		 **/
        unsigned long size() const
        {
			return sz;
		}

		/**
		 * \brief print the content of the Task
		 **/
        void print() const
        {
			cout << "(start_idx=" << start_idx << ",end_idx=" << end_idx << ",size=" << sz << ")" << endl;
		}
};

#endif /* TASK_HPP_ */
