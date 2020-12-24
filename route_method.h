#ifndef ROUTEMETHOD_H
#define ROUTEMETHOD_H

#include <vector>

class Snake;
class ScreenData;
class Seed;

class RouteMethod
{
public:
    RouteMethod(Snake& snake, ScreenData& data, Seed& seed) : snake(snake), data(data), seed(seed) {}
    virtual ~RouteMethod() {}
    virtual bool findNext(std::vector<int>& from) = 0;

protected:
    Snake& snake;
    ScreenData& data;
    Seed& seed;
};

#endif // ROUTEMETHOD_H
