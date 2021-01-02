#ifndef BFSMETHOD_H
#define BFSMETHOD_H

#include "route_method.h"

class BFSMethod : public RouteMethod
{
public:
    BFSMethod(Snake& snake, ScreenData& data, Seed& seed) : RouteMethod(snake, data, seed) { }

private:
    bool findNext(std::vector<int>& from);
};

#endif // BFSMETHOD_H
