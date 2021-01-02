#include "dijkstra_method.h"
#include "screen_data.h"
#include "snake.h"
#include "index_heap.h"
#include <QDebug>
#include <cassert>

bool DijkstraMethod::findNext(std::vector<int>& from)
{
    int snakeHead = snake.getHead();
    std::vector<int> marked(from.size(), false);
    IndexHeap<int> heap(from.size());
    heap.push(snakeHead, 0);
    from[snakeHead] = -2;

    while (!heap.empty())
    {
        int nd = heap.top_index();
        heap.pop();
        marked[nd] = true;

        std::vector<int> nodes = snake.aroundNodes(nd);

        for (auto node : nodes)
        {
            int r = snake.getRow(node);
            int c = snake.getCol(node);

            // found goal
            if (node == seed.getNode())
            {
                from[node] = nd;
                return true;
            }

            if (screenData.getType(r, c) != Node_available)
            {
                continue;
            }

            // found empty node
            if (marked[node])
            {
                continue;
            }

            int nextDis = calDistance(node, nd);
            if (from[node] == -1)
            {
                assert(!heap.in_heap(node));
                heap.push(node, heap.get(nd) + nextDis);
                from[node] = nd;
            }
            else if (heap.get(nd) + nextDis < heap.get(node))
            {
               heap.change(node, heap.get(nd) + nextDis);
               from[node] = nd;
            }
        }
    }

    return false;
}

int DijkstraMethod::calDistance(int nextNode, int curNode)
{
    int seedNode = seed.getNode();
    int seedRow = snake.getRow(seedNode);
    int seedCol = snake.getCol(seedNode);

    int nextRow = snake.getRow(nextNode);
    int nextCol = snake.getCol(nextNode);

    int curRow = snake.getRow(curNode);
    int curCol = snake.getCol(curNode);

    int nextDis = std::abs(seedRow - nextRow) + std::abs(seedCol - nextCol);
    int curDis = std::abs(seedRow - curRow) + std::abs(seedCol - curCol);

    // 若下一节点到的目标点的曼哈顿距离更近，则当前节点到下一阶段的距离更小
    if (nextDis < curDis)
    {
        return 10;
    }
    else
    {
        assert(nextDis > curDis);
        return 20;
    }
}









