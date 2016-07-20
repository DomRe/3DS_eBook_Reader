// Time.hpp
// Wrapper for std::chrono

#ifndef EBOOK_TIME_HPP_
#define EBOOK_TIME_HPP_

#include <cstdint>

namespace re
{
	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Use the high resolution clock to get the time. Use this to get time difference in loops.
	*/
	static uint64_t NanoTime();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Get the current system time in milliseconds.
	*/
	static uint64_t MillisTime();
}

#endif