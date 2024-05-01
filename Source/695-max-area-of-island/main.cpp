#define CATCH_CONFIG_MAIN
#include "Catch2/catch.hpp"

#include <vector>
#include <deque>

struct Vec2
{
	int row = 0;
	int col = 0;
	Vec2 operator+ (const Vec2& other)
	{
		return { row + other.row, col + other.col };
	}
};
static const Vec2 s_up{ -1,  0 };
static const Vec2 s_down{ 1,  0 };
static const Vec2 s_left{ 0, -1 };
static const Vec2 s_right{ 0,  1 };
static const int s_water = 0;
static const int s_land = 1;

using Grid = std::vector<std::vector<int>>;
using VisitationGrid = std::vector<std::vector<bool>>;
bool isValid(const Grid& grid, Vec2 cell)
{
	int cellRow = cell.row;
	int cellCol = cell.col;

	return cellRow >= 0 && cellRow < grid.size()
		&& cellCol >= 0 && cellCol < grid[0].size();
}

int valueAt(const Grid& grid, Vec2 cell)
{
	if (!isValid(grid, cell))
	{
		return -1;
	}
	return grid[cell.row][cell.col];
}

// Starting from the given Vec2 coord, attempt to map out an island and return its size.
int discoverIsland(const Grid& grid, VisitationGrid& islandMap, const Vec2& start)
{
	if (!isValid(grid, start))
	{
		return 0;
	}
	int islandSize = 0;
	std::deque<Vec2> todoCoordStack;

	todoCoordStack.push_back(start);

	while (!todoCoordStack.empty())
	{
		Vec2 current = todoCoordStack.back();
		todoCoordStack.pop_back();

		// Already plotted this cell
		if (islandMap[current.row][current.col])
			continue;

		// Don't care about water
		if (valueAt(grid, current) == s_water)
			continue;

		islandSize++;

		// Plot the land on our map.
		islandMap[current.row][current.col] = true;
		
		Vec2 newCell = current + s_up;
		if (valueAt(grid, newCell) == s_land)
			todoCoordStack.push_back(newCell);

		newCell = current + s_down;
		if (valueAt(grid, newCell) == s_land)
			todoCoordStack.push_back(newCell);

		newCell = current + s_left;
		if (valueAt(grid, newCell) == s_land)
			todoCoordStack.push_back(newCell);

		newCell = current + s_right;
		if (valueAt(grid, newCell) == s_land)
			todoCoordStack.push_back(newCell);
	}
	return islandSize;
}

int maxAreaOfIsland(Grid& grid)
{
	if (grid.empty())
		return 0;

	VisitationGrid islandMap(grid.size(), std::vector<bool>(grid.size(), false));
	int largestIsland = 0;
	for (int i = 0; i < grid.size(); ++i)
	{
		for (int j = 0; j < grid.size(); ++j)
		{
			// Skip cells that are already known to be part of an island, or can't be an island.
			Vec2 current = { i, j };
			if (islandMap[i][j] || valueAt(grid, current) == s_water)
			{
				continue;
			}
			largestIsland = std::max(largestIsland, discoverIsland(grid, islandMap, current));
		}
	}

	return largestIsland;
}

SCENARIO("In a grid of islands, find the largest one")
{
	GIVEN("A binary grid")
	{
		Grid grid = {
			{ 0, 0, 1, 0, 0, 0, 0, 1, 0 ,0 ,0 ,0 ,0 },
			{ 0, 0, 0, 0, 0, 0, 0, 1, 1 ,1 ,0 ,0 ,0 },
			{ 0, 1, 1, 0, 1, 0, 0, 0, 0 ,0 ,0 ,0 ,0 },
			{ 0, 1, 0, 0, 1, 1, 0, 0, 1 ,0 ,1 ,0 ,0 },
			{ 0, 1, 0, 0, 1, 1, 0, 0, 1 ,1 ,1 ,0 ,0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,1 ,0 ,0 },
			{ 0, 0, 0, 0, 0, 0, 0, 1, 1 ,1 ,0 ,0 ,0 },
			{ 0, 0, 0, 0, 0, 0, 0, 1, 1 ,0 ,0 ,0 ,0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 },
		};

		WHEN("Finding the largest island")
		{
			REQUIRE(maxAreaOfIsland(grid) == 6);
		}
	}
}