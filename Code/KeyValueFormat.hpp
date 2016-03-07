#ifndef KEY_VALUE_FORMAT_HPP_
#define KEY_VALUE_FORMAT_HPP_

#include <utility>
#include "Record.hpp"

using namespace std;

/**
 * \file KeyValueFormat.hpp
 * \class KeyValueFormat
 * 
 * \brief Class representing the entity which must be extended by the user in order to furnish to the MapReduce Skeleton a method that will be
 * 		  used to extract initial key value pairs.
 **/
template<typename RecordContentType, typename KeyType, typename ValueType>
class KeyValueFormat
{
    public:
        virtual pair<KeyType, ValueType> extract_keyvalue_pair(Record<RecordContentType>* record) = 0;
};

#endif /* KEY_VALUE_FORMAT_HPP_ */
