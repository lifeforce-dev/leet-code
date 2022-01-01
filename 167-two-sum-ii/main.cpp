#define CATCH_CONFIG_MAIN

#include "Catch2/catch.hpp"
#include "common/Random.h"
#include "common/Timer.h"
#include "common/Log.h"

#include <iostream>
#include <vector>

int FindMatchingSumIndex(const std::vector<int>& numbers, int currentIndex, int target)
{
	int beginIndex = 0;
	int endIndex = numbers.size() - 1;
	int midIndex = 0;

	//[1,2,3,4,4,9,56,90]
	while (beginIndex <= endIndex)
	{
		midIndex = beginIndex + (endIndex - beginIndex) / 2;

		// We're not allowed to use the same element twice.
		int testValue = numbers[currentIndex] + numbers[midIndex];
		if (midIndex != currentIndex
			&& numbers[currentIndex] + numbers[midIndex] == target)
		{
			return midIndex;
		}
		else if (testValue < target)
		{
			beginIndex = midIndex + 1;
		}
		else if (testValue > target)
		{
			endIndex = midIndex - 1;
		}
		// Because the array is sorted, if the current index and the mid index are
		// the same and add to our equal target, it means there MUST be a duplicate value
		// which will be located 1 above mid index.
		else if (midIndex == currentIndex
			&& numbers[currentIndex] + numbers[midIndex] == target)
		{
			return midIndex + 1;
		}
	}

	// Did not find a solution for the current index.
	return -1;
}

std::vector<int> FindMatchingSumBinary(const std::vector<int>& numbers, int target)
{
	// For every value in numbers
	for (int i = 0; i < numbers.size(); ++i)
	{
		// Perform binary search to find the value that adds to target.
		// If not found, move to next number
		int secondIndex = FindMatchingSumIndex(numbers, i, target);
		if (secondIndex != -1)
		{
			// The problem is dumb and uses 1-indexed arrays, so we add 1 to the results.
			return { i + 1, secondIndex + 1 };
		}
	}
	return {};
}

std::vector<int> TwoSumSlow(std::vector<int>& numbers, int target)
{
	// [1, 2, 3, 4, 4, 9, 56, 90]
	for (int i = 0; i < numbers.size(); ++i)
	{
		for (int j = i + 1; j < numbers.size(); j++)
		{
			if (numbers[i] + numbers[j] == target)
			{
				return { i + 1, j + 1 };
			}
		}
	}

	return {};
}

std::vector<int> TwoSum(std::vector<int>& numbers, int target)
{
	// numbers will never have less than 2 elements.
	if (numbers.size() == 2)
	{
		return { 1, 2 };
	}

	std::vector<int> result;
	result = FindMatchingSumBinary(numbers, target);

	//result = TwoSumSlow(numbers, target);

	return result;
}

SCENARIO("Find 2 numbers that add to target")
{
	// Test 1.
	GIVEN("A sorted array with 2 elements, target = -1")
	{
		int target = -1;
		std::vector<int> test = { -1, 0 };
		
		THEN("Vector will have 2 sorted elements and target will be -1")
		{
			REQUIRE(target == -1);
			REQUIRE(test[0] == -1);
			REQUIRE(test[1] == 0);
		}

		WHEN("Get the indecies of two numbers that add to target")
		{
			std::vector<int> result = TwoSum(test, target);

			THEN("Result will contain the two indecies.")
			{
				REQUIRE(result[0] == 1);
				REQUIRE(result[1] == 2);
			}
		}
	}

	// Test 2.
	GIVEN("Vector [2,7,11,15], Target = 9")
	{
		int target = 9;
		std::vector<int> test = { 2,7,11,15 };

		THEN("Vector will have 4 sorted elements and target will be 9")
		{
			REQUIRE(target == 9);
			REQUIRE(test.size() == 4);
			REQUIRE(std::is_sorted(std::cbegin(test), std::cend(test)));
		}

		WHEN("Get the indecies of two numbers that add to target")
		{
			std::vector<int> result = TwoSum(test, target);

			THEN("Result will contain the two indecies.")
			{
				REQUIRE(result[0] == 1);
				REQUIRE(result[1] == 2);
			}
		}
	}

	GIVEN("Vector [2,3,4], Target = 6")
	{
		int target =6;
		std::vector<int> test = { 2, 3, 4 };

		THEN("Vector will have 3 sorted elements and target will be 6")
		{
			REQUIRE(target == 6);
			REQUIRE(test.size() == 3);
			REQUIRE(std::is_sorted(std::cbegin(test), std::cend(test)));
		}

		WHEN("Get the indecies of two numbers that add to target")
		{
			std::vector<int> result = TwoSum(test, target);

			THEN("Result will contain the two indecies.")
			{
				REQUIRE(result[0] == 1);
				REQUIRE(result[1] == 3);
			}
		}
	}

	GIVEN("Vector [5,25,75], Target = 100")
	{
		int target = 100;
		std::vector<int> test = { 5, 25, 75 };

		THEN("Vector will have 3 sorted elements and target will be 6")
		{
			REQUIRE(target == 100);
			REQUIRE(test.size() == 3);
			REQUIRE(std::is_sorted(std::cbegin(test), std::cend(test)));
		}

		WHEN("Get the indecies of two numbers that add to target")
		{
			std::vector<int> result = TwoSum(test, target);

			THEN("Result will contain the two indecies.")
			{
				REQUIRE(result[0] == 2);
				REQUIRE(result[1] == 3);
			}
		}
	}

	GIVEN("Vector [1,2,3,4,4,9,56,90], Target = 8")
	{
		int target = 8;
		std::vector<int> test = { 1, 2, 3, 4, 4, 9, 56, 90 };

		THEN("Vector will have 3 sorted elements and target will be 6")
		{
			REQUIRE(target == 8);
			REQUIRE(test.size() == 8);
			REQUIRE(std::is_sorted(std::cbegin(test), std::cend(test)));
		}

		WHEN("Get the indecies of two numbers that add to target")
		{
			std::vector<int> result = TwoSum(test, target);

			THEN("Result will contain the two indecies.")
			{
				REQUIRE(result[0] == 4);
				REQUIRE(result[1] == 5);
			}
		}
	}
}
