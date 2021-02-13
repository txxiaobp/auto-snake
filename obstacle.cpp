#include "obstacle.h"
#include <cstdlib>
#include <ctime>
#include "direction.h"
#include <QDebug>
#include <queue>

const float NUM_PERCENT = 0.05;
const int OBSTACLE_NUM_A_TIME = 8;
const int OBSTACLE_NUM_IN_DIRECTION = 4;

Obstacle::Obstacle(ScreenData& screenData, bool isMovable)
    : screenData(screenData)
    , isMovable(isMovable)
{
    do
    {
        clearObstacle();
        setObstacle();
    } while (!isAllAvailableNodesConnected());

    setMovable(isMovable);
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
    setMovable(isMovable);
}

void Obstacle::clearObstacle()
{
    setMovable(false);
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
    clearObstacleNode();
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
                for (j = 0; j < DIRECTION_COUNT - 1; j++)
                {
                    nextDirection = (nextDirection + 1) % 4;
                    nextRow += directions[nextDirection][0];
                    nextCol += directions[nextDirection][1];
                    if (screenData.inArea(nextRow, nextCol) && screenData.getType(nextRow, nextCol) == NODE_AVAILABLE)
                    {
                        break;
                    }
                }
                if (j == DIRECTION_COUNT - 1)
                {
                    break;
                }
                directionCount = 0;
            }
            screenData.setType(nextRow, nextCol, NODE_OBSTACLE);
            setObstacleNode(nextRow, nextCol);
            nextRow += directions[nextDirection][0];
            nextCol += directions[nextDirection][1];
        }
        obstacleNum -= i;
    }
}


std::pair<int,int> Obstacle::randSetObstacle()
{
    srand(time(NULL));
    int rowBound = screenData.getRow();
    int colBound = screenData.getCol();

    int row, col;
    do
    {
        row = rand() % rowBound;
        col = rand() % colBound;
    } while(screenData.getType(row, col) != NODE_AVAILABLE);


    screenData.setType(row, col, NODE_OBSTACLE);
    return std::make_pair(row, col);
}

void Obstacle::getObstacleData(std::vector<int>& obstacleNodes)
{
    obstacleNodes.clear();
    for (int row = 1; row < screenData.getRow() - 1; row++)
    {
        for (int col = 1; col < screenData.getCol() - 1; col++)
        {
            if (screenData.getType(row, col) == NODE_OBSTACLE)
            {
                obstacleNodes.push_back(screenData.getNode(row, col));
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

void Obstacle::setObstacleNode(int nodeRow, int nodeCol)
{
    obstacleNodes.push_back(screenData.getNode(nodeRow, nodeCol));
}

void Obstacle::clearObstacleNode()
{
    obstacleNodes.clear();
}

void Obstacle::setMovable(bool isMovable)
{
    const int rowBound = screenData.getRow();
    const int colBound = screenData.getCol();
    this->isMovable = isMovable;
    if (isMovable)
    {
        int obstacleNum = std::min(screenData.getRow(), screenData.getCol()) / 4;

        srand(time(NULL));
        movableObstacle.clear();
        for (int i = 0; i < obstacleNum; i++)
        {
            int row, col;
            do
            {
                row = rand() % rowBound;
                col = rand() % colBound;
            } while(screenData.getType(row, col) != NODE_AVAILABLE);

            movableObstacle.push_back( std::make_pair(screenData.getNode(row, col), Direction_e(rand() % DIRECTION_MAX)) );
            screenData.setType(row, col, NODE_MOVABLE_OBSTACLE);
        }
    }
    else
    {
        for (auto p : movableObstacle)
        {
            screenData.setType(p.first, NODE_AVAILABLE);
        }
        movableObstacle.clear();
    }
}

int Obstacle::getNextNode(int node, Direction_e direction)
{
    std::pair<int,int> currNodePair = screenData.getNodePair(node);

    int currRow = currNodePair.first;
    int currCol = currNodePair.second;

    int nextRow = currRow + directions[int(direction)][0];
    int nextCol = currCol + directions[int(direction)][1];
    return screenData.getNode(nextRow, nextCol);
}


void Obstacle::singleNodeMoveNext(std::pair<int,Direction_e> &nodeDirectionPair)
{
    screenData.setType(nodeDirectionPair.first, NODE_AVAILABLE);

    int nextNode = getNextNode(nodeDirectionPair.first, nodeDirectionPair.second);

    if (screenData.getType(nextNode) != NODE_AVAILABLE)
    {
        //若下一点不是空白，需要反弹
        //折射方向
        Direction_e refraction = getRefractionDirection(nodeDirectionPair.second);
        //折射方向的下一点
        int nextNodeRefraction = getNextNode(nodeDirectionPair.first, refraction);

        if (screenData.getType(nextNodeRefraction) != NODE_AVAILABLE)
        {
            //折射方向的下一点不可到达，判断折射方向的反方向
            Direction_e refractionReflect = getReflectDirection(refraction);
            int nextNodeRefractionReflect = getNextNode(nodeDirectionPair.first, refractionReflect);

            if (screenData.getType(nextNodeRefractionReflect) != NODE_AVAILABLE)
            {
                //折射方向反方向的下一点也不可到达，原路反弹
                nodeDirectionPair.second = getReflectDirection(nodeDirectionPair.second);
            }
            else
            {
                nodeDirectionPair.second = refractionReflect;
            }
        }
        else
        {
            nodeDirectionPair.second = refraction;
        }
    }
    else
    {
        nodeDirectionPair.first = nextNode;
    }
    screenData.setType(nodeDirectionPair.first, NODE_MOVABLE_OBSTACLE);
}

void Obstacle::move()
{
    if (!isMovable)
    {
        return;
    }

    for (auto &p : movableObstacle)
    {
        singleNodeMoveNext(p);
    }
}






















