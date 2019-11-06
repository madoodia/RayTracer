#ifndef MYTIMER_H
#define MYTIMER_H

#include <iostream>
#include <chrono>

struct Timer
{
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration = {};
	Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;

		float ms = duration.count() * 1000.0f;
		std::cout << ms << "ms (" << ms / 1000.0 << " sec / " << ms / 60000.0 << " min)\n";
	}
};

#endif // MYTIMER_H
