#define CATCH_CONFIG_MAIN
#include "Catch2/catch.hpp"

#include <vector>


void MoveZeroes(std::vector<int>& nums)
{
	// Keep track of how many places back to move elements.
	int shiftCount = 0;

	for (int i = 0; i < nums.size(); ++i)
	{
		// Skip over 0s, but record how far back to shift subsequent elements
		if (nums[i] == 0)
		{
			shiftCount++;
		}
		else
		{
			// Overwrite the previous elements.
			nums[i - shiftCount] = nums[i];
		}
	}

	// Place all of our 0s at the back of the array.
	for (int i = nums.size() - shiftCount; i < nums.size(); ++i)
	{
		nums[i] = 0;
	}
}

SCENARIO("Move 0s to back")
{
	GIVEN("Vector [1, 0, 3, 4, 2, 0, 6, 7]")
	{
		std::vector<int> test = { 1, 0, 3, 4, 2, 0, 6, 7 };
		WHEN("Moving 0s to back of array")
		{
			MoveZeroes(test);
			CAPTURE(test);

			THEN("0s are at the back and relative array order is preserved")
			{
				std::vector<int> expected = { 1, 3, 4, 2, 6, 7, 0, 0 };
				REQUIRE(test.size() == expected.size());

				bool isCorrect = true;
				for (int i = 0; i < test.size(); ++i)
				{
					isCorrect &= test[i] == expected[i];
				}
			
				REQUIRE(isCorrect);
			}
		}
	}
}

SCENARIO("One Element")
{
	GIVEN("Vector size 1: [0]")
	{
		std::vector<int> test = { 0 };
		WHEN("Moving 0s to back of array")
		{
			MoveZeroes(test);
			CAPTURE(test);

			THEN("Array return untouched")
			{
				REQUIRE(test.size() == 1);
				REQUIRE(test[0] == 0);
			}
		}
	}

	GIVEN("Vector size 1: [1]")
	{
		std::vector<int> test = { 1 };
		WHEN("Attempting to move non-existent 0s")
		{
			MoveZeroes(test);
			CAPTURE(test);

			THEN("Array return untouched")
			{
				REQUIRE(test.size() == 1);
				REQUIRE(test[0] == 1);
			}
		}
	}
}

SCENARIO("Moving many 0s to back of array")
{
	GIVEN("Vector with many 0s: [0, 1, 2, 3, 4, 5, 6, 0, 7, 8, 0, 0, 0, 0, 0, 9, 10, 11, 12, 0, 13, 14, 0]")
	{
		std::vector<int> test = { 0, 1, 2, 3, 4, 5, 6, 0, 7, 8, 0, 0, 0, 0, 0, 9, 10, 11, 12, 0, 13, 14, 0 };
		WHEN("Moving 0s to back")
		{
			MoveZeroes(test);
			CAPTURE(test);
			
			THEN("Array return untouched")
			{
				std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0};
				REQUIRE(test.size() == expected.size());
				bool isCorrect = true;
				for (int i = 0; i < test.size(); ++i)
				{
					isCorrect &= test[i] == expected[i];
				}
				REQUIRE(isCorrect);
			}
		}
	}
}
