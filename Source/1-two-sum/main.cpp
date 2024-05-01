#define CATCH_CONFIG_MAIN

#include "Catch2/catch.hpp"
#include "common/Timer.h"
#include "common/Log.h"
#include "common/Random.h"

#include <algorithm>
#include <assert.h>
#include <vector>

/*
Strategy:
	Brute Force: Simply use a nested loop to check each value to see if they add up to target

	Optimized: Use a map to store each value of the list of numbers associated with their index.
	           This mapping will represent the compliment of A. Then, iterate over each value in
			   the list again. Each value represents A. Then we subtract A from the target which reveals the
			   compliment. Test if the compliment exists in our map, if it does then get its index. Then just
			   return the two indices.
*/

/*
Description:
	Given an array of integers nums and an integer target, return indices of the two numbers such that they add up to target.

	You may assume that each input would have exactly one solution, and you may not use the same element twice.
	You can return the answer in any order.

Example 1:
	Input: nums = [2,7,11,15], target = 9
	Output: [0,1]
	Explanation: Because nums[0] + nums[1] == 9, we return [0, 1].

Example 2:
	Input: nums = [3,2,4], target = 6
	Output: [1,2]

Example 3:
	Input: nums = [3,3], target = 6
	Output: [0,1]
*/

// O(n^2) algorithm
std::vector<int32_t> twoSumBruteForce(const std::vector<int32_t>& nums, int32_t target)
{
	if (nums.empty())
	{
		return {};
	}

	for (int32_t aIdx = 0; aIdx < nums.size(); ++aIdx)
	{
		for (int32_t bIdx = aIdx+1; bIdx < nums.size(); ++bIdx)
		{
			if (nums[aIdx] + nums[bIdx] == target)
			{
				return { aIdx, bIdx };
			}
		}
	}

	return {};
}

// 0(n)
std::vector<int> twoSumOptimized(const std::vector<int32_t>& nums, int32_t target)
{
	std::unordered_map<int32_t, int32_t> complimentsMap;
	complimentsMap.reserve(nums.size());

	// To find the two indices we need A + B == target. We're given a list of potential compliments.
	// Record all of the potential compliments so when we test part A, we can easily locate its compliment.
	for (int32_t i = 0; i < nums.size(); ++i)
	{
		complimentsMap[nums[i]] = i;
	}

	for (int32_t i = 0; i < nums.size(); ++i)
	{
		// Find compliment by subtracting a, and checking if that compliment exists in the map.
		int32_t a = nums[i];
		int32_t compliment = target - a;
		if (auto it = complimentsMap.find(compliment) != complimentsMap.end())
		{
			int32_t aIdx = complimentsMap.at(compliment);
			// We can't use the same element.
			if (aIdx != i)
			{
				return { i , complimentsMap.at(compliment) };
			}
		}
	}

	return {};
}

bool isValid(const std::vector<int32_t>& testIndices, const std::vector<int32_t>& targetIndices)
{
	return std::is_permutation(std::cbegin(targetIndices),
		std::cend(targetIndices), std::cbegin(testIndices));
}

SCENARIO("Find 2 numbers that add to target")
{
	std::vector<int32_t> result(2);
	GIVEN("even count inputs with 1 target on outter edge")
	{
		std::vector<int32_t> nums{ 2, 6, 4, 10 };


		WHEN("Searching for target indices")
		{
			result = twoSumOptimized(nums, 16);
			REQUIRE(isValid(result, {1, 3}));
		}
	}

	GIVEN("even count inputs with 2 targets on the edges")
	{
		std::vector<int32_t> nums{ 2, 6, 4, 10 };
		WHEN("Searching for target indices")
		{
			result = twoSumOptimized(nums, 12);
			REQUIRE(isValid(result, { 0, 3 }));
		}
	}

	GIVEN("even count inputs with targets in the middle")
	{
		std::vector<int32_t> nums{ 2, 6, 4, 10 };
		WHEN("Searching for target indices")
		{
			result = twoSumOptimized(nums, 10);
			REQUIRE(isValid(result, { 1, 2 }));
		}
	}

	GIVEN("even count inputs with 1 target on inner edge")
	{
		std::vector<int32_t> nums{ 2, 6, 4, 5, 9, 7, 12, 10 };
		WHEN("Searching for target indices")
		{
			result = twoSumOptimized(nums, 7);
			REQUIRE(isValid(result, { 0, 3 }));
		}
	}

	GIVEN("even count inputs with first 2 indices as target")
	{
		std::vector<int32_t> nums{ 2, 6, 4, 5, 9, 7, 12, 10 };
		WHEN("Searching for target indices")
		{
			result = twoSumOptimized(nums, 8);
			REQUIRE(isValid(result, { 0, 1 }));
		}
	}

	GIVEN("even count inputs with last 2 indices as target")
	{
		std::vector<int32_t> nums{ 2, 6, 4, 5, 9, 7, 12, 10 };
		WHEN("Searching for target indices")
		{
			result = twoSumOptimized(nums, 22);
			REQUIRE(isValid(result, { 6, 7 }));
		}
	}

	GIVEN("odd count inputs with 1 target on outter edge")
	{
		std::vector<int32_t> nums{ 2, 6, 4, 10, 2 };
		WHEN("Searching for target indices")
		{
			result = twoSumOptimized(nums, 16);
			REQUIRE(isValid(result, { 1, 3 }));
		}
	}

	GIVEN("odd count inputs with 2 targets on the edges")
	{
		std::vector<int32_t> nums{ 2, 6, 4, 10, 5 };
		WHEN("Searching for target indices")
		{
			result = twoSumOptimized(nums, 7);
			REQUIRE(isValid(result, { 0, 4 }));
		}
	}

	GIVEN("odd count inputs with targets in the middle")
	{
		std::vector<int32_t> nums{ 2, 6, 4, 10, 3 };
		WHEN("Searching for target indices")
		{
			result = twoSumOptimized(nums, 10);
			REQUIRE(isValid(result, { 1, 2 }));
		}
	}

	GIVEN("odd count inputs with 1 target on inner edge")
	{
		std::vector<int32_t> nums{ 2, 6, 4, 5, 9, 7, 12, 10, 7 };
		WHEN("Searching for target indices")
		{
			result = twoSumOptimized(nums, 7);
			REQUIRE(isValid(result, { 0, 3 }));
		}
	}

	GIVEN("odd count inputs with first 2 indices as target")
	{
		std::vector<int32_t> nums{ 2, 6, 4, 5, 9, 7, 12, 10, 11 };
		WHEN("Searching for target indices")
		{
			result = twoSumOptimized(nums, 8);
			REQUIRE(isValid(result, { 0, 1 }));
		}
	}

	GIVEN("odd count inputs with last 2 indices as target")
	{
		std::vector<int32_t> nums{ 2, 6, 4, 5, 9, 7, 12, 10, 15 };
		WHEN("Searching for target indices")
		{
			result = twoSumOptimized(nums, 25);
			REQUIRE(isValid(result, { 7, 8 }));
		}
	}

	GIVEN("odd count inputs with 1 element that's half of target")
	{
		std::vector<int32_t> nums{ 2, 6, 4, 5, 9, 7, 12, 10, 14 };
		WHEN("Searching for target indices")
		{
			result = twoSumOptimized(nums, 24);
			REQUIRE(isValid(result, { 7, 8 }));
		}
	}

	GIVEN("A long list of numbers with targets at end")
	{
		std::vector<int32_t> nums;
		nums.reserve(10000);
		spdlog::info("Generating list...");

		// we're simply testing speed of handling a large data set. So we
		// need to control when we successfully find the 1 and only value.
		for (int i = 0; i < 9998; ++i)
		{
			int number = 0;
			while (number == 0 || number == 1)
			{
				// Choose between two number less than half the max range so that
				// max range is impossible to hit with the random values.
				number = Common::GetRandomNumber(-499999999, 499999999);
			}

			nums.push_back(number);
		}

		// Manually add two numbers that add to the max range, set that as target.
		nums.push_back(500000000);
		nums.push_back(500000000);

		spdlog::info("Done generating...");

		Common::Timer timer;
		timer.Start();
		result = twoSumBruteForce(nums, 1000000000);
		timer.Stop();
		spdlog::info("Time Elapsed: {}ms", timer.GetTotalElapsedMs().count());
		spdlog::info("result={}", fmt::join(result, ","));
		REQUIRE(isValid(result, { 9998, 9999 }));

		timer.Clear();

		timer.Start();
		result = twoSumOptimized(nums, 1000000000);
		timer.Stop();
		spdlog::info("Time Elapsed: {}ms", timer.GetTotalElapsedMs().count());
		spdlog::info("result={}", fmt::join(result, ","));
		REQUIRE(isValid(result, { 9998, 9999 }));
	}
}
