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

    int i = 0;
    while (!heap.empty())
    {
        int nd = heap.top_index();
        heap.pop();
        marked[nd] = true;

        std::vector<int> nodes = snake.around_nodes(nd);

        for (auto node : nodes)
        {
            int r = snake.getRow(node);
            int c = snake.getCol(node);

            // found goal
            if (node == seed.get_node())
            {
                from[node] = nd;
                return true;
            }

            // found empty node
            if (data.get_type(r, c) == Node_available)
            {
                if (marked[node])
                {
                    continue;
                }
                if (from[node] == -1)
                {
                    assert(!heap.in_heap(node));
                    heap.push(node, heap.get(nd) + 1);
                    from[node] = nd;
                }
                else if (heap.get(nd) + 1 < heap.get(node))
                {
                   heap.change(node, heap.get(nd) + 1);
                   from[node] = nd;
                }
            }
        }
        i++;
    }

    return false;
}









