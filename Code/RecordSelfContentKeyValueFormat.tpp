/**
 * \file RecordSelfContentKeyValueFormat.tpp
 * \brief implementation of the methods of the class RecordSelfContentKeyValueFormat present into the file \link RecordSelfContentKeyValueFormat.hpp
 **/
 
template<typename RecordContentType>
pair<int, RecordContentType> RecordSelfContentKeyValueFormat<RecordContentType>::extract_keyvalue_pair(Record<RecordContentType>* record)
{
    return pair<int, RecordContentType>(-1, record->get_content());
}

