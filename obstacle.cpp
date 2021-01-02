#include "obstacle.h"
#include <cstdlib>
#include <ctime>
#include "direction.h"
#include <QDebug>

const float NUM_PERCENT = 0.05;
const int OBSTACLE_NUM_A_TIME = 8;
const int OBSTACLE_NUM_IN_DIRECTION = 4;

Obstacle::Obstacle(ScreenData& screenData)
    : screenData(screenData)
{
    clearObstacle();
    setObstacle();
}

void Obstacle::resetObstacle()
{
    clearObstacle();
    setObstacle();
}


void Obstacle::clearObstacle()
{
    int row = screenData.getRow();
    int col = screenData.getCol();
    for (int i = 1; i < row - 1; i++)
    {
        for (int j = 1; j < col - 1; j++)
        {
            if (screenData.getType(i, j) == Node_obstacle)
            {
                screenData.setType(i, j, Node_available);
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
            if (directionCount >= OBSTACLE_NUM_IN_DIRECTION || screenData.getType(nextRow, nextCol) != Node_available)
            {
                int j;
                for (j = 0; j < 3; j++)
                {
                    nextDirection = (nextDirection + 1) % 4;
                    nextRow += directions[nextDirection][0];
                    nextCol += directions[nextDirection][1];
                    if (screenData.getType(nextRow, nextCol) == Node_available)
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
            screenData.setType(nextRow, nextCol, Node_obstacle);
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
    } while(screenData.getType(row, col) != Node_available);


    screenData.setType(row, col, Node_obstacle);
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
            if (screenData.getType(i, j) == Node_obstacle)
            {
                obstacleNode.push_back(col * i + j);
            }
        }
    }
}






























