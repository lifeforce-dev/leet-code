#define CATCH_CONFIG_MAIN
#include "Catch2/catch.hpp"

#include "common/Timer.h"
#include "common/Random.h"
#include "common/Log.h";

#include <string>
#include <sstream>
#include <vector>

namespace {
	Common::Timer s_timer;
}

int compress(std::vector<char>& chars) {
	std::string tmp = "";
	int64_t left = 0;
	int64_t right = 0;
	int64_t count = 0;
	s_timer.Start();
	while (left < chars.size())
	{
		while (right < chars.size() && chars[left] == chars[right])
		{
			right++;
		}
		int64_t length = right - left;

		if (length == 1)
		{
			tmp += chars[left];
		}
		else
		{
			tmp += chars[left] + std::to_string(length);
		}

		left = right;
	}
	s_timer.Stop();
	std::cout << fmt::format("Compressing string strSize: |Time Elapsed: {}ms\n",
		s_timer.GetTotalElapsedMs().count());
	s_timer.Stop();
	chars.clear();
	for (auto i : tmp) {
		chars.push_back(i);
	}
	std::cout << "Count: " << count << "\n";
	return tmp.length();
}

int countDigits(int i)
{
	int64_t count = 0;
	while (i != 0)
	{
		i = i / 10;
		++count;
	}

	return count;
}

void print(const std::vector<char>& str)
{
	for (auto a : str)
	{
		std::cout << a;
	}
	std::cout << "\n";
}
SCENARIO("Run length encoding")
{

	GIVEN("An string ddddppppfffsyyuudddllwwwiifftttjjjjjvvvxaaaiiaafff")
	{
		// d4p4f3sy2u2d3l2w3i2f2t3j5v3xa3i2a2f3
		std::string str = "ddddppppfffsyyuudddllwwwiifftttjjjjjvvvxaaaiiaafff";
		std::vector<char> vec;
		for (auto a : str)
		{
			vec.push_back(a);
		}

		WHEN("Encoding")
		{

			REQUIRE(compress(vec) == 36);
		}
	}

	GIVEN("An empty string")
	{
		std::vector<char> str;
		
		WHEN("Encoding")
		{

			REQUIRE(compress(str) == 0);
			REQUIRE(str.empty());
		}
	}

	GIVEN("A small string ")
	{
		std::vector<char> str = { 'a','a', 'b', 'b', 'c', 'c', 'c'};

		WHEN("Encoding")
		{
			std::vector<char> result = { 'a', '2', 'b', '2', 'c', '3' };
			REQUIRE(compress(str) == 6);
			REQUIRE(str == result);
		}
	}

	GIVEN("String with a bunch of singles")
	{
		std::vector<char> str = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
			'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
			'x', 'y', 'z'};

		WHEN("Encoding")
		{
			std::vector<char> result = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
			'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
			'x', 'y', 'z' };

			REQUIRE(compress(str) == 26);
			REQUIRE(str == result);
		}
	}

	GIVEN("A massive random string")
	{
		// Generate a 12 GB random string to encode.
		int64_t strSize = 12000000000;
		int64_t maxRepeatLength = 10000000;
		std::vector<char> decodedString;
		decodedString.reserve(strSize);
		std::string letters = "abcdefghijklmnopqrstuvwxyz";
		int64_t encodedStrLength = 0;
		char oldLetter = ' ';
		while (strSize > 0)
		{
			int64_t num = Common::GetRandomNumber(1, maxRepeatLength);
			if (strSize - num < 0)
			{
				num = strSize;
			}
			int64_t numDigitCount = 0;
			if (num != 1)
			{
				numDigitCount = countDigits(num);
			}

			// letter + digit. i.e a3 is 2. a345 is 4.
			encodedStrLength += numDigitCount + 1;

			strSize -= num;

			// for counting purposes, make sure we don't pick the same letter.
			char newLetter = ' ';
			do
			{
				newLetter = letters[Common::GetRandomNumber(0, letters.size() - 1)];
			} while (oldLetter == newLetter);


			for (int64_t i = 0; i < num; ++i)
			{
				decodedString.push_back(newLetter);
			}
			oldLetter = newLetter;
		}
		std::vector<char> oldDecodedString = decodedString;
		std::vector<char> encodedString = decodedString;
		WHEN("Encoding")
		{
			Common::Timer timer;
			timer.Start();
			std::cout << "Compressing...\n";
			REQUIRE(compress(encodedString) == encodedStrLength);
			timer.Stop();
			std::cout << fmt::format("Compressing string size:{}  |Time Elapsed: {}ms\n",
				encodedStrLength,
				timer.GetTotalElapsedMs().count());
		}
	}
}
