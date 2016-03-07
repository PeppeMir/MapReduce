#ifndef EXECUTABLE_HPP_
#define EXECUTABLE_HPP_

/**
 * \file Executable.hpp
 * \class Executable
 * 
 * \brief entity which represents an executable job
 **/
class Executable
{
	public:
		virtual ~Executable() { }
		
		virtual void execute() = 0;
};

#endif	/* EXECUTABLE_HPP_ */
