#define CATCH_CONFIG_MAIN

#include "Catch2/catch.hpp"

#include <algorithm>
#include <string>

namespace
{
	// Extended ascii lut, value indicates last known position of ascii character.
	std::vector<int> s_lut(256, -1);
}
int lengthOfLongestSubstring(const std::string& s)
{
	std::fill(std::begin(s_lut), std::end(s_lut), -1);
	// The goal is to see how large of a gap we can find between begin and end.
	int windowBegin = 0;

	// We advance the end of the window until we reach the same char as window begin.
	int windowEnd = 0;

	// Keeps track of the widest gap between windowBegin and windowEnd.
	int maxLength = 0;

	while (windowEnd < s.size())
	{
		// Load the character we're considering.
		char current = s[windowEnd];

		// Check to see if we've seen this one before.
		int lastSeenIndex = s_lut[current];
		if (lastSeenIndex != -1
			// Any duplicate before windowBegin is stale as has been cut off by another duplicate.
			&& lastSeenIndex >= windowBegin)
		{
			// Move the beginning of the window forward as we try and find a bigger gap.
			windowBegin = lastSeenIndex + 1;
		}

		// Record the gap between the window begin and end, accounting for the current character.
		// ex. "|pew|" when we reach the second |, we measure from second | to p counting the |.
		maxLength = std::max(maxLength, windowEnd - windowBegin + 1);

		// Record where we are now so that if we find a duplicate later, we can measure the gap.
		s_lut[current] = windowEnd;

		// Attempt to further widen the gap.
		windowEnd++;
	}
	return maxLength;
}

SCENARIO("finding the longest substring without repeating characters")
{
	GIVEN("string bbbbb")
	{
		std::string testStr = "bbbbb";

		REQUIRE(testStr.size() == 5);

		WHEN("Finding the longest substring in fully repeating string")
		{
			int length = lengthOfLongestSubstring(testStr);
			REQUIRE(length == 1);
		}
	}

	GIVEN("string abcabcbb")
	{
		std::string testStr = "abcabcbb";

		REQUIRE(testStr.size() == 8);

		WHEN("Finding the longest substring")
		{
			int length = lengthOfLongestSubstring(testStr);
			REQUIRE(length == 3);
		}
	}

	GIVEN("string pwwkew")
	{
		std::string testStr = "pwwkew";

		REQUIRE(testStr.size() == 6);

		WHEN("Finding the longest substring")
		{
			int length = lengthOfLongestSubstring(testStr);
			REQUIRE(length == 3);
		}
	}

	GIVEN("string abba")
	{
		std::string testStr = "abba";

		REQUIRE(testStr.size() == 4);

		WHEN("Finding the longest substring")
		{
			int length = lengthOfLongestSubstring(testStr);
			REQUIRE(length == 2);
		}
	}

	GIVEN("pwwkew")
	{
		std::string testStr = "|pew|";

		REQUIRE(testStr.size() == 5);

		WHEN("Finding the longest substring")
		{
			int length = lengthOfLongestSubstring(testStr);
			REQUIRE(length == 3);
		}
	}

	GIVEN("string @&h^h#&)(  )() - - - - - poiueydh9/~23")
	{
		std::string testStr = "@&h^h#&)(  )() - - - - - poiueydh9/~23 ";

		REQUIRE(testStr.size() == 39);

		WHEN("Finding the longest substring")
		{
			int length = lengthOfLongestSubstring(testStr);
			REQUIRE(length == 10);
		}
	}

	GIVEN("string with only a space")
	{
		std::string testStr = " ";

		REQUIRE(testStr.size() == 1);

		WHEN("Finding the longest substring")
		{
			int length = lengthOfLongestSubstring(testStr);
			REQUIRE(length == 1);
		}
	}

	GIVEN("string wfduwke")
	{
		std::string testStr = "wfduwke";

		REQUIRE(testStr.size() == 7);

		WHEN("Finding the longest substring")
		{
			int length = lengthOfLongestSubstring(testStr);
			REQUIRE(length == 6);
		}
	}

	GIVEN("string dvdf")
	{
		std::string testStr = "dvdf";

		REQUIRE(testStr.size() == 4);

		WHEN("Finding the longest substring")
		{
			int length = lengthOfLongestSubstring(testStr);
			REQUIRE(length == 3);
		}
	}
}
