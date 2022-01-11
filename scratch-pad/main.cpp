#include <algorithm>
#include <string>
#include <string_view>
#include <iterator>
#include <iostream>
#include <vector>
#include <sstream>

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



struct Window
{
	// index of the first character in the window.
	int beginIndex = 0;

	// index of the second character in the window.
	int endIndex = 0;

	// View of the letters we can see through the window.
	std::string_view view;
};

void init(std::string_view& v, const std::string& s)
{
	// MAYBE FINE
	// Here you're creating a string_view that will store a pointer to the
	// actual char* data stored in the string object.

	// NOTE You still need to be careful with this as there's no way to know
	// if s is a temporary (like if you called init(v, "some string"));
	v = std::string_view(s.data(), 4);

	// BAD
	// Here you're creating a string_view that will store a pointer to the
	// // temporary string object created by substr().

	//v = std::string_view(s.substr(0, 5));
}

void printStringView(const std::string_view& sv)
{
	for (int i = 0; i < sv.size(); ++i) { std::cout << sv[i]; }
}


// Test this by running in release.
int main()
{

	int count = 0;
	std::stringstream ss;
	std::string str = "aaaabbbbbbcccccccccccccccccccccccccccccccdddddddhhhhhjjjjjkkkkkkssssso";
	str = "aaab";
	char current = str[0];
	int size = str.size();
	bool done = false;
	int pos = 0;
	while (!done)
	{
		count++;
		done = pos + 1 == str.size();
		if (done || str[pos + 1] != current)
		{
			ss << current << count;
			count = 0;
			current = str[std::min(pos + 1, size - 1)];
		}
		pos++;
	}

	std::cout << ss.str();
}
;