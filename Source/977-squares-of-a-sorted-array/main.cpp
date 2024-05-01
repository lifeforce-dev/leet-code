#include <iostream>
#include <vector>
#include <algorithm>

#include "common/Log.h"
#include "common/Timer.h"
#include "common/Random.h"

namespace {
	std::shared_ptr<spdlog::logger> s_logger;
}

// Optimized, removes need for sort.
std::vector<int> getSortedSquaresOptimized(std::vector<int>& nums)
{
	// r-sorted     sorted
	//           |
	// -4,-3,-2, 0, 1,3,6
	// 16, 9, 4, 0, 2, 9, 36
	// 
	// move pointers inward from either side, dumping the appropriate squared
	// value into the result array. Removes the need to call std::sort

	Common::Timer timer;
	timer.Start();

	int count = nums.size();
	std::vector<int> sortedSquares(count);
	int leftIndex = 0;
	int rightIndex = count - 1;
	int valueToSquare = 0;

	for (int i = count - 1; i >= 0; --i)
	{

		if (abs(nums[leftIndex]) < abs(nums[rightIndex]))
		{
			valueToSquare = nums[rightIndex];
			rightIndex--;
		}
		else
		{
			valueToSquare = nums[leftIndex];
			leftIndex++;
		}

		sortedSquares[i] = valueToSquare * valueToSquare;
	}

	timer.Stop();
	SPDLOG_LOGGER_INFO(s_logger, "sortedSquaresOptimized() Time Elapsed: {}ms",
		timer.GetTotalElapsedMs().count());
	return sortedSquares;
}

// Naive version. Easy to read, probably fast enough.
std::vector<int> getSortedSquares(std::vector<int>& nums)
{
	Common::Timer timer;
	timer.Start();

	std::vector<int> squares;
	squares.reserve(nums.size());

	for (int num : nums)
	{
		squares.push_back(num * num);
	}
	std::sort(std::begin(squares), std::end(squares));

	timer.Stop();
	SPDLOG_LOGGER_INFO(s_logger, "sortedSquares() Time Elapsed: {}ms",
		timer.GetTotalElapsedMs().count());
	return squares;
}

int main()
{
	REGISTER_LOGGER("977. Squares Of A Sorted Array");
	s_logger = Log::Logger("977. Squares Of A Sorted Array");

	Common::Timer timer;
	timer.Start();
	std::vector<int> nums = Common::GetRandomNumberList(5000000, -10000000, 10000000);
	std::sort(std::begin(nums), std::end(nums));
	timer.Stop();

	SPDLOG_LOGGER_INFO(s_logger, "Building random numbers list Time Elapsed: {}ms",
		timer.GetTotalElapsedMs().count());

	std::vector<int> squaredNums = getSortedSquares(nums);
	squaredNums = getSortedSquaresOptimized(nums);

	// Sample metrics with these inputs
	// Building random numbers list Time Elapsed : 438ms
	// getSortedSquares() Time Elapsed           : 395ms
	// getSortedSquaresOptimized() Time Elapsed  :  19ms

	return 0;
}
