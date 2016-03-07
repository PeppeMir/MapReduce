#ifndef RESULTS_WRAPPER_
#define RESULTS_WRAPPER_

#include <vector>

using namespace std;

/**
 * \file ResultsWrapper.hpp
 * \class ResultsWrapper
 * 
 * \brief Class which represents a wrapper for a std::vector
 **/
template<typename T>
class ResultsWrapper
{
	public:
		typedef typename vector<T>::iterator iterator;
		typedef typename vector<T>::reverse_iterator reverse_iterator;
		
	private:
		vector<T> v;	// wrapped std::vector
		
	public:
		/**
		 * Copy constructor
		 **/
		ResultsWrapper(const ResultsWrapper& rw) : v(rw.v) { }
		
		/**
		 * Move constructor
		 **/
		ResultsWrapper(ResultsWrapper&& rw) : v(std::move(rw.v)) { }
		
		/**
		 * Copy assignment operator
		 **/
		ResultsWrapper& operator=(const ResultsWrapper& rw)
		{
			v = rw.v;
			return *this;
		}

		/**
		 * Move assignment
		 **/
		ResultsWrapper& operator=(ResultsWrapper&& rw)
		{
			v = std::move(rw.v);
			return *this;
		}
			
		/**
		 * Default constructor		
		 **/
		ResultsWrapper() { }
		
		/** 
		 * [] operators
		 **/
		T& operator[](const unsigned int index) { return v[index]; };
		const T& operator[](const unsigned int index) const { return v[index]; };
	 	
	 	/**
	 	 * \brief emit a new result into the wrapper
	 	 **/
		void emit(const T& result);
				
		/**
		 * \return the first value contained into the wrapper
		 **/
		T first();
		
		/**
		 * \param index indicating the position of the result to get
		 * \return the index-th result into the wrapper
		 **/
		T at(const unsigned int index);

		/**
		 * \param the last result into the wrapper
		 **/
		T last();
		
		/**
		 * \brief delete the last result from the wrapper
		 **/
		void delete_last();
		
		/**
		 * \return a pointer to the data-area of the wrapper (accessible by "[]")
		 **/
		T* data();
		
		/**
		 * \return the size of the wrapper, i.e. the number of results into the wrapper
		 **/
		unsigned int size();
		
		/**
		 * \return a begin iterator for the wrapper
		 **/
		ResultsWrapper<T>::iterator begin();

		/**
		 * \return an end iterator for the wrapper
		 **/		
		ResultsWrapper<T>::iterator end();
		
		/**
		 * \return a begin reverse iterator for the wrapper
		 **/
		ResultsWrapper<T>::reverse_iterator rbegin();

		/**
		 * \return an end reverse iterator for the wrapper
		 **/		
		ResultsWrapper<T>::reverse_iterator rend();
};

#include "ResultsWrapper.tpp"

#endif	/* RESULTS_WRAPPER_ */
