#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <queue>
#include "screen_data.h"
#include "seed.h"

class RouteMethod;

typedef enum 
{
	UP,
	DOWN,
	LEFT, 
	RIGHT,
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
	Snake(Seed& seed, ScreenData& screenData);
    ~Snake();
    void setDirection(int node);
    int move_direction(int direction);
    bool move();
    void push(int row, int col, Node_t node_type, bool initial = false);
	void pop();

    int getHead() { return getNum(headRow, headCol); }
    int getHeadRow() { return headRow; }
    int getHeadCol() { return headCol; }

public:
    std::vector<int> around_nodes(int node);
    int getNum(int nodeX, int nodeY);
    int getRow(int node);
    int getCol(int node);
    void setMethod(search_method_e m);
    search_method_e getMethod();
    void reset(); // restart the game

private:
    std::queue<int> snake; //蛇尾是队列头，蛇头是队列尾
    ScreenData& screenData;
    int headRow;
    int headCol;
    Seed& seed;
    direction_e last_direction;
    RouteMethod* method[METHOD_MAX];
    search_method_e cur_method;
};

#endif
