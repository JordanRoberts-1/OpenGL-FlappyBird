#pragma once
#include <iostream>

class Score
{
public:
	static void IncrementScore() { s_Score++; std::cout << s_Score << std::endl; }
	static void ResetScore() { s_Score = 0; }
	static int GetScore() { return s_Score; }
private:
	static float s_Reward; //float for the learning algorithm
	static int s_Score;
};