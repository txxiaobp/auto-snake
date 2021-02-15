#ifndef DSTARMETHOD_H
#define DSTARMETHOD_H


#include "route_method.h"
#include "direction.h"
#include <vector>
#include <unordered_map>

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
    std::unordered_map<int,Direction_e> walkedNodes;
};

#endif // DSTARMETHOD_H
