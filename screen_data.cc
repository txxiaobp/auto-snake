#include "screen_data.h"
#include <cstdlib>
#include <ctime>
#include <cassert>
#include "seed.h"
#include <QDebug>

ScreenData::ScreenData(int r, int c) :
    data(std::vector<std::vector<Node_t> >(r, std::vector<Node_t>(c, Node_available)))
{
	assert(r > 0 && c > 0);	
    set_wall();
}

void ScreenData::reset(int newRow, int newCol)
{
    data = std::vector<std::vector<Node_t> >(newRow, std::vector<Node_t>(newCol, Node_available));

    set_wall();
}

void ScreenData::set_wall()
{
    int r = getRow(), c = getCol();
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

int ScreenData::getRow()
{
	return data.size();
}
	
int ScreenData::getCol()
{
	return data[0].size();
}
	
Node_t ScreenData::getType(int r, int c)
{
    assert(r >= 0 && r < getRow());
    assert(c >= 0 && c < getCol());
	
    return data[r][c];
}

Node_t ScreenData::getType(int node)
{
    int r = node / getCol();
    int c = node % getCol();
    assert(r >= 0 && r < getRow());
    assert(c >= 0 && c < getCol());

    return data[r][c];
}

void ScreenData::setType(int node, Node_t node_type)
{
    int row = node / getCol();
    int col = node % getCol();
    setType(row, col, node_type);
}

void ScreenData::setType(int r, int c, Node_t node_type)
{
    assert(r >= 0 && r < getRow());
    assert(c >= 0 && c < getCol());
	
    data[r][c] = node_type;
}










