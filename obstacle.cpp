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
            screenData.setType(row, col, NODE_OBSTACLE);
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

std::pair<int,Direction_e> Obstacle::getNextNode(int node, Direction_e direction)
{
    std::pair<int,int> currNodePair = screenData.getNodePair(node);

    int currRow = currNodePair.first;
    int currCol = currNodePair.second;

    int nextRow = currRow + directions[int(direction)][0];
    int nextCol = currCol + directions[int(direction)][1];
    return std::make_pair(screenData.getNode(nextRow, nextCol), direction);
}

std::pair<int,Direction_e> Obstacle::getNextNodeRefraction(const std::pair<int,Direction_e> &nodePair)
{
    std::pair<int,Direction_e> nextNodePair = getNextNode(nodePair.first, nodePair.second);
    assert(screenData.getType(nextNodePair.first) != NODE_AVAILABLE);

    //获取折射方向
    Direction_e nextDirection = getRefractionDirection(nextNodePair.second);

    //根据折射方向，获取下一点的位置
    std::pair<int,Direction_e> nextRefractionNodePair = std::make_pair(nextNodePair.first, nextDirection);

    if (!checkRefractionNode(nextRefractionNodePair, nodePair))
    {
        nextDirection = getReflectDirection(nextDirection);
        return getNextNode(nextNodePair.first, nextDirection);
    }
    return getNextNode(nextNodePair.first, nextDirection);
}

std::pair<int,Direction_e> Obstacle::getNextNodeRreflect(const std::pair<int,Direction_e> &nodePair)
{
    Direction_e nextDirection = getReflectDirection(nodePair.second);
    return getNextNode(nodePair.first, nextDirection);
}

void Obstacle::setNodePair(std::pair<int,Direction_e> &currPair, std::pair<int,Direction_e> &nextPair)
{
    screenData.setType(currPair.first, NODE_AVAILABLE);
    currPair.first = nextPair.first;
    currPair.second = nextPair.second;
    screenData.setType(currPair.first, NODE_OBSTACLE);

    qDebug() << currPair.first << "  " << currPair.second;
}

bool Obstacle::checkRefractionNode(std::pair<int,Direction_e> &nextRefractionNodePair, const std::pair<int,Direction_e> &nodeDirectionPair)
{
    std::pair<int,int> nextNodePair = screenData.getNodePair(nextRefractionNodePair.first);
    std::pair<int,int> currNodePair = screenData.getNodePair(nodeDirectionPair.first);

    int nextRow = nextNodePair.first;
    int nextCol = nextNodePair.second;

    int currRow = currNodePair.first;
    int currCol = currNodePair.second;

    int midRow = (currRow + nextRow) / 2;
    int midCol = (currCol + nextCol) / 2;

    if (screenData.getType(midRow, midCol) != NODE_AVAILABLE)
    {
        return false;
    }
    return true;
}

void Obstacle::singleNodeMoveNext(std::pair<int,Direction_e> &nodeDirectionPair)
{
    std::pair<int,Direction_e> nextNodePair = getNextNode(nodeDirectionPair.first, nodeDirectionPair.second);

    if (screenData.getType(nextNodePair.first) != NODE_AVAILABLE)
    {
        std::pair<int,Direction_e> nextRefractionNodePair = getNextNodeRefraction(nodeDirectionPair);

        if (screenData.getType(nextRefractionNodePair.first) != NODE_AVAILABLE)
        {
            std::pair<int,Direction_e> nextReflectNodePair = getNextNodeRreflect(nextNodePair);
            setNodePair(nodeDirectionPair, nextReflectNodePair);
        }
        else
        {
            setNodePair(nodeDirectionPair, nextRefractionNodePair);
        }
    }
    else
    {
        setNodePair(nodeDirectionPair, nextNodePair);
    }
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






















