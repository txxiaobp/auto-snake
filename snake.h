#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <queue>
#include "screen_data.h"
#include "seed.h"
#include "mode_selection.h"

class RouteMethod;
class DataRecorder;

typedef enum 
{
    DIRECTION_UP     = 0,
    DIRECTION_DOWN   = 1,
    DIRECTION_LEFT   = 2,
    DIRECTION_RIGHT  = 3,
    DIRECTION_MAX
}Direction_e;

typedef enum
{
    BFS_METHOD = 0,
    DIJKSTRA_METHOD,
    METHOD_MAX
}Search_method_e;

class Snake
{
public:
    Snake(Seed& seed, ScreenData& screenData, DataRecorder& dataRecorder, ModeSelection& modeSelection);
    ~Snake();
    void setDirection(int node);
    void setDirection(Direction_e direction);
    Direction_e getDirection();
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
    void setMethod(Search_method_e m);
    Search_method_e getMethod();
    void reset(); // restart the game

private:
    void setData(int node);
    void toNextNode(int nextNode);

private:
    std::queue<int> snake; //��β�Ƕ���ͷ����ͷ�Ƕ���β
    ScreenData& screenData;
    DataRecorder& dataRecorder;
    ModeSelection& modeSelection;
    int walkedCount;
    int headRow;
    int headCol;
    Seed& seed;
    Direction_e last_direction;
    RouteMethod* method[METHOD_MAX];
    Search_method_e cur_method;
};

#endif
