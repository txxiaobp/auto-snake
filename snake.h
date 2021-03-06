#ifndef SNAKE_H
#define SNAKE_H

#include <QObject>
#include <vector>
#include <queue>
#include "screen_data.h"
#include "seed.h"
#include "direction.h"
#include "mode_selection.h"
#include "algorithm_selection.h"

class RouteMethod;
class DataRecorder;


class Snake : public QObject
{
    Q_OBJECT
public:
    Snake(Seed& seed, ScreenData& screenData, DataRecorder& dataRecorder, ModeSelection& modeSelection, AlgorithmSelection& algorithmSelection);
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
    int getNum(int nodeX, int nodeY);
    int getRow(int node);
    int getCol(int node);
    int getSize();
    int getEatenCount();
    int getWalkedCount();
    void reset(); // restart the game

signals:
    void snakeLengthIncrease(int length);
    void snakeWalkedIncrease(int length);

private:
    void setData(int node);
    void toNextNode(int nextNode);

private:
    std::queue<int> snake; //蛇尾是队列头，蛇头是队列尾
    ScreenData& screenData;
    DataRecorder& dataRecorder;
    ModeSelection& modeSelection;
    AlgorithmSelection& algorithmSelection;
    int walkedCount;
    int headRow;
    int headCol;
    Seed& seed;
    Direction_e last_direction;
    RouteMethod* method[ALGORITHM_MAX];
};

#endif
