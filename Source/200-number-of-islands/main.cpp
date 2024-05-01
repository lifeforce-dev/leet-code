//=====================================================
//
// Number of Islands
// Given an m x n 2D binary grid grid which represents a map of '1's (land)
// and '0's (water), return the number of islands.
//
// An island is surrounded by water and is formed by connecting adjacent lands
// horizontally or vertically. You may assume all four edges of the grid are all
// surrounded by water.
//


// Ex 1:
// Input: grid = [
// ["1", "1", "1", "1", "0"],
// ["1", "1", "0", "1", "0"],
// ["1", "1", "0", "0", "0"],
// ["0", "0", "0", "0", "0"]
// ]
// Output : 1

// 0, 0 = 1
// 0, 1 = 1
// 1, 0 = 1


// north = -1,  0
// south =  1,  0
// east  =  0,  1
// west  =  0, -1


// Input: grid = [
// ["1", "1", "0", "0", "0"],
// ["1", "1", "0", "0", "0"],
// ["0", "0", "1", "0", "0"],
// ["0", "0", "0", "1", "1"]
// ]
// Output : 3

#include <iostream>
#include <vector>
#include <deque>

enum struct DirectionType : int32_t
{
	North,
	South,
	East,
	West
};

struct Coord
{
	int x = 0;
	int y = 0;
};

using Direction = std::pair<DirectionType, Coord>;
const std::vector<Direction> s_directions = {
	{ DirectionType::North, { -1,  0 } },
	{ DirectionType::South, {  1,  0 } },
	{ DirectionType::East,  {  0,  1 } },
	{ DirectionType::West,  {  0, -1 } }
};

const std::vector<std::vector<char>> s_testGrid = {
	{'1', '1', '0', '0', '0'},
	{'1', '1', '0', '0', '0'},
	{'0', '0', '1', '0', '0'},
	{'0', '0', '0', '1', '1'},
};

class Cell
{
private:
	int m_id = 0;
	Coord m_index;
};


int GetNumIslands(std::vector<std::vector<char>>& grid)
{
	return 0;
}

int main()
{
	// find a 1
	// out of bounds counts as 0
	// find north 0, south 0, east 0, west 0, or out of bounds
	//

}
