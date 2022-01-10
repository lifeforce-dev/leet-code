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
static const Vec2 s_up   { -1,  0 };
static const Vec2 s_down {  1,  0 };
static const Vec2 s_left {  0, -1 };
static const Vec2 s_right{  0,  1 };

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

void floodFill(Grid& grid, int sr, int sc, int target)
{
	if (grid. empty())
	{
		return;
	}

	std::deque<Vec2> todoCoordStack;
	VisitationGrid visitedCells(grid.size(), std::vector<bool>(grid[0].size(), false));
	if (!isValid(grid, {sr, sc}))
	{
		return;
	}
	int connectingColor = grid[sr][sc];

	todoCoordStack.push_back({ sr, sc });
	bool done = false;
	while (!todoCoordStack.empty())
	{
		Vec2 current = todoCoordStack.back();
		todoCoordStack.pop_back();

		if (visitedCells[current.row][current.col])
		{
			continue;
		}
		visitedCells[current.row][current.col] = true;
		grid[current.row][current.col] = target;

		// check up
		Vec2 newCell = current + s_up;
		if (valueAt(grid, newCell) == connectingColor)
			todoCoordStack.push_back(newCell);
		// check down
		newCell = current + s_down;
		if (valueAt(grid, newCell) == connectingColor)
			todoCoordStack.push_back(newCell);
		// check left
		newCell = current + s_left;
		if (valueAt(grid, newCell) == connectingColor)
			todoCoordStack.push_back(newCell);
		// check right
		newCell = current + s_right;
		if (valueAt(grid, newCell) == connectingColor)
			todoCoordStack.push_back(newCell);
	}
}

SCENARIO("Flood filling a 2D grid")
{
	GIVEN("A simple small 2D array")
	{
		 Grid grid = {
			{1, 1, 1},
			{1, 1, 0},
			{1, 0, 1},
		};

		WHEN("Flood filling from (1,1)")
		{
			Grid result = {
				{2, 2, 2},
				{2, 2, 0},
				{2, 0, 1},
			};

			floodFill(grid, 1, 1, 2);
			REQUIRE(grid == result);
		}
	}

	GIVEN("A 2D array with blob in middle")
	{
		Grid grid = {
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
			{1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1},
			{0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1},
			{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1},
			{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1},
			{0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
			{1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
			{1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1},
			{1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1},
			{1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1},
			{1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1},
			{1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
			{1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
		};

		WHEN("Flood filling from (8,9)")
		{
			Grid result = {
				{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 8, 1, 1, 1},
				{1, 1, 1, 1, 1, 1, 1, 1, 8, 8, 8, 8, 8, 8, 1},
				{1, 1, 1, 1, 0, 0, 1, 1, 8, 8, 8, 1, 8, 8, 1},
				{0, 1, 1, 1, 0, 0, 1, 1, 8, 8, 8, 1, 8, 8, 1},
				{0, 1, 1, 1, 1, 1, 1, 1, 8, 8, 8, 1, 8, 8, 1},
				{0, 1, 1, 1, 1, 1, 1, 1, 8, 8, 8, 1, 8, 8, 1},
				{0, 0, 1, 1, 1, 8, 8, 8, 8, 8, 8, 1, 8, 8, 1},
				{1, 0, 1, 1, 1, 8, 8, 8, 8, 8, 8, 1, 8, 8, 1},
				{1, 0, 1, 1, 1, 1, 1, 8, 8, 8, 8, 1, 8, 8, 1},
				{1, 0, 1, 0, 0, 1, 1, 8, 8, 8, 1, 1, 8, 8, 1},
				{1, 1, 1, 0, 0, 1, 1, 8, 8, 8, 1, 8, 8, 8, 1},
				{1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 8, 8, 1, 8, 1},
				{1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 8, 1, 8, 1},
				{1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 8, 8, 1},
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
			};

			floodFill(grid, 8, 9, 8);
			REQUIRE(grid == result);
		}
	}

	GIVEN("A 2D array with blob on bottom edge")
	{
		Grid grid = {
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
			{1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1},
			{0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1},
			{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1},
			{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1},
			{0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
			{1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
			{1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1},
			{1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1},
			{1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1},
			{1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1},
			{1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
			{1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
		};

		WHEN("Flood filling from (14,0)")
		{
			Grid result = {
				{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1},
				{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
				{1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1},
				{0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1},
				{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1},
				{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1},
				{0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
				{1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
				{1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1},
				{1, 0, 1, 8, 8, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1},
				{1, 1, 1, 8, 8, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1},
				{1, 1, 1, 8, 8, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1},
				{1, 1, 1, 8, 8, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
				{1, 1, 1, 8, 8, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1},
				{8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 1, 1, 1, 1, 1},
			};

			floodFill(grid, 14, 0, 8);
			REQUIRE(grid == result);
		}
	}

	GIVEN("A 2D array with blob on left edge")
	{
		Grid grid = {
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
			{1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1},
			{0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1},
			{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1},
			{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1},
			{0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
			{1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
			{1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1},
			{1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1},
			{1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1},
			{1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1},
			{1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
			{1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
		};

		WHEN("Flood filling from (3,0)")
		{
			Grid result = {
				{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1},
				{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
				{1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1},
				{8, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1},
				{8, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1},
				{8, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1},
				{8, 8, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
				{1, 8, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
				{1, 8, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1},
				{1, 8, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1},
				{1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1},
				{1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1},
				{1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
				{1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1},
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
			};

			floodFill(grid, 3, 0, 8);
			REQUIRE(grid == result);
		}
	}

	GIVEN("An empty grid")
	{
		Grid grid = {};
		WHEN("Flood filling from (4,7)")
		{
			Grid result = {};
			floodFill(grid, 4, 7, 0);

			REQUIRE(grid == result);

		}
	}

	GIVEN("A small grid")
	{
		Grid grid = {
			{0, 0, 0},
			{0, 1, 1}
		};
		WHEN("Flood filling from (4,7)")
		{
			Grid result = {
				{0, 0, 0},
				{0, 1, 1}
			};
			floodFill(grid, 1, 1, 1);

			REQUIRE(grid == result);

		}
	}
}
