#include "Timing.hpp"

void Timing::start_timing()
{
    start_time = high_resolution_clock::now();
}

void Timing::stop_timing()
{
    end_time = high_resolution_clock::now();
}

float Timing::get_nanosecond_time() const
{
	return duration<float, nanoseconds::period>(end_time - start_time).count();
}

float Timing::get_microsecond_time() const
{
	return duration<float, microseconds::period>(end_time - start_time).count();
}

float Timing::get_millisecond_time() const
{
	return duration<float, milliseconds::period>(end_time - start_time).count();
}

float Timing::get_second_time() const
{
	return duration<float, seconds::period>(end_time - start_time).count();
}
