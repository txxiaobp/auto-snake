#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "screen_data.h"
#include <map>
#include <vector>

class Obstacle
{
public:
    Obstacle(ScreenData& screenData);
    void resetObstacle();
    void clearObstacle();
    void getObstacleData(std::vector<int>& obstacleNodes);

private:
    void setObstacle();
    std::pair<int,int> randSetObstacle();
    bool isAllAvailableNodesConnected();
    void setObstacleNode(int nodeRow, int nodeCol);
    void clearObstacleNode();

private:
    ScreenData& screenData;
    std::vector<int> obstacleNodes;
};

#endif // OBSTACLE_H
