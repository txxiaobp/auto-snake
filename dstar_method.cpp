#include "dstar_method.h"
#include "index_heap.h"
#include "snake.h"
#include <QDebug>

DStarMethod::DStarMethod(Snake& snake, ScreenData& data, Seed& seed)
    : RouteMethod(snake, data, seed)
    , lastSeedNode(-1)
{
    replan();
}

void DStarMethod::replan()
{
    if (lastSeedNode == seed.getNode())
    {
        return;
    }
    lastSeedNode = seed.getNode();
int snakeHead = snake.getHead();
    pastNodes = std::vector<bool>(screenData.getRow() * screenData.getCol(), false);
    pastNodes[snakeHead] = true;

    std::vector<int> reverseFrom = std::vector<int>(screenData.getRow() * screenData.getCol(), -1);
    disToSeed = std::vector<int>(screenData.getRow() * screenData.getCol(), -1);

std::vector<int> marked(reverseFrom.size(), false);
IndexHeap<int> heap(reverseFrom.size());

    heap.push(lastSeedNode, 0);
    reverseFrom[lastSeedNode] = -2;
    disToSeed[lastSeedNode] = 0;

    while (!heap.empty())
    {
int nd = heap.top_index();
        heap.pop();
        marked[nd] = true;

std::vector<int> nodes = screenData.aroundNodes(nd);

        for (auto node : nodes)
        {
auto pair = screenData.getNodePair(node);
int r = pair.first;
int c = pair.second;

            // found goal
            if (node == snakeHead)
            {
                reverseFrom[node] = nd;
                int d = heap.get(nd) + 1;
                disToSeed[node] = d;
                return;
            }

            if (screenData.getType(r, c) != NODE_AVAILABLE)
            {
                continue;
            }

            // found empty node
            if (marked[node])
            {
                continue;
            }

            if (reverseFrom[node] == -1)
            {
                assert(!heap.in_heap(node));
                heap.push(node, heap.get(nd) + 1);
                disToSeed[node] = heap.get(nd) + 1;
                reverseFrom[node] = nd;
            }
            else if (heap.get(nd) + 1 < heap.get(node))
            {
               heap.change(node, heap.get(nd) + 1);
               disToSeed[node] = heap.get(nd) + 1;
               reverseFrom[node] = nd;
            }
        }
    }
}

bool DStarMethod::findNext(std::vector<int>& from)
{
    replan();
int snakeHead = snake.getHead();
    if (disToSeed[snakeHead] < 0)
    {
        int d = disToSeed[snakeHead];
        return false;
    }

    IndexHeap<int> heap(disToSeed.size());
    std::vector<int> nodes = screenData.aroundNodes(snakeHead);
    for (int node : nodes)
    {
        if (!pastNodes[node] && disToSeed[node] != -1 && (screenData.getType(node) == NODE_AVAILABLE || screenData.getType(node) == NODE_SEED))
        {
            heap.push(node, disToSeed[node]);
        }
    }
    if (heap.empty())
    {
const int RETRY_TIMES = 3;
        for (int i = 0; i < RETRY_TIMES && heap.empty(); i++)
        {
            replan();
            heap = IndexHeap<int>(disToSeed.size());
            for (int node : nodes)
            {
                if (!pastNodes[node] && disToSeed[node] != -1 && (screenData.getType(node) == NODE_AVAILABLE || screenData.getType(node) == NODE_SEED))
                {
                    heap.push(node, disToSeed[node]);
                }
            }
            qDebug() << i;
        }

        if (heap.empty())
        {
            return false;
        }
    }

    int minNode = heap.top_index();
    assert(minNode >= 0 && minNode < int(disToSeed.size()));

    from[lastSeedNode] = minNode;
    from[minNode] = snakeHead;
    pastNodes[minNode] = true;
    return true;
}


