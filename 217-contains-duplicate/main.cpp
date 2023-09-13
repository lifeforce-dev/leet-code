#define CATCH_CONFIG_MAIN

#include "Catch2/catch.hpp"
#include "common/Log.h"
#include "common/Timer.h"

#include <algorithm>
#include <assert.h>
#include <vector>

/*
* Given an integer array nums,
  return true if any value appears at least twice in the array,
  and return false if every element is distinct.

  Constraints:
  1 <= nums.length <= 105
  -109 <= nums[i] <= 109
*/

bool containsDuplicate(std::vector<int32_t>& nums)
{
	std::sort(std::begin(nums), std::end(nums));
	int32_t lastNum = nums[0];
	for (int32_t i = 1; i < nums.size(); ++i)
	{
		if (nums[i] == lastNum)
		{
			return true;
		}

		lastNum = nums[i];
	}

	return false;
}

SCENARIO("Check if an array of numbers contains a duplicate")
{
	GIVEN("A small list of numbers containing a duplicate")
	{

		std::vector<int32_t> nums{ 1, 2, 3, 1 };

		REQUIRE(containsDuplicate(nums));
	}
}
