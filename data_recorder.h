#ifndef DATARECORDER_H
#define DATARECORDER_H

#include <vector>
#include "snake.h"
#include "screen_data.h"
#include "mode_selection.h"

class DataRecorder
{
public:
    DataRecorder(ScreenData& screenData, ModeSelection& modeSelection);
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
    ModeSelection& modeSelection;
    int row;
    int col;
    int snakeIndex;
    int goalIndex;
};

#endif // DATARECORDER_H
