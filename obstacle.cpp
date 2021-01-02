#include "obstacle.h"
#include <cstdlib>
#include <ctime>

const float NUM_PERCENT = 0.1;

Obstacle::Obstacle(ScreenData& screenData)
    : screenData(screenData)
{
    int row = screenData.getRow();
    int col = screenData.getCol();

    int obstacleNum = int(NUM_PERCENT * row * col);

    int directions[4][2] = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    srand(time(NULL));
    while (obstacleNum > 0)
    {
        int randNum = rand() % obstacleNum;


        std::pair<int,int> nodePair = randSetObstacle();
        int nextDirection = rand() % 4;
        int nextRow = nodePair.first + directions[nextDirection][0];
        int nextCol = nodePair.second + directions[nextDirection][1];
        int i = 0;
        for (i = 0; i < randNum; i++)
        {
            if (screenData.getType(nextRow, nextCol) != Node_available)
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
            }
            screenData.setType(nextRow, nextCol, Node_available);
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































