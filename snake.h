#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <queue>
#include "screen_data.h"
#include "seed.h"

class RouteMethod;
class DataRecorder;

typedef enum 
{
    UP     = 0,
    DOWN   = 1,
    LEFT   = 2,
    RIGHT  = 3,
    DIRECTION_MAX
}direction_e;

typedef enum
{
    BFS_METHOD = 0,
    DIJKSTRA_METHOD,
    METHOD_MAX
}search_method_e;

class Snake
{
public:
    Snake(Seed& seed, ScreenData& screenData, DataRecorder& dataRecorder);
    ~Snake();
    void setDirection(int node);
    int moveDirection(int direction);
    bool move();
    void push(int row, int col, Node_t node_type, bool initial = false);
	void pop();

    int getHead() { return getNum(headRow, headCol); }
    int getHeadRow() { return headRow; }
    int getHeadCol() { return headCol; }

public:
    std::vector<int> aroundNodes(int node);
    int getNum(int nodeX, int nodeY);
    int getRow(int node);
    int getCol(int node);
    int getSize();
    int getEatenCount();
    int getWalkedCount();
    void setMethod(search_method_e m);
    search_method_e getMethod();
    void reset(); // restart the game
    void replay(bool replay);

private:
    void setData(int node);
    void toNextNode(int nextNode);

private:
    std::queue<int> snake; //蛇尾是队列头，蛇头是队列尾
    ScreenData& screenData;
    DataRecorder& dataRecorder;
    int walkedCount;
    int headRow;
    int headCol;
    Seed& seed;
    direction_e last_direction;
    RouteMethod* method[METHOD_MAX];
    search_method_e cur_method;
    bool isReplay;
};

#endif
