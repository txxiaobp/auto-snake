#ifndef DSTARMETHOD_H
#define DSTARMETHOD_H


#include "route_method.h"
#include <vector>

class DStarMethod : public RouteMethod
{
public:
    DStarMethod(Snake& snake, ScreenData& data, Seed& seed);

private:
    bool findNext(std::vector<int>& from);
    void replan();
    int lastSeedNode;
    std::vector<int> disToSeed;
    std::vector<bool> pastNodes;
};

#endif // DSTARMETHOD_H
