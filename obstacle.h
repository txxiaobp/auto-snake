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
    void move();
    void setMovable(bool isMovable);

private:
    void setObstacle();
    std::pair<int,int> randSetObstacle();
    bool isAllAvailableNodesConnected();
    void setObstacleNode(int nodeRow, int nodeCol);
    void clearObstacleNode();
    void singleNodeMoveNext(std::pair<int,Direction_e> &nodeDirectionPair);
    std::pair<int,Direction_e> getNextNode(int node, Direction_e direction);
    std::pair<int,Direction_e> getNextNodeRefraction(const std::pair<int,Direction_e> &nodePair);
    std::pair<int,Direction_e> getNextNodeRreflect(const std::pair<int,Direction_e> &nodePair);
    void setNodePair(std::pair<int,Direction_e> &currPair, std::pair<int,Direction_e> &nextPair);

private:
    ScreenData& screenData;
    std::vector<int> obstacleNodes;
    bool isMovable;
    std::vector<std::pair<int,Direction_e>> movableObstacle;
};

#endif // OBSTACLE_H
