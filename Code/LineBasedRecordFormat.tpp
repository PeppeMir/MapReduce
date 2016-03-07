/**
 * \file LineBasedRecordFormat.tpp
 * \brief implementation of the methods of the class \p LineBasedRecordFormat present into the file \link LineBasedRecordFormat.hpp
 **/
 
template<typename RecordContentType>
Record<RecordContentType>* LineBasedRecordFormat<RecordContentType>::extract_record(std::ifstream* file)
{
	// read a line from the file
    std::string line;
    getline(*file, line);
    
    // skip empty lines
    if (line.compare("") == 0)
        return nullptr;
    
    // convert the string "line" into the right type
    std::stringstream ss(line);
    RecordContentType casted_line;
    ss >> casted_line;
    
    return new Record<RecordContentType>(casted_line, line.size());
}
