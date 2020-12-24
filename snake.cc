#include <cassert>
#include <iostream>
#include <QDebug>
#include "bfs_method.h"
#include "dijkstra_method.h"
#include "snake.h"
#include <string>


std::string method_name_map[METHOD_MAX] = {
    "BFS",
    "Dijkstra"
};


Snake::Snake(Seed& seed, ScreenData& screenData) :
	screenData(screenData),
	seed(seed),
    last_direction(DIRECTION_MAX),
    cur_method(BFS_METHOD)
{
	int row = screenData.row();
	int col = screenData.col();

    push(row / 2, col / 2 - 1, Node_snake_body);       //snake end
    push(row / 2, col / 2, Node_snake_head, true);     //snake head

    method[BFS_METHOD] = new BFSMethod(*this, screenData, seed);
    method[DIJKSTRA_METHOD] = new DijkstraMethod(*this, screenData, seed);
}

Snake::~Snake()
{
    for (int i = 0; i < METHOD_MAX; i++)
    {
        delete method[i];
        method[i] = nullptr;
    }
}

void Snake::push(int row, int col, Node_t node_type, bool initial)
{
    if (node_type == Node_snake_head)
    {
        if (!initial)
        {
            screenData.set_type(headRow, headCol, Node_snake_body);
        }
        headRow = row;
        headCol = col;
    }
	screenData.set_type(row, col, node_type);
    snake.push(getNum(row, col));
}

void Snake::pop()
{
    int tail = snake.front();
	snake.pop();
    screenData.set_type(getRow(tail), getCol(tail), Node_available);
}

std::vector<int> Snake::around_nodes(int node)
{
    std::vector<int> ret;
    int node_row = getRow(node);
    int node_col = getCol(node);
	
    ret.push_back(getNum(node_row + 1, node_col));
    ret.push_back(getNum(node_row - 1, node_col));
    ret.push_back(getNum(node_row, node_col + 1));
    ret.push_back(getNum(node_row, node_col - 1));
	
	return ret;
}

bool Snake::move()
{
    int row = screenData.row();
    int col = screenData.col();
    std::vector<int> from(row * col, -1);

    bool has_path = false;
    if (method[cur_method])
        has_path = method[cur_method]->findNext(from);

    if (has_path)
	{
        screenData.set_type(headRow, headCol, Node_snake_body);
        int node = seed.get_node();

        while (from[node] != getNum(headRow, headCol))
		{
            node = from[node];
		}
        setDirection(node);
		if (node == seed.get_node())
		{
			seed.set();
            push(getRow(node), getCol(node), Node_snake_head);
		}
		else
		{
            push(getRow(node), getCol(node), Node_snake_head);
			pop();	
		}

		return true;
	}
	else
	{
        int direction = last_direction;
        for (int i = 0; i < DIRECTION_MAX; i++, direction = (direction + 1) % DIRECTION_MAX)
		{
            int node = move_direction(direction);
            if (node != -1)
			{
				last_direction = direction_e(direction);
                push(getRow(node), getCol(node), Node_snake_head);
                pop();
				return true;
			}
		}
		return false;	
	}
}

int Snake::getNum(int nodeX, int nodeY)
{
    return nodeX * screenData.col() + nodeY;
}

int Snake::getRow(int node)
{
    return node / screenData.col();
}

int Snake::getCol(int node)
{
    return node % screenData.col();
}

void Snake::setDirection(int node)
{
    int nodeRow = getRow(node);
    int nodeCol = getCol(node);

    if (headRow + 1 == nodeRow && headCol == nodeCol)
    {
        last_direction = DOWN;
    }
    else if (headRow - 1 == nodeRow && headCol == nodeCol)
    {
        last_direction = UP;
    }
    else if (headRow == nodeRow && headCol == nodeCol + 1)
    {
        last_direction = LEFT;
    }
    else if (headRow== nodeRow && headCol == nodeCol - 1)
    {
        last_direction = RIGHT;
    }
}

int Snake::move_direction(int direction)
{
    int nodeRow = 0, nodeCol = 0;
    switch(direction)
    {
    case UP:
        nodeRow = headRow - 1;
        nodeCol = headCol;
        break;
    case DOWN:
        nodeRow = headRow + 1;
        nodeCol = headCol;
        break;
    case LEFT:
        nodeRow = headRow;
        nodeCol = headCol - 1;
        break;
    case RIGHT:
        nodeRow = headRow;
        nodeCol = headCol + 1;
        break;
    }
    if (screenData.get_type(nodeRow, nodeCol) == Node_available)
    {
        return getNum(nodeRow, nodeCol);
    }
    else
    {
        return -1;
    }
}

void Snake::setMethod(search_method_e m)
{
    cur_method = m;
    std::cout << "set method to " << method_name_map[cur_method] << std::endl;
}

search_method_e Snake::getMethod()
{
    return cur_method;
}

