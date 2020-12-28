#ifndef DSTARMETHOD_H
#define DSTARMETHOD_H


#include "route_method.h"


class DStarMethod : public RouteMethod
{
public:
    DStarMethod(Snake& snake, ScreenData& data, Seed& seed) : RouteMethod(snake, data, seed) {}
    bool findNext(std::vector<int>& from);
};

#endif // DSTARMETHOD_H
