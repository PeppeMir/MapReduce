#ifndef REC_SELF_CONT_KEYVALUEFORMAT_HPP_
#define REC_SELF_CONT_KEYVALUEFORMAT_HPP_

#include "KeyValueFormat.hpp"

using namespace std;

/**
 * \file RecordSelfContentKeyValueFormat.hpp
 * \class RecordSelfContentKeyValueFormat
 * 
 * \brief Class representing the entity which furnish to the MapReduce Skeleton a method that will be
 * 		  used to extract initial key value pairs simply considering the content of the Record.
 **/
template<typename RecordContentType>
class RecordSelfContentKeyValueFormat : public KeyValueFormat<RecordContentType, int, RecordContentType>
{
    public:
        pair<int, RecordContentType> extract_keyvalue_pair(Record<RecordContentType>* record);
};

#include "RecordSelfContentKeyValueFormat.tpp"

#endif /* REC_SELF_CONT_KEYVALUEFORMAT_HPP_ */
