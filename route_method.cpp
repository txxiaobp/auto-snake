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
    QtConcurrent::run(this, &RouteMethod::findNext);
}


void RouteMethod::findNext()
{
    while (true)
    {
        if (isInit)
        {
            continue;
        }

        mutex.lock();
        if (nextNodeReady)
        {
            mutex.unlock();
            continue;
        }
        mutex.unlock();

        std::vector<int> from(screenData.getRow() * screenData.getCol(), -1);
        bool hasPath = findNext(from);
        if (!hasPath)
        {
            mutex.lock();
            nextNodeReady = true;
            nextNodeData = std::make_pair(hasPath, -1);
            mutex.unlock();
            continue;
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
}

bool RouteMethod::getNext(int& nextNode)
{
    if (isInit)
    {
        isInit = false;
    }

    static int i;
    while (true)
    {
        mutex.lock();
        if (nextNodeReady)
        {
            mutex.unlock();
            break;
        }
        mutex.unlock();
        qDebug() << "wait " << i++;
    }

    mutex.lock();
    nextNodeReady = false;
    bool ret = nextNodeData.first;
    nextNode = nextNodeData.second;
    mutex.unlock();

    return ret;
}
