#pragma once
#include <random>

class Random
{
public:
	static float Random01()
	{
		return (float)dist(e2);
	}

	static int Random0n(int n)
	{
		std::uniform_int_distribution<> distribution(0, n);
		return distribution(e2);
	}

private:
	static std::random_device rd;
	static std::mt19937 e2;
	static std::uniform_real_distribution<> dist;
};

