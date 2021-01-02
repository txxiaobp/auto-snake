#include "route_method.h"
#include "seed.h"
#include "snake.h"
#include <cassert>


RouteMethod::RouteMethod(Snake& snake, ScreenData& data, Seed& seed)
    : snake(snake)
    , screenData(data)
    , seed(seed)
#ifdef FIND_NEXT_MULTI_THREAD
    , isInit(true)
#endif

{
#ifdef FIND_NEXT_MULTI_THREAD
    QtConcurrent::run(this, &RouteMethod::findNext);
#endif
}


void RouteMethod::findNext()
{
#ifdef FIND_NEXT_MULTI_THREAD
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
#endif

        std::vector<int> from(screenData.getRow() * screenData.getCol(), -1);
        bool hasPath = findNext(from);
        if (!hasPath)
        {
#ifdef FIND_NEXT_SINGLE_THREAD
            nextNodeData = std::make_pair(hasPath, -1);
            return;
#endif

#ifdef FIND_NEXT_MULTI_THREAD
            mutex.lock();
            nextNodeReady = true;
            nextNodeData = std::make_pair(hasPath, -1);
            mutex.unlock();
            continue;
#endif
        }

        int node = seed.getNode();
        int snakeHead = snake.getHead();

        while (from[node] != snakeHead)
        {
            node = from[node];
        }

#ifdef FIND_NEXT_SINGLE_THREAD
        nextNodeData = std::make_pair(hasPath, node);
#endif

#ifdef FIND_NEXT_MULTI_THREAD
        mutex.lock();
        nextNodeReady = true;
        nextNodeData = std::make_pair(hasPath, node);
        mutex.unlock();
    }
#endif
}

bool RouteMethod::getNext(int& nextNode)
{
#ifdef FIND_NEXT_MULTI_THREAD
    if (isInit)
    {
        isInit = false;
    }


    while (true)
    {
        mutex.lock();
        if (nextNodeReady)
        {
            mutex.unlock();
            break;
        }
        mutex.unlock();
    }

    mutex.lock();
    nextNodeReady = false;
    bool ret = nextNodeData.first;
    nextNode = nextNodeData.second;
    mutex.unlock();
#endif

#ifdef FIND_NEXT_SINGLE_THREAD
    findNext();
    bool ret = nextNodeData.first;
    nextNode = nextNodeData.second;
#endif
    return ret;
}
