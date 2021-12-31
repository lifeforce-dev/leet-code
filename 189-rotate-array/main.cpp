#include <algorithm>
#include <vector>

void rotate(std::vector<int>& nums, int k)
{
	if (nums.size() <= 1)
	{
		return;
	}

	std::rotate(std::rbegin(nums), std::rbegin(nums) + (k % nums.size()), std::rend(nums));
}

int main()
{
	std::vector<int> test = { 1, 2 };

	rotate(test, 3);

	return 0;
}
