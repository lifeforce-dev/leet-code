#include <iostream>
#include <vector>
#include <algorithm>

#include "common/Log.h"
#include "common/Timer.h"

namespace {
	std::shared_ptr<spdlog::logger> s_logger;
}

std::vector<int> sortedSquares(std::vector<int>& nums)
{
	std::vector<int> squares;
	squares.reserve(nums.size());

	for (int num : nums)
	{
		squares.push_back(num * num);
	}
	std::sort(std::begin(squares), std::end(squares));
	return squares;
}

int main()
{
	REGISTER_LOGGER("977. Squares Of A Sorted Array");
	s_logger = Log::Logger("977. Squares Of A Sorted Array");

	Common::Timer timer;

	std::vector<int> nums = { -4,-1,0,3,10 };

	timer.Start();
	auto a = sortedSquares(nums);
	timer.Stop();
	SPDLOG_LOGGER_INFO(s_logger, "Time Elapsed: {}",
		timer.GetTotalElapsedNs().count());

	return 0;
}
