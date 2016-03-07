#ifndef RECORD_HPP_
#define RECORD_HPP_

/**
 * \file Record.hpp
 * \class Record
 * 
 * \brief entity which represents a Record, so a logical fraction of data extracted from the input file(s)
 **/
template<typename RecordContentType>
class Record
{
	private:
		RecordContentType content;
		long size;

	public:
		/**
		 * Copy constructor
		 **/
		Record(const Record& r) : content(r.content), size(r.size) { }
		
		/**
		 * Move constructor
		 **/
		Record(Record& r) : content(std::move(r.content)), size(r.size) { }
		
		/**
		 * Copy assignment operator
		 **/
		Record& operator=(const Record& r)
		{
			// self-assignment check
			if (this != &r) 
			{ 
				// copy data from other's storage to this storage
				content = r.content;
				size = r.size;
			}
			
			return *this;
		}
		
		/**
		 * Move assignment
		 **/
		Record& operator=(Record&& r)
		{
			// self-assignment check
			if (this != &r)
			{  
				content = std::move(r.content);  // move data
				size = r.size;
			}
			
			return *this;
		}
		
		/**
		 * Default constructor
		 **/
		Record(const RecordContentType _content, const long _size) : content(_content), size(_size)
		{
		}

		RecordContentType get_content() const
		{
			return content;
		}

		long get_size() const
		{
			return size;
		}
};

#endif /* RECORD_HPP_ */
