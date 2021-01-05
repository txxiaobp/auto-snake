#ifndef DEBFSMETHOD_H
#define DEBFSMETHOD_H


#include "route_method.h"


class DeBFSMethod : public RouteMethod
{
public:
    DeBFSMethod(Snake& snake, ScreenData& data, Seed& seed) : RouteMethod(snake, data, seed) { }

private:
    bool findNext(std::vector<int>& from);
    void generateFrom(std::vector<int>& from, std::vector<int>& seedFrom, int meetNode);
};

#endif // DEBFSMETHOD_H
