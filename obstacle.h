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
    void getObstacleData(std::vector<int>& obstacleNode);

private:
    void setObstacle();
    std::pair<int,int> randSetObstacle();
    bool isAllAvailableNodesConnected();

private:
    ScreenData& screenData;
};

#endif // OBSTACLE_H
