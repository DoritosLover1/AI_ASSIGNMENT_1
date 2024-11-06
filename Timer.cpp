#include "Timer.h"
#include <iostream>;
#include <string>

std::string Timer::endTimer()
{
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
;
	std::cout << "Taken time: " << duration.count() << " sn" << std::endl;
	return std::to_string(duration.count());
}
