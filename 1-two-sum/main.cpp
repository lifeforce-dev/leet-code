#define CATCH_CONFIG_MAIN

#include "Catch2/catch.hpp"
#include "common/Timer.h"
#include "common/Log.h"

#include <vector>

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

std::vector<int> twoSum(const std::vector<int>& nums, int target)
{
    return {};
}

bool isValid(const std::vector<int>& values, int target)
{
    return values[0] + values[1] == target;
}

SCENARIO("Find 2 numbers that add to target")
{
    std::vector<int> result(2);
    GIVEN("even number inputs")
    {

        WHEN("Searching for target indices")
        {
            REQUIRE(true);
        }
    }
}