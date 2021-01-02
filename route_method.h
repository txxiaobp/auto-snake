#ifndef ROUTEMETHOD_H
#define ROUTEMETHOD_H

#include <vector>
#include <map>
#include <QMutex>
#include <QtConcurrent/QtConcurrent>

class Snake;
class ScreenData;
class Seed;

class RouteMethod
{
public:
    RouteMethod(Snake& snake, ScreenData& data, Seed& seed);
    virtual ~RouteMethod() {}
    bool getNext(int& nextNode);

protected:
    void findNext();
    virtual bool findNext(std::vector<int>& from) = 0;

protected:
    Snake& snake;
    ScreenData& screenData;
    Seed& seed;  

private:
    QMutex mutex;
    std::pair<bool,int> nextNodeData;
    bool nextNodeReady;
    bool isInit;
};

#endif // ROUTEMETHOD_H
