#define CATCH_CONFIG_MAIN

#include "Catch2/catch.hpp"
#include "common/Log.h"
#include "common/Timer.h"

#include <algorithm>
#include <assert.h>
#include <deque>
#include <vector>

bool isPalindromeDequeSolution(int x)
{
	// Negative numbers cannot be a palindrome
	if (x < 0)
	{
		return false;
	}

	int forwardCounter = x;
	std::deque<int> forwardDigits;
	while (forwardCounter > 0)
	{
		int digit = forwardCounter % 10;
		forwardCounter /= 10;
		forwardDigits.push_front(digit);
	}

	int reverseCounter = x;
	std::deque<int> reverseDigits;
	while (reverseCounter > 0)
	{
		int digit = reverseCounter % 10;
		reverseCounter /= 10;
		reverseDigits.push_back(digit);
	}

	return forwardDigits == reverseDigits;
}

// Given an integer x, return true if x is a palindrome, and false otherwise.
bool isPalindromeMathSolution(int x)
{
	// Negative numbers cannot be a palindrome
	if (x < 0)
	{
		return false;
	}

	// Single digit numbers are always a palindrome
	if (x < 10)
	{
		return true;
	}

	// Multiples of 10 are never a palindrome
	if (x % 10 == 0)
	{
		return false;
	}

	int reverseCounter = x;
	int reverseDigits = 0;
	int numPos = -1;

	int firstDigit = 0;
	int lastDigit = 0;
	while (reverseCounter > 0)
	{
		if (reverseCounter == x)
		{
			lastDigit = reverseCounter % 10;
		}
		if (reverseCounter / 10 == 0)
		{
			firstDigit = reverseCounter % 10;
		}
		reverseCounter /= 10;
		numPos++;
	}

	// If the reverse of the number is larger than int max it cannot be a palindrome.
	int64_t intMaxTest = std::pow(10, numPos) * lastDigit;
	if (intMaxTest > std::numeric_limits<int>::max())
	{
		return false;
	}

	if (firstDigit != lastDigit)
	{
		return false;
	}

	reverseCounter = x;
	while (reverseCounter > 0)
	{
		int temp = reverseCounter % 10;
		reverseDigits += std::pow(10, numPos) * temp;
		reverseCounter /= 10;
		numPos--;
	}

	return reverseDigits == x;
}

SCENARIO("Check if an int is a palindrome")
{
	GIVEN("A 3 digit number.")
	{
		REQUIRE(isPalindromeDequeSolution(121));
	}

	GIVEN("A 3 digit number.")
	{
		REQUIRE(!isPalindromeDequeSolution(123));
	}

	GIVEN("A 3 digit number.")
	{
		REQUIRE(isPalindromeDequeSolution(1234554321));
	}

	GIVEN("A 3 digit number.")
	{
		REQUIRE(isPalindromeDequeSolution(122221));
	}

	GIVEN("A 3 digit number.")
	{
		REQUIRE(!isPalindromeDequeSolution(-121));
	}

	GIVEN("A 3 digit number.")
	{
		REQUIRE(!isPalindromeDequeSolution(121345));
	}

	GIVEN("A 2 digit number.")
	{
		REQUIRE(!isPalindromeDequeSolution(10));
	}

	GIVEN("A 2 digit number.")
	{
		REQUIRE(!isPalindromeDequeSolution(1000));
	}

	GIVEN("A 2 digit number.")
	{
		REQUIRE(!isPalindromeDequeSolution(1234567899));
	}

}
