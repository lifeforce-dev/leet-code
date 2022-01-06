#define CATCH_CONFIG_MAIN

#include "Catch2/catch.hpp"

#include "common/Log.h"
#include "common/Random.h"
#include "common/Timer.h"

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <random>
#include <string>
#include <string_view>
#include <iostream>
#include <iterator>

namespace {
	// Used to index lowercase ascii characters.
	int s_letterOffset = 97;
	int s_asciiTableSize = 26;

	// Frequency of all chars in the string we want permutations of.
	std::vector<int> s_queryStrFrequencyTable(s_asciiTableSize, 0);

	// Frequency of all chars in the string that might contain permutations of query.
	std::vector<int> s_sourceStrFrequencyTable(s_asciiTableSize, 0);
}

struct Window
{
	// Index into source string of the first character in the window.
	int beginIndex = 0;

	// Keeping track of the number of matches we find in our window.
	int numMatches = 0;

	// View of the letters we can see through the window.
	std::string_view view;
};

void IncrementFrequency(std::vector<int>& table, char letter)
{
	// index into the table using ascii as an index.
	table[letter - s_letterOffset]++;
}

void DecrementFrequency(std::vector<int>& table, char letter)
{
	// index into the table using ascii as an index.
	int count = table[letter - s_letterOffset];
	count--;
	// This should never be below 0.
	assert(count >= 0);
	table[letter - s_letterOffset] = count;
}

// returns whether the letter appears in both tables.
bool isMatch(char letter)
{
	return s_queryStrFrequencyTable[letter - s_letterOffset] > 0
		&& s_sourceStrFrequencyTable[letter - s_letterOffset] > 0;
}

bool isFrequencyMatch(char letter)
{
	return s_queryStrFrequencyTable[letter - s_letterOffset] ==
		s_sourceStrFrequencyTable[letter - s_letterOffset];
}

void init(const std::string queryStr, const std::string& sourceStr, Window& window)
{
	std::fill(std::begin(s_queryStrFrequencyTable), std::end(s_queryStrFrequencyTable), 0);
	std::fill(std::begin(s_sourceStrFrequencyTable), std::end(s_sourceStrFrequencyTable), 0);

	// Init the window.
	window.beginIndex = 0;
	std::string_view view(sourceStr.data(), queryStr.size());
	
	// Populate initial frequency table for query string.
	for (int i = 0; i < queryStr.size(); ++i)
	{
		IncrementFrequency(s_queryStrFrequencyTable, queryStr[i]);
	}

	// Load the initial window into the source frequency table.
	for (int i = 0; i < view.size(); ++i)
	{
		IncrementFrequency(s_sourceStrFrequencyTable, sourceStr[i]);
	}

	// Init matches in our inital window.
	for (int i = 0; i < view.size(); ++i)
	{
		if (isMatch(view[i]))
		{
			window.numMatches++;
		}
	}

	window.view = view;
}

// Moves the window forward one step
void updateWindow(Window& window, const std::string& sourceStr)
{
	// To move the window, we need to shift the left side and then the right.
	std::string_view& view = window.view;

	// We need to account for matches that might have fallen out of view.
	if (isMatch(view[0]))
	{
		window.numMatches--;
	}

	DecrementFrequency(s_sourceStrFrequencyTable, view[0]);

	// Shift the window over.
	window.beginIndex++;
	view = std::string_view(sourceStr.data() + window.beginIndex, view.size());

	int viewEnd = view.size() - 1;
	IncrementFrequency(s_sourceStrFrequencyTable, view[viewEnd]);

	// Now we need to account for any new matches that might have come into view.
	if (isMatch(view[viewEnd]))
	{
		window.numMatches++;
	}
}

bool IsWindowPermutation(const Window& window)
{
	const std::string_view& windowView = window.view;
	int viewSize = windowView.size();
	if (window.numMatches != viewSize)
	{
		return false;
	}

	// If all characters in our view are matches of the same frequency,
	// this window is a permutation.
	bool isPermutation = true;
	for (int i = 0; i < viewSize; ++i)
	{
		isPermutation &= isMatch(windowView[i])
			&& isFrequencyMatch(windowView[i]);
	}

	return isPermutation;
}

bool checkInclusionTest2(std::string queryStr, std::string sourceStr)
{
	// There are not enough letters in sourceStr to contain a permutation of queryStr.
	if (queryStr.size() > sourceStr.size())
	{
		return false;
	}

	Window window;
	init(queryStr, sourceStr, window);
	if (IsWindowPermutation(window))
	{
		return true;
	}

	int sentinal = sourceStr.size() - queryStr.size();
	while (window.beginIndex < sentinal)
	{
		updateWindow(window, sourceStr);
		if (IsWindowPermutation(window))
		{
			return true;
		}
	}

	return false;
}

SCENARIO("Determine if a string contains a permutation of another string.")
{
	GIVEN("test string [ab] source string [eidbaooo]")
	{
		std::string s1 = "abbo";
		std::string s2 = "beidbabooo";

		WHEN("Searching for permutation")
		{
			REQUIRE(checkInclusionTest2(s1, s2) == true);
		}
	}

	GIVEN("test string with permutation at end")
	{
		std::string s1 = "ab";
		std::string s2 = "lkjhgfdsaapoiuytrewba";

		WHEN("Searching for permutation")
		{
			REQUIRE(checkInclusionTest2(s1, s2) == true);
		}
	}

	GIVEN("test string with permutation at beginning")
	{
		std::string s1 = "ab";
		std::string s2 = "bacdefghijku";

		WHEN("Searching for permutation")
		{
			REQUIRE(checkInclusionTest2(s1, s2) == true);
		}
	}
	GIVEN("test string with lots of duplicates")
	{
		std::string s1 = "ab";
		std::string s2 = "aaaaaaaaaaaaaaaafbfafbfabfffaoksl";

		WHEN("Searching for permutation")
		{
			REQUIRE(checkInclusionTest2(s1, s2) == true);
		}
	}

	GIVEN("An source string larger than query string.")
	{
		std::string s1 = "ab";
		std::string s2 = "a";

		WHEN("Searching for permutation")
		{
			REQUIRE(checkInclusionTest2(s1, s2) == false);
		}
	}

	GIVEN("Testing a massive string with permutation at end")
	{
		// Generate massive random string with guaranteed permutation at the end.
		//std::string s1 = "snroavitraifgtwrkchdczmlduoxpqkvbyftej";
		//std::string s2 = Common::GetRandomLowercaseString(10000000);
		//std::stringstream ss;
		//ss << s2 << s1;
		//std::ofstream out("test_data.txt");
		//out << ss.str();
		//out.close();
		//-----------------------------------------------------------
		std::ifstream data;
		data.open("test_data.txt");
		std::string s1 = "aabccddeffghiijkklmnoopqrrrstttuvvwxyz";
		std::string s2;
		s2.reserve(10000000);
		std::getline(data, s2);

		WHEN("Searching for permutation")
		{
			Common::Timer timer;
			timer.Start();
			bool foundPermutation = checkInclusionTest2(s1, s2);
			REQUIRE(foundPermutation);
			timer.Stop();
			std::cout << fmt::format("Found permutation test 2: {} |Time Elapsed: {}ms\n",
				foundPermutation,
				timer.GetTotalElapsedMs().count());
		}
	}
}
