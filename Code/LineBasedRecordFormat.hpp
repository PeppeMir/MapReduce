#ifndef LB_RECORD_FORMAT_HPP_
#define LB_RECORD_FORMAT_HPP_

#include <sstream>
#include "RecordFormat.hpp"

/**
 * \file LineBasedRecordFormat.hpp
 * \class LineBasedRecordFormat
 **/
template<typename RecordContentType>
class LineBasedRecordFormat : public RecordFormat<RecordContentType>
{
	public:
		Record<RecordContentType>* extract_record(std::ifstream* file);
};

#include "LineBasedRecordFormat.tpp"

#endif /* LB_RECORD_FORMAT_HPP_ */
