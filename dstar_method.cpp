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

    reverseFrom = std::vector<int>(screenData.getRow() * screenData.getCol(), -1);

int snakeHead = snake.getHead();
std::vector<int> marked(reverseFrom.size(), false);
IndexHeap<int> heap(reverseFrom.size());

    heap.push(lastSeedNode, 0);
    reverseFrom[lastSeedNode] = -2;

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

int nextDis = calDistance(node, nd);
            if (reverseFrom[node] == -1)
            {
                assert(!heap.in_heap(node));
                heap.push(node, heap.get(nd) + nextDis);
                reverseFrom[node] = nd;
            }
            else if (heap.get(nd) + nextDis < heap.get(node))
            {
               heap.change(node, heap.get(nd) + nextDis);
               reverseFrom[node] = nd;
            }
        }
    }
}

int DStarMethod::calDistance(int nextNode, int curNode)
{
    int snakeRow = snake.getHeadRow();
    int snakeCol = snake.getHeadCol();

    auto nextPair = screenData.getNodePair(nextNode);
    int nextRow = nextPair.first;
    int nextCol = nextPair.second;

    auto curPair = screenData.getNodePair(curNode);
    int curRow = curPair.first;
    int curCol = curPair.second;

    int nextDis = std::abs(snakeRow - nextRow) + std::abs(snakeCol - nextCol);
    int curDis = std::abs(snakeRow - curRow) + std::abs(snakeCol - curCol);

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

bool DStarMethod::findNext(std::vector<int>& from)
{
    replan();
int snakeHead = snake.getHead();
    if (reverseFrom[snakeHead] < 0)
    {
        return false;
    }

    from[lastSeedNode] = reverseFrom[snakeHead];
    from[reverseFrom[snakeHead]] = snakeHead;
    return true;
}


