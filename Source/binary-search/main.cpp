#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <assert.h>

bool isSortedAscending(int data[], int size)
{
	// 0, 1, 2, 3, 4...
	for (int i = 0; i < size;)
	{
		if (data[i] > data[++i])
		{
			return false;
		}
	}

	return true;
}

bool containsDuplicates(int data[], int size)
{
	assert(isSortedAscending(data, size));

	for (int i = 0; i < size;)
	{
		if (data[i] == data[++i])
		{
			return false;
		}
	}

	return true;
}

int search(int data[], int target, int beginIndex, int endIndex)
{
	if (endIndex >= beginIndex)
	{
		int midIndex = beginIndex + (endIndex - beginIndex) / 2;

		if (data[midIndex] == target)
		{
			return midIndex;
		}

		// Search upper half.
		if (data[midIndex] < target)
		{
			return search(data, target, midIndex + 1, endIndex);
		}

		// Search lower half.
		if (data[midIndex] > target)
		{
			return search(data, target, beginIndex, midIndex - 1);
		}
	}
	return -1;
}

int main()
{
	return 0;
}
