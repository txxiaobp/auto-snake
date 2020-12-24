#include "screen_data.h"
#include <cstdlib>
#include <ctime>
#include <cassert>
#include "seed.h"

ScreenData::ScreenData(int r, int c) :
    data(std::vector<std::vector<Node_t> >(r, std::vector<Node_t>(c, Node_available)))
{
	assert(r > 0 && c > 0);	
	set_wall();
}

void ScreenData::set_wall()
{
	int r = row(), c = col();
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
		{
			if (i == 0 || i == r - 1 || j == 0 || j == c - 1)
			{
                data[i][j] = Node_wall;
			}
			else
			{
                data[i][j] = Node_available;
			}
		}
	}
}

int ScreenData::row()
{
	return data.size();
}
	
int ScreenData::col()
{
	return data[0].size();
}
	
Node_t ScreenData::get_type(int r, int c)
{
	assert(r >= 0 && r < row());
	assert(c >= 0 && c < col());
	
    return data[r][c];
}

void ScreenData::set_type(int r, int c, Node_t node_type)
{
	assert(r >= 0 && r < row());
	assert(c >= 0 && c < col());
	
    data[r][c] = node_type;
}










