#ifndef ASTARMETHOD_H
#define ASTARMETHOD_H

#include "route_method.h"

class AStarMethod : public RouteMethod
{
public:
    AStarMethod(Snake& snake, ScreenData& data, Seed& seed) : RouteMethod(snake, data, seed) {}
    bool findNext(std::vector<int>& from);

private:
    int calDistance(int nextNode, int curNode);
};

#endif // ASTARMETHOD_H
