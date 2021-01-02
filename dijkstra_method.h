#ifndef DIJSTRAMETHOD_H
#define DIJSTRAMETHOD_H


#include "route_method.h"

class DijkstraMethod : public RouteMethod
{
public:
    DijkstraMethod(Snake& snake, ScreenData& data, Seed& seed) : RouteMethod(snake, data, seed) { RouteMethod::findNext(); }

private:
    bool findNext(std::vector<int>& from);

private:
    int calDistance(int nextNode, int curNode);
};

#endif // DIJSTRAMETHOD_H
