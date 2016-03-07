#ifndef RECORD_FORMAT_HPP_
#define RECORD_FORMAT_HPP_

#include <fstream>
#include "Record.hpp"

/**
 * \file RecordFormat.hpp
 * \class RecordFormat
 * 
 * \brief Class representing an entity able to extract record from a file located into the File System
 **/
template<typename RecordContentType>
class RecordFormat
{
	public:
		virtual Record<RecordContentType>* extract_record(std::ifstream* file) = 0;
};

#endif /* RECORD_FORMAT_HPP_ */
