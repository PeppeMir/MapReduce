/**
 * \file TaskGenerator.tpp
 * 
 * \brief implementation of the methods of the class \p TaskGenerator present into the file \link TaskGenerator.hpp
 **/

template<typename T>
void TaskGenerator::generate_tasks(const unsigned int task_num, vector<T>& input_vector, vector<Task>& output_tasks)
{
	TaskGenerator::generate_tasks<T>(task_num, input_vector.size(), input_vector.data(), output_tasks);
}

template<typename T>
void TaskGenerator::generate_tasks(const unsigned int task_num, const unsigned int input_size, T* input_vector, vector<Task>& output_tasks)
{
	// if there is less than an input items per Task to generate, we are unable to create the desired number of Tasks
	if (input_size < task_num)
	{
		cerr << "TaskGenerator: the number of input items to process (" << input_size << ") is less than the desired number of Task to generate (" << task_num << ") ..." << endl;
		return;
	}
		
	// INITIAL number of input items per Task (of course >= "1" because of the previous check)
    const unsigned int items_per_task = input_size / task_num;
    
    // compute how many input items has been excluded by the previous division (of course <= "task_num")
	unsigned int residual_num = input_size - (task_num * items_per_task);
	
	#ifdef VERBOSE
		cout << "TaskGenerator: #InputItem=" << input_size << " #DesiredTask=" << task_num << " #AvgItems-per-Task=" << items_per_task << " ..." << endl;
	#endif
    
    // generate Tasks and insert them into the output vector
	output_tasks.reserve(task_num);
    unsigned int start_idx = 0, end_idx;
    for (unsigned int i = 0; i < task_num; i++)
    {
        // set current Task's end index
		end_idx = start_idx + items_per_task - 1;
		
		// if there are still residual items to assign
		if (residual_num > 0)
		{
			end_idx++;
			residual_num--;
		}

		// create a Task using above indexes and insert it into the output Tasks vector
		output_tasks.push_back(Task(start_idx, end_idx, end_idx - start_idx + 1));
		
		#ifdef VERBOSE
			cout << "TaskGenerator: created Task ";
			output_tasks[i].print();
        #endif 
        
        // update current index
        start_idx = end_idx + 1;
    }
}
