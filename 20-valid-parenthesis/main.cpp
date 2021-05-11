#include <algorithm>
#include <assert.h>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct BorderToken
{
	// Begins a sequence of characters
	char open;

	// Ends a sequence of characters
	char close;
};
const std::vector<BorderToken> s_borderTokens = { {'{', '}'}, {'(', ')'}, {'[', ']'} };
const std::string s_validString = "YES";
const std::string s_invalidString = "NO";

// Cannot be balanced if we have less than required for a pair.
const int s_minSize = 2;

bool isMatchingClosingToken(char openingToken, char closingToken)
{
	auto it = std::find_if(std::cbegin(s_borderTokens), std::cend(s_borderTokens),
		[openingToken](const BorderToken& t)
		{
			return t.open == openingToken;
		});
	if (it == std::cend(s_borderTokens))
	{
		return false;
	}

	return closingToken == it->close;
}

bool isOpenToken(char c)
{
	return std::any_of(std::cbegin(s_borderTokens), std::cend(s_borderTokens),
		[c](const BorderToken& borderToken)
		{
			return c == borderToken.open;
		});
}

bool isClosedToken(char c)
{
	return std::any_of(std::cbegin(s_borderTokens), std::cend(s_borderTokens),
		[c](const BorderToken& borderToken)
		{
			return c == borderToken.close;
		});
}

// Complete the isBalanced function below.
std::string isBalanced(std::string s)
{
	// If the last character is an open token, this sequence can't possibly be valid.
	if (isOpenToken(s[s.size() - 1])
		|| s.size() < s_minSize)
	{
		return s_invalidString;
	}

	// Start by moving our first token to the front of our stack.
	std::deque<char> openTokenStack;

	// Current token in string we're considering.
	int current = 0;
	while (current <= s.size())
	{
		// End of string and we found uneven matching closing tokens.
		if (!openTokenStack.empty() && static_cast<size_t>(current) >= s.size())
			return s_invalidString;

		// End of string, and all matching closing tokens have been found.
		if (openTokenStack.empty() && static_cast<size_t>(current) >= s.size())
			return s_validString;

		// Attempt to validate.
		if (isOpenToken(s[current]))
		{
			// Open braces are added to the stack until we find their matching closing brace.
			openTokenStack.push_front(s[current]);
		}
		else if (isMatchingClosingToken(openTokenStack.front(), s[current]))
		{
			// We found the patching closing token, we're done with this opening token.
			openTokenStack.pop_front();
		}
		else
		{
			// If we're here, we just encountered a loose closing brace. Impossible to be valid.
			return s_invalidString;
		}
		current++;
	}

	return s_invalidString;
}

// My own driver
int main()
{
	std::vector<std::string> strings{ "{(([])[])[]]}", "(){}[]","{[()]}","{{}}(", "{", "}{", "{(})" };
	std::vector<std::string> validStrings;
	for (size_t i = 0; i < strings.size(); ++i)
	{
		std::string result = isBalanced(strings[i]);

		validStrings.push_back(result);

		std::cout << "result: " << result << "\n";
	}

	assert(validStrings[0] == "NO");
	assert(validStrings[1] == "YES");
	assert(validStrings[2] == "YES");
	assert(validStrings[3] == "NO");
	assert(validStrings[4] == "NO");
	assert(validStrings[5] == "NO");
	assert(validStrings[6] == "NO");
	return 0;
}
