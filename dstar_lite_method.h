#ifndef DSTARLITEMETHOD_H
#define DSTARLITEMETHOD_H


#include "route_method.h"


class DStarLiteMethod : public RouteMethod
{
public:
    DStarLiteMethod(Snake& snake, ScreenData& data, Seed& seed) : RouteMethod(snake, data, seed) {}
    bool findNext(std::vector<int>& from);
};

#endif // DSTARLITEMETHOD_H
