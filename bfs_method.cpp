#include "bfs_method.h"
#include "screen_data.h"
#include "snake.h"
#include <queue>
#include <QDebug>

bool BFSMethod::findNext(std::vector<int>& from)
{
    std::queue<int> s;

    int snakeHead = snake.getHead();
    from[snakeHead] = -2;
    s.push(snakeHead);

    while (!s.empty())
    {
        int nd = s.front();
        s.pop();

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

            // not found goal but empty node
            if (data.get_type(r, c) == Node_available && from[node] == -1)
            {
                from[node] = nd;
                s.push(node);
            }
        }
    }
    return false;
}
