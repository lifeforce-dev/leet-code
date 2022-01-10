
namespace {

const int s_firstBad = 2751;
const int s_currentVersion = 10000;

}

static int apiCallCount = 0;
bool isBadVersion(int checkVersion)
{
	apiCallCount++;
	return checkVersion >= s_firstBad;
}

int main()
{
	// When going up, its mid + 1 to end
	// When going down, its mid-1 to begin
	// begin is 0, end is n - 1 for arrays

	long begin = 0;
	long end = s_currentVersion;
	long mid = 0;
	int currentBadVersion = mid;
	while (begin <= end)
	{
		// Just memorize this.
		mid = begin + (end - begin) / 2;
		if (isBadVersion(mid))
		{
			currentBadVersion = mid;
			end = mid - 1;
		}
		else
		{
			begin = mid + 1;
		}
	}

	return 0;
}
