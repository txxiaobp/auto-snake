#ifndef ROUTEMETHOD_H
#define ROUTEMETHOD_H

#include <vector>
#include <map>
#include <QMutex>
#include <QtConcurrent/QtConcurrent>


//#define FIND_NEXT_MULTI_THREAD
#define FIND_NEXT_SINGLE_THREAD


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

#ifdef FIND_NEXT_MULTI_THREAD
    QMutex mutex;
    bool nextNodeReady;
    bool isInit;
#endif

    std::pair<bool,int> nextNodeData;
};

#endif // ROUTEMETHOD_H
