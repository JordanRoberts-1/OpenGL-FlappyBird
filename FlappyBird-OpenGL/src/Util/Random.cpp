#include "Random.h"

std::random_device Random::rd = std::random_device();
std::mt19937 Random::e2 = std::mt19937(std::random_device{} ());
std::uniform_real_distribution<> Random::dist(0, 1);