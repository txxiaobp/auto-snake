#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "screen_data.h"
#include <map>

class Obstacle
{
public:
    Obstacle(ScreenData& screenData);

private:
    std::pair<int,int> randSetObstacle();

private:
    ScreenData& screenData;
};

#endif // OBSTACLE_H
