#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> sortedSquares(std::vector<int>& nums)
{
	std::vector<int> squares;
	squares.reserve(nums.size());

	for (int num : nums)
	{
		squares.push_back(num*num);
	}
	std::sort(std::begin(squares), std::end(squares));
	return squares;
}

int main()
{
	std::vector<int> nums = { -4,-1,0,3,10 };
	auto a = sortedSquares(nums);
	return 0;
}
