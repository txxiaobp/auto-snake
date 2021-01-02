#ifndef ASTARMETHOD_H
#define ASTARMETHOD_H

#include "route_method.h"
#include <map>

class AStarMethod : public RouteMethod
{
public:
    AStarMethod(Snake& snake, ScreenData& data, Seed& seed) : RouteMethod(snake, data, seed) { }

private:
    bool findNext(std::vector<int>& from);

private:
    std::pair<int,int> calDistance(int nextNode, int curNode);
};

#endif // ASTARMETHOD_H
