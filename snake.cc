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
    reset();

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
            screenData.setType(headRow, headCol, Node_snake_body);
        }
        headRow = row;
        headCol = col;
    }
    screenData.setType(row, col, node_type);
    snake.push(getNum(row, col));
}

void Snake::pop()
{
    int tail = snake.front();
	snake.pop();
    screenData.setType(getRow(tail), getCol(tail), Node_available);
}

std::vector<int> Snake::aroundNodes(int node)
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

    bool hasPath = false;
    if (method[cur_method])
        hasPath = method[cur_method]->findNext(from);

    bool ret = false;
    if (hasPath)
	{
        screenData.setType(headRow, headCol, Node_snake_body);
        int node = seed.getNode();

        while (from[node] != getNum(headRow, headCol))
		{
            node = from[node];
		}
        setDirection(node);
        if (node == seed.getNode())
		{
			seed.set();
            push(getRow(node), getCol(node), Node_snake_head);
		}
		else
		{
            push(getRow(node), getCol(node), Node_snake_head);
			pop();	
		}

        ret = true;
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
                ret = true;
                break;
			}
		}
	}
    if (ret)
    {
        walkedCount++;
    }
    return ret;
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
    if (screenData.getType(nodeRow, nodeCol) == Node_available)
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

void Snake::reset()
{
    while (!snake.empty())
    {
        pop();
    }

    int row = screenData.row();
    int col = screenData.col();

    push(row / 2, col / 2 - 1, Node_snake_body);       //snake end
    push(row / 2, col / 2, Node_snake_head, true);     //snake head
    walkedCount = 0;
}

int Snake::getSize()
{
    return snake.size();
}

int Snake::getEatenCount()
{
    return getSize() - 2;
}

int Snake::getWalkedCount()
{
    return walkedCount;
}

