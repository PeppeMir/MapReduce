#ifndef TIMING_HPP_
#define TIMING_HPP_

#include <chrono>

using namespace std::chrono;

/**
 * \file Timing.hpp
 * \class Timing
 * 
 * \brief Class implementing a simple timing library providing different measure units.
 **/
class Timing
{
    private:
        high_resolution_clock::time_point start_time;		// store start time-point
        high_resolution_clock::time_point end_time;			// store end time-point

    public:
		/**
		 * \brief start the timing process
		 **/
        void start_timing();

		/**
		 * \brief stop the timing process
		 **/
        void stop_timing();
		
		/**
		 * \return retrieve the time spent between the last invocation of start_timing() and the last
		 * 		   invocation of stop_timing(), in nanoseconds (nsec)
		 **/
		float get_nanosecond_time() const;

		/**
		 * \return retrieve the time spent between the last invocation of start_timing() and the last
		 * 		   invocation of stop_timing(), in microseconds (usec)
		 **/
        float get_microsecond_time() const;

		/**
		 * \return retrieve the time spent between the last invocation of start_timing() and the last
		 * 		   invocation of stop_timing(), in milliseconds (msec)
		 **/
        float get_millisecond_time() const;
        
		/**
		 * \return retrieve the time spent between the last invocation of start_timing() and the last
		 * 		   invocation of stop_timing(), in seconds (sec)
		 **/
        float get_second_time() const;
};

#endif /* TIMING_HPP_ */
