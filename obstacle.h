#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "screen_data.h"
#include "direction.h"
#include <map>
#include <vector>

class Obstacle
{
public:
    Obstacle(ScreenData& screenData, bool isMovable = false);
    void resetObstacle();
    void clearObstacle();
    void getObstacleData(std::vector<int>& obstacleNodes);
    void getInitMovableObstacle(std::vector<std::pair<int,Direction_e>> &movableObstacleNodes);
    void setMovableObstacle(std::vector<std::pair<int,Direction_e>> &movableObstacleNodes);
    void move();
    void setMovable(bool isMovable);
    void resetMovableObstacle();

private:
    void setObstacle();
    std::pair<int,int> randSetObstacle();
    bool isAllAvailableNodesConnected();
    void setObstacleNode(int nodeRow, int nodeCol);
    void clearObstacleNode();
    void singleNodeMoveNext(std::pair<int,Direction_e> &nodeDirectionPair);
    int getNextNode(int node, Direction_e direction);

private:
    ScreenData& screenData;
    std::vector<int> obstacleNodes;
    bool isMovable;
    std::vector<std::pair<int,Direction_e>> movableObstacles;
    std::vector<std::pair<int,Direction_e>> initMovableObstacles;
};

#endif // OBSTACLE_H
