#pragma once

#include <iostream>
#include <chrono>
#include <thread>


using namespace std::literals::chrono_literals;

class Timer
{
private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;
public:
	Timer() {
		start = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(1s);
	}
	std::string endTimer();
};

