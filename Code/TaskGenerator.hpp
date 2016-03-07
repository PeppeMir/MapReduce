#ifndef TASK_GENERATOR_HPP_
#define TASK_GENERATOR_HPP_

#include <iostream>
#include <vector>
#include "Record.hpp"
#include "Task.hpp"

using namespace std;

/**
 * \file TaskGenerator.hpp
 * \class TaskGenerator
 * 
 * \brief Class able to generate Task(s) from an input vector
 **/
class TaskGenerator
{
	public:
		/**
		 * \brief generate the specified number of tasks from the specified input vector inserting them into
		 * 		  the specified Task vector
		 * 
		 * \param task_num the number of Task to produce
		 * \param input_size the size of the input vector
		 * \param input_vector the input vector from which extract the Task(s)
		 * \param output_tasks the Task vector in which insert the created Task(s)
		 **/
		template<typename T>
		static void generate_tasks(const unsigned int task_num, const unsigned int input_size, T* input_vector, vector<Task>& output_tasks);
		
		/**
		 * \brief generate the specified number of tasks from the specified input vector inserting them into
		 * 		  the specified Task vector
		 * 
		 * \param task_num the number of Task to produce
		 * \param input_vector the input vector from which extract the Task(s)
		 * \param output_tasks the Task vector in which insert the created Task(s)
		 **/
		template<typename T>
		static void generate_tasks(const unsigned int task_num, vector<T>& input_vector, vector<Task>& output_tasks);
};

#include "TaskGenerator.tpp"

#endif /* TASK_GENERATOR_HPP_ */
