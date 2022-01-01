#include <algorithm>
#include <string>
#include <iterator>

std::string reverseWords(std::string s)
{
	// Count the characters in a word
	int charCount = 0;
	std::string::iterator it = s.begin();

	for (int i = 0; i < s.size(); ++i)
	{
		// Saw either a space or end of string.
		if (*it == ' ')
		{
			std::reverse(it - charCount, it);

			charCount = 0;
		}
		else if (i == s.size() - 1)
		{
			std::reverse(it - charCount, s.end());
			return s;
		}
		else
		{
			// Don't count spaces
			charCount++;
		}

		std::advance(it, 1);
	}
	return "";
}


int main()
{
	std::string s = "abcdefg hijkl mnop qrst u v wxy z";
	s = reverseWords(s);
}