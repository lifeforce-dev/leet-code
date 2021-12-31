//---------------------------------------------------------------
//
// Log.cpp
//

#include "common/Random.h"

#include <random>

namespace {
	std::random_device s_rd;
	std::mt19937_64 s_mt(s_rd());
}

namespace Common {

//===============================================================================

std::vector<int> GetRandomNumberList(int count, int lowerBound, int upperBound)
{
	std::vector<int> ints;
	ints.reserve(count);

	for (int i = 0; i < count; ++i)
	{
		ints.push_back(GetRandomNumber(lowerBound, upperBound));
	}

	return ints;
}

int GetRandomNumber(int lowerBound, int upperBound)
{
	std::uniform_int_distribution<int> dist(lowerBound, upperBound);
	return dist(s_mt);
}

//===============================================================================

} // namespace Common

