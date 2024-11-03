#include "Timer.h"

void Timer::endTimer()
{
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
;
	std::cout << "Taken time: " << duration.count() << " sn" << std::endl;
}
