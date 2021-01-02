#include "obstacle.h"
#include <cstdlib>
#include <ctime>
#include "direction.h"
#include <QDebug>
#include <queue>

const float NUM_PERCENT = 0.05;
const int OBSTACLE_NUM_A_TIME = 8;
const int OBSTACLE_NUM_IN_DIRECTION = 4;

Obstacle::Obstacle(ScreenData& screenData)
    : screenData(screenData)
{
    do
    {
        clearObstacle();
        setObstacle();
    } while (!isAllAvailableNodesConnected());
}

void Obstacle::resetObstacle()
{
    int i = 0;
    do
    {
        clearObstacle();
        setObstacle();
        i++;
    } while (!isAllAvailableNodesConnected());
}

void Obstacle::clearObstacle()
{
    int row = screenData.getRow();
    int col = screenData.getCol();
    for (int i = 1; i < row - 1; i++)
    {
        for (int j = 1; j < col - 1; j++)
        {
            if (screenData.getType(i, j) == NODE_OBSTACLE)
            {
                screenData.setType(i, j, NODE_AVAILABLE);
            }
        }
    }
}

void Obstacle::setObstacle()
{
    int row = screenData.getRow();
    int col = screenData.getCol();

    int obstacleNum = int(NUM_PERCENT * row * col);

    srand(time(NULL));
    while (obstacleNum > 0)
    {
        int randNum;

        if (obstacleNum > OBSTACLE_NUM_A_TIME)
        {
            randNum = OBSTACLE_NUM_A_TIME;
        }
        else
        {
            randNum = obstacleNum;
        }

        std::pair<int,int> nodePair = randSetObstacle();
        int nextDirection = rand() % 4;
        int nextRow = nodePair.first + directions[nextDirection][0];
        int nextCol = nodePair.second + directions[nextDirection][1];
        int i = 0;
        int directionCount = 1;
        for (i = 0; i < randNum; i++, directionCount++)
        {
            if (directionCount >= OBSTACLE_NUM_IN_DIRECTION || screenData.getType(nextRow, nextCol) != NODE_AVAILABLE)
            {
                int j;
                for (j = 0; j < 3; j++)
                {
                    nextDirection = (nextDirection + 1) % 4;
                    nextRow += directions[nextDirection][0];
                    nextCol += directions[nextDirection][1];
                    if (screenData.getType(nextRow, nextCol) == NODE_AVAILABLE)
                    {
                        break;
                    }
                }
                if (j == 3)
                {
                    break;
                }
                directionCount = 0;
            }
            screenData.setType(nextRow, nextCol, NODE_OBSTACLE);
            nextRow += directions[nextDirection][0];
            nextCol += directions[nextDirection][1];
        }
        obstacleNum -= i;
    }
}


std::pair<int,int> Obstacle::randSetObstacle()
{
    srand(time(NULL));
    int row_bound = screenData.getRow();
    int col_bound = screenData.getCol();

    int row, col;
    do
    {
        row = rand() % row_bound;
        col = rand() % col_bound;
    } while(screenData.getType(row, col) != NODE_AVAILABLE);


    screenData.setType(row, col, NODE_OBSTACLE);
    return std::make_pair(row, col);
}

void Obstacle::getObstacleData(std::vector<int>& obstacleNode)
{
    obstacleNode.clear();
    int row = screenData.getRow();
    int col = screenData.getCol();
    for (int i = 1; i < row - 1; i++)
    {
        for (int j = 1; j < col - 1; j++)
        {
            if (screenData.getType(i, j) == NODE_OBSTACLE)
            {
                obstacleNode.push_back(col * i + j);
            }
        }
    }
}


bool Obstacle::isAllAvailableNodesConnected()
{
    std::vector<bool> visited(screenData.getRow() * screenData.getCol(), false);
    int connectNum = 0;
    for (int row = 1; row < screenData.getRow() - 1; row++)
    {
        for (int col = 1; col < screenData.getCol() - 1; col++)
        {
            if (screenData.getType(row, col) == NODE_OBSTACLE || screenData.getType(row, col) == NODE_WALL)
            {
                continue;
            }
            int node = row * screenData.getCol() + col;
            if (connectNum > 1)
            {
                return false;
            }
            else if (!visited[node])
            {
                connectNum++;
                visited[node] = true;
                std::queue<int> queue;
                queue.push(node);

                while (!queue.empty())
                {
                    int queueNode = queue.front();
                    queue.pop();

                    int queueRow = queueNode / screenData.getCol();
                    int queueCol = queueNode % screenData.getCol();

                    for (int i = 0; i < DIRECTION_COUNT; i++)
                    {
                        int nextRow = queueRow + directions[i][0];
                        int nextCol = queueCol + directions[i][1];
                        int nextNode = nextRow * screenData.getCol() + nextCol;

                        if (screenData.inArea(nextRow, nextCol)
                                && screenData.getType(nextNode) != NODE_WALL
                                && screenData.getType(nextRow, nextCol) != NODE_OBSTACLE
                                && !visited[nextNode])
                        {
                            visited[nextNode] = true;
                            queue.push(nextNode);
                        }
                    }
                }
            }
        }
    }
    return connectNum == 1;
}



























