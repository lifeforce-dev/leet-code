#define CATCH_CONFIG_MAIN

#include "Catch2/catch.hpp"

#include "common/Log.h"
#include "common/Random.h"
#include "common/Timer.h"

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <string_view>
#include <vector>

namespace {
	// Used to index lowercase ascii characters.
	int s_letterOffset = 97;
	int s_asciiTableSize = 26;

	// Frequency of all chars in the string we want permutations of.
	std::vector<int> s_queryStrFrequencyTable(s_asciiTableSize, 0);

	// Frequency of all chars in the string that might contain permutations of query.
	std::vector<int> s_sourceStrFrequencyTable(s_asciiTableSize, 0);
}

struct WindowContext
{
	// Index into source string of the first character in the window.
	int beginIndex = 0;

	// Keeping track of the number of matches we find in our window.
	int numMatches = 0;

	// When we reach this number of matches, we're done.
	int targetMatches = 0;

	// View of the letters we can see through the window.
	std::string_view view;
};

// Returns whether the letter appears in both tables with the same frequency.
bool isMatch(char letter)
{
	int queryTableCount = s_queryStrFrequencyTable[letter - s_letterOffset];
	int sourceTableCount = s_sourceStrFrequencyTable[letter - s_letterOffset];

	return queryTableCount > 0 && sourceTableCount == queryTableCount;
}

void updateMatch(WindowContext& window, bool wasMatch, char letter)
{
	bool isMatchNow = isMatch(letter);
	if (wasMatch == isMatchNow)
		return;

	// Whether we came TO or FROM being a match, update numMatches appropriately.
	window.numMatches += !wasMatch && isMatch(letter) ? 1 : -1;
}

void updateFrequency(WindowContext& window, std::vector<int>& table, char letter, int count)
{
	// We need to know if updating the frequency caused a match or an unmatch.
	bool wasMatch = isMatch(letter);
	table[letter - s_letterOffset] += count;
	updateMatch(window, wasMatch, letter);
}

int getUniqueLetterCount(const std::string& str)
{
	std::vector<int> letterCounts(s_asciiTableSize, 0);
	int uniqueCount = 0;
	for (char letter : str)
	{
		// Early out in the case of huge string and we have already seen the alphabet.
		if (uniqueCount>= s_asciiTableSize)
		{
			return uniqueCount;
		}

		int currentCount = ++letterCounts[letter - s_letterOffset];
		if (currentCount == 1)
		{
			uniqueCount++;
		}
	}

	return uniqueCount;
}

void init(const std::string& queryStr, const std::string& sourceStr, WindowContext& window)
{
	std::fill(std::begin(s_queryStrFrequencyTable), std::end(s_queryStrFrequencyTable), 0);
	std::fill(std::begin(s_sourceStrFrequencyTable), std::end(s_sourceStrFrequencyTable), 0);

	int count = getUniqueLetterCount(queryStr);
	window.targetMatches = count;

	// Init the window.
	window.beginIndex = 0;
	std::string_view view(sourceStr.data(), queryStr.size());
	
	// Populate initial frequency table for query string.
	for (int i = 0; i < queryStr.size(); ++i)
	{
		updateFrequency(window, s_queryStrFrequencyTable, queryStr[i], 1);
	}

	// Load the initial window into the source frequency table.
	for (int i = 0; i < view.size(); ++i)
	{
		updateFrequency(window, s_sourceStrFrequencyTable, sourceStr[i], 1);
	}

	window.view = view;
}

// Moves the window forward one step
void updateWindow(WindowContext& window, const std::string& sourceStr)
{
	std::string_view& view = window.view;

	// When moving the window over, we only need to maintain the counts of
	// the letter falling off, and the letter coming into view.
	updateFrequency(window, s_sourceStrFrequencyTable, view.front(), -1);

	// Shift the window over.
	window.beginIndex++;
	view = std::string_view(sourceStr.data() + window.beginIndex, view.size());

	updateFrequency(window, s_sourceStrFrequencyTable, view.back(), 1);
}

bool isWindowPermutation(const WindowContext& window)
{
	if (window.numMatches != window.targetMatches)
		return false;

	return true;
}

bool checkInclusion(const std::string& queryStr, const std::string& sourceStr)
{
	if (queryStr.empty() || sourceStr.empty())
	{
		return false;
	}

	// There are not enough letters in sourceStr to contain a permutation of queryStr.
	if (queryStr.size() > sourceStr.size())
	{
		return false;
	}

	// Set up the look up tables and our initial window.
	WindowContext window;
	init(queryStr, sourceStr, window);

	// We're done if initial window happens to be a permutation.
	if (isWindowPermutation(window))
	{
		std::cout << "Found Permutation. beginIndex=" << window.beginIndex << "\n";
		return true;
	}

	int sentinel = sourceStr.size() - queryStr.size();
	while (window.beginIndex < sentinel)
	{
		// Move the window and check if it's a permutation.
		updateWindow(window, sourceStr);
		if (isWindowPermutation(window))
		{
			bool isPermutation = true;

			std::cout << "Found Permutation. beginIndex=" << window.beginIndex << "\n";
			return true;
		}
	}
	std::cout << "Failed to find permutation. beginIndex=" << window.beginIndex << "\n";
	return false;
}

SCENARIO("Determine if a string contains a permutation of another string.")
{
	GIVEN("Query string [abbo] source string [beidbabooo].")
	{
		// Visualization:
		// queryStr = abbo
		// sourceStr = beidbabooo
		// windowEnd      |
		// windowBegin _

		std::string queryStr = "abbo";
		std::string sourceStr = "beidbabooo";

		WHEN("Searching for permutation")
		{
			REQUIRE(checkInclusion(queryStr, sourceStr) == true);
		}
	}

	GIVEN("Source string with permutation at end.")
	{
		std::string queryStr = "ab";
		std::string sourceStr = "lkjhgfdsaapoiuytrewba";

		WHEN("Searching for permutation")
		{
			REQUIRE(checkInclusion(queryStr, sourceStr) == true);
		}
	}

	GIVEN("Source string with permutation at beginning.")
	{
		std::string queryStr = "ab";
		std::string sourceStr = "bacdefghijku";

		WHEN("Searching for permutation")
		{
			REQUIRE(checkInclusion(queryStr, sourceStr) == true);
		}
	}

	GIVEN("Source string with lots of duplicate sequences.")
	{
		std::string queryStr = "ab";
		std::string sourceStr = "aafbfafbfabfffaoksl";
		WHEN("Searching for permutation")
		{
			REQUIRE(checkInclusion(queryStr, sourceStr) == true);
		}
	}

	GIVEN("A query string larger than source string.")
	{
		std::string queryStr = "ab";
		std::string sourceStr = "a";

		WHEN("Searching for permutation")
		{
			REQUIRE(checkInclusion(queryStr, sourceStr) == false);
		}
	}

	GIVEN("An empty query string.")
	{
		std::string queryStr = "";
		std::string sourceStr = "abcdefg";

		WHEN("Searching for permutation")
		{
			REQUIRE(checkInclusion(queryStr, sourceStr) == false);
		}
	}

	GIVEN("Two empty strings.")
	{
		std::string queryStr = "";
		std::string sourceStr = "";

		WHEN("Searching for permutation")
		{
			REQUIRE(checkInclusion(queryStr, sourceStr) == false);
		}
	}

	GIVEN("Testing a massive string with permutation at end.")
	{
		std::string queryStr = "aabccddezzzzzzzzffghiijkklmnoopqrrrstttuvvwx"
			"yffffffggggggggguuuuiiiiiiissssssoijfoijweofijosidjfoijsofdijwo"
			"joijiwmlkfmqmlkmsfmnmasofijfoijoijosijoijwmnxcvnjksdfqupduihqez";
		//std::string queryStr = "abcdefghijklmnopqrstuvwxyz";
		std::random_device rd;
		std::mt19937 gen(rd());

		// Derived from shuffling the "query string" so that the permutation isn't 1:1 identical.
		std::string permutationStr = queryStr;
		std::shuffle(permutationStr.begin(), permutationStr.end(), gen);

		// Generate massive random string with guaranteed permutation at the end.
		std::string randomStr;
		randomStr.reserve(10000000);
		Common::GetRandomLowercaseString(randomStr, 10000000);
		std::stringstream ss;
		ss << std::move(randomStr) << std::move(permutationStr);

		//std::string queryStr = "cadebabb";

		WHEN("Searching for permutation")
		{
			std::cout << "Starting:\n";
			Common::Timer timer;
			timer.Start();
			bool foundPermutation = checkInclusion(queryStr, ss.str());
			if (!foundPermutation)
			{
				std::cout << ss.str();
			}
			REQUIRE(foundPermutation);
			timer.Stop();
			if (foundPermutation)
			{
			std::cout << fmt::format("Found permutation test 2: {} |Time Elapsed: {}ms\n",
				foundPermutation,
				timer.GetTotalElapsedMs().count());
			}
		}
	}
}
