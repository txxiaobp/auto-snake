#ifndef DATARECORDER_H
#define DATARECORDER_H

#include <vector>
#include "snake.h"
#include "screen_data.h"

class DataRecorder
{
public:
    DataRecorder(ScreenData& screenData, bool isReplay);
    void pushSnakeData(int directionData);
    void pushGoalData(int goal);
    int popSnakeData();
    int popGoalData();
    void exportToFile(const char *fileName);
    void importFromFile(const char *fileName);
    void reset();
    bool empty();
    int getRow() { return row; };
    int getCol() { return col; };

private:
    std::vector<int> snakeHeadData;
    std::vector<int> goalData;
    int row;
    int col;
    int snakeIndex;
    int goalIndex;
    bool isReplay;
};

#endif // DATARECORDER_H
