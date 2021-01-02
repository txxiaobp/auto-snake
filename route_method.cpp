#include "route_method.h"
#include "seed.h"
#include "snake.h"
#include <cassert>


RouteMethod::RouteMethod(Snake& snake, ScreenData& data, Seed& seed)
    : snake(snake)
    , screenData(data)
    , seed(seed)
    , isInit(true)
{
}


void RouteMethod::findNext()
{
    std::vector<int> from(screenData.getRow() * screenData.getCol(), -1);
    bool hasPath = findNext(from);
    if (!hasPath)
    {
        mutex.lock();
        nextNodeData = std::make_pair(hasPath, -1);
        mutex.unlock();
        return;
    }

    int node = seed.getNode();
    int snakeHead = snake.getHead();

    while (from[node] != snakeHead)
    {
        node = from[node];
    }
    mutex.lock();
    nextNodeReady = true;
    nextNodeData = std::make_pair(hasPath, node);
    mutex.unlock();
}

bool RouteMethod::getNext(int& nextNode)
{
    if (isInit)
    {
        isInit = false;
        findNext();
        nextNodeReady = true;
    }
    else
    {
        while (true)
        {
            qDebug() << "wait";
            mutex.lock();
            if (nextNodeReady)
            {
                mutex.unlock();
                break;
            }
            mutex.unlock();
        }
    }

    assert (nextNodeReady);
    nextNodeReady = false;

    bool ret = nextNodeData.first;
    nextNode = nextNodeData.second;

    QtConcurrent::run(this, &RouteMethod::findNext);

    return ret;
}
