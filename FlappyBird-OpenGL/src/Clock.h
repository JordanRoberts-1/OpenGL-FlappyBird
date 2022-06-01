#pragma once
#include <chrono>

static class Clock
{
public:
	static double CurrTimeInMillis()
	{
		auto now = std::chrono::high_resolution_clock::now();
		double ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now).time_since_epoch().count();
		return ms;
	}
};
