#include "debfs_method.h"
#include "screen_data.h"
#include "snake.h"
#include <queue>
#include <cassert>
#include <QDebug>



bool DeBFSMethod::findNext(std::vector<int>& from)
{
    std::queue<int> ss;
    std::queue<int> se;

    int snakeHead = snake.getHead();
    int seedNode = seed.getNode();

    ss.push(snakeHead);
    se.push(seedNode);

    std::vector<int> seedFrom(screenData.getTotalSize(), -1);

    from[snakeHead] = -2;
    seedFrom[seedNode] = -2;

    while (!ss.empty() || !se.empty())
    {
        if (!ss.empty())
        {
            int nd = ss.front();
            ss.pop();

            std::vector<int> nodes = screenData.aroundNodes(nd);

            for (auto node : nodes)
            {
                std::pair<int,int> nodePair = screenData.getNodePair(node);
                int r = nodePair.first;
                int c = nodePair.second;

                // found goal
                if (seedFrom[node] != -1)
                {
                    from[node] = nd;
                    generateFrom(from, seedFrom, node);
                    return true;
                }

                // not found goal but empty node
                if (screenData.getType(r, c) == NODE_AVAILABLE && from[node] == -1)
                {
                    from[node] = nd;
                    ss.push(node);
                }
            }
        }
        if (!se.empty())
        {
            int nd = se.front();
            se.pop();

            std::vector<int> nodes = screenData.aroundNodes(nd);

            for (auto node : nodes)
            {
                std::pair<int,int> nodePair = screenData.getNodePair(node);
                int r = nodePair.first;
                int c = nodePair.second;

                // found snake type node
                if (from[node] != -1)
                {
                    seedFrom[node] = nd;
                    generateFrom(from, seedFrom, node);
                    return true;
                }

                // not found snake head type node but empty node
                if (screenData.getType(r, c) == NODE_AVAILABLE && seedFrom[node] == -1)
                {
                    seedFrom[node] = nd;
                    se.push(node);
                }
            }
        }
    }

    return false;
}

void DeBFSMethod::generateFrom(std::vector<int>& from, std::vector<int>& seedFrom, int meetNode)
{
    while (seedFrom[meetNode] != -2)
    {
        from[seedFrom[meetNode]] = meetNode;
        meetNode = seedFrom[meetNode];
    }
}
