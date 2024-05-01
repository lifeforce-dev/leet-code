#include <iostream>
#include <vector>

int findInsertIndex(int target, const std::vector<int>& nums)
{
	// Track indices.
	int begin = 0;
	int end = nums.size() - 1;
	int mid = 0;

	// Insertion should always be possible. -1 indicates a bug.
	int insertIndex = -1;

	if (target > nums[end])
		return nums.size();
	if (target < nums[begin])
		return 0;
	size_t size = nums.size();
	size_t low = 0;


	while (begin <= end)
	{
		mid = begin + (end - begin) / 2;

		// If we find the target, just return its index.
		if (target == nums[mid])
		{
			return mid;
		}

		//  We're getting warmer
		else if (nums[mid] > target)
		{
			// Last record will be index of first item bigger than target.
			insertIndex = mid;
			end = mid - 1;
		}
		// Keep looking
		else
		{
			begin = mid + 1;
		}

	}

	return insertIndex;
}


int main()
{
	// mid = begin + (end - begin) / 2

	// Check mid
	// Determine if in upper or lower half
	// set mid to the upper or lower mid
	// repeat so until found or as long as begin <= end.


	std::vector<int> data = { 1, 3, 5, 6, 7, 9, 12, 15 };

	int value = 13;
	int index = findInsertIndex(value, data);



	return 0;
}
