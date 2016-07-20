// Time.hpp
// Wrapper for std::chrono

#include <ctime>
#include <chrono>
#include <algorithm>

#include "Time.hpp"

namespace re
{
	uint64_t NanoTime()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}

	uint64_t MillisTime()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
}