#include <cassert>
#include <iostream>
#include <QDebug>
#include "bfs_method.h"
#include "debfs_method.h"
#include "dijkstra_method.h"
#include "astar_method.h"
#include "dstar_lite_method.h"
#include "dstar_method.h"
#include "data_recorder.h"
#include "snake.h"
#include <string>


Snake::Snake(Seed& seed, ScreenData& screenData, DataRecorder& dataRecorder, ModeSelection& modeSelection, AlgorithmSelection& algorithmSelection) :
    screenData(screenData)
    , dataRecorder(dataRecorder)
    , modeSelection(modeSelection)
    , algorithmSelection(algorithmSelection)
    , seed(seed)
    , last_direction(DIRECTION_MAX)
{
    int row = screenData.getRow();
    int col = screenData.getCol();

    if (modeSelection.getMode() != MODE_MANUAL)
    {
        last_direction = DIRECTION_RIGHT;
    }

    push(row / 2, col / 2 - 1, NODE_SNAKE_BODY);       //snake end
    push(row / 2, col / 2, NODE_SNAKE_HEAD, true);     //snake head
    walkedCount = 0;

    method[ALGORITHM_BFS] = new BFSMethod(*this, screenData, seed);
    method[ALGORITHM_DEBFS] = new DeBFSMethod(*this, screenData, seed);
    method[ALGORITHM_DIJKSTRA] = new DijkstraMethod(*this, screenData, seed);
    method[ALGORITHM_ASTAR] = new AStarMethod(*this, screenData, seed);
    method[ALGORITHM_DSTAR] = new DStarMethod(*this, screenData, seed);
//    method[ALGORITHM_DSTAR_LITE] = new DStarLiteMethod(*this, screenData, seed);
}

Snake::~Snake()
{
    for (int i = 0; i < ALGORITHM_MAX; i++)
    {
        if (method[i])
        {
            delete method[i];
            method[i] = nullptr;
        }
    }
}

void Snake::push(int row, int col, Node_t node_type, bool initial)
{
    if (node_type == NODE_SNAKE_HEAD)
    {
        if (!initial)
        {
            setData(getNum(row, col));
        }

        if (!initial)
        {
            screenData.setType(headRow, headCol, NODE_SNAKE_BODY);
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
    screenData.setType(getRow(tail), getCol(tail), NODE_AVAILABLE);
}

bool Snake::move()
{
    bool ret = false;

    if (modeSelection.getMode() == MODE_REPLAY)
    {
        if (!dataRecorder.isDataEmpty())
        {
            int nextNode = dataRecorder.popSnakeData();
            setDirection(nextNode);
            toNextNode(nextNode);
            ret = true;
        }
        else
        {
            ret = false;
        }
    }
    else if (modeSelection.getMode() == MODE_MANUAL)
    {
        int nextNode = moveDirection(last_direction);
        if (-1 == nextNode)
        {
            ret = false;
        }
        else
        {
            toNextNode(nextNode);
            ret = true;
        }
    }
    else
    {
        bool hasPath = false;
        int nextNode = 0;

        if (method[algorithmSelection.getAlgo()])
            hasPath = method[algorithmSelection.getAlgo()]->getNext(nextNode);

        if (hasPath)
        {
            screenData.setType(headRow, headCol, NODE_SNAKE_BODY);
            setDirection(nextNode);
            toNextNode(nextNode);
            ret = true;
        }
        else
        {
            int direction = last_direction;
            for (int i = 0; i < DIRECTION_MAX; i++, direction = (direction + 1) % DIRECTION_MAX)
            {
                int node = moveDirection(direction);
                if (node != -1)
                {
                    last_direction = Direction_e(direction);
                    push(getRow(node), getCol(node), NODE_SNAKE_HEAD);
                    pop();
                    ret = true;
                    break;
                }
            }
        }
    }

    if (ret)
    {
        walkedCount++;
        emit snakeWalkedIncrease(walkedCount);
    }
    return ret;
}

int Snake::getNum(int nodeX, int nodeY)
{
    return nodeX * screenData.getCol() + nodeY;
}

int Snake::getRow(int node)
{
    return node / screenData.getCol();
}

int Snake::getCol(int node)
{
    return node % screenData.getCol();
}

void Snake::setDirection(int node)
{
    int nodeRow = getRow(node);
    int nodeCol = getCol(node);

    if (headRow + 1 == nodeRow && headCol == nodeCol)
    {
        last_direction = DIRECTION_DOWN;
    }
    else if (headRow - 1 == nodeRow && headCol == nodeCol)
    {
        last_direction = DIRECTION_UP;
    }
    else if (headRow == nodeRow && headCol == nodeCol + 1)
    {
        last_direction = DIRECTION_LEFT;
    }
    else if (headRow== nodeRow && headCol == nodeCol - 1)
    {
        last_direction = DIRECTION_RIGHT;
    }
}

void Snake::setDirection(Direction_e direction)
{
    if (modeSelection.getMode() != MODE_MANUAL)
    {
        return;
    }
    if ((last_direction == DIRECTION_UP && direction == DIRECTION_DOWN) ||
            (last_direction == DIRECTION_DOWN && direction == DIRECTION_UP) ||
            (last_direction == DIRECTION_LEFT && direction == DIRECTION_RIGHT) ||
            (last_direction == DIRECTION_RIGHT && direction == DIRECTION_LEFT))
    {
        return;
    }

    last_direction = direction;
}

Direction_e Snake::getDirection()
{
    return last_direction;
}

int Snake::moveDirection(int direction)
{
    int nodeRow = 0, nodeCol = 0;
    switch(direction)
    {
    case DIRECTION_UP:
        nodeRow = headRow - 1;
        nodeCol = headCol;
        break;
    case DIRECTION_DOWN:
        nodeRow = headRow + 1;
        nodeCol = headCol;
        break;
    case DIRECTION_LEFT:
        nodeRow = headRow;
        nodeCol = headCol - 1;
        break;
    case DIRECTION_RIGHT:
        nodeRow = headRow;
        nodeCol = headCol + 1;
        break;
    }
    if (screenData.getType(nodeRow, nodeCol) == NODE_AVAILABLE || screenData.getType(nodeRow, nodeCol) == NODE_SEED)
    {
        return getNum(nodeRow, nodeCol);
    }
    else
    {
        return -1;
    }
}

void Snake::reset()
{
    while (!snake.empty())
    {
        pop();
    }

    int row = screenData.getRow();
    int col = screenData.getCol();

    push(row / 2, col / 2 - 1, NODE_SNAKE_BODY);       //snake end
    push(row / 2, col / 2, NODE_SNAKE_HEAD, true);     //snake head
    walkedCount = 0;
    last_direction = DIRECTION_RIGHT;

    if (modeSelection.getMode() != MODE_REPLAY)
    {
        seed.setData();
    }
    else
    {
        seed.setFromRecorder();
    }
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

void Snake::setData(int node)
{
    dataRecorder.pushSnakeData(node);
}

void Snake::toNextNode(int nextNode)
{
    if (nextNode == seed.getNode())
    {
        seed.set();     
        push(getRow(nextNode), getCol(nextNode), NODE_SNAKE_HEAD);
        emit this->snakeLengthIncrease(snake.size());
    }
    else
    {
        push(getRow(nextNode), getCol(nextNode), NODE_SNAKE_HEAD);
        pop();
    }
}
