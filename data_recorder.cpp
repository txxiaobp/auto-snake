#include "data_recorder.h"
#include <fstream>
#include <cassert>
#include <iostream>

DataRecorder::DataRecorder(ScreenData& screenData, ModeSelection& modeSelection)
    : modeSelection(modeSelection)
    , row(screenData.row())
    , col(screenData.col())
    , snakeIndex(0)
    , goalIndex(0)
{
    reset();
    if (modeSelection.getMode() == MODE_REPLAY)
    {
        importFromFile("data.txt");
        screenData.reset(row, col);
    }
}

void DataRecorder::reset()
{
    snakeHeadData.clear();
    goalData.clear();
    snakeIndex = 0;
    goalIndex = 0;
}

void DataRecorder::pushSnakeData(int directionData)
{
    if (modeSelection.getMode() == MODE_REPLAY)
    {
        return;
    }
    snakeHeadData.push_back(directionData);
}

void DataRecorder::pushGoalData(int goal)
{
    if (modeSelection.getMode() == MODE_REPLAY)
    {
        return;
    }
    goalData.push_back(goal);
}

void DataRecorder::exportToFile(const char *fileName)
{
    std::ofstream of(fileName);
    assert(of.is_open());
    of << row << " " << col << " " << snakeHeadData.size() << " " << goalData.size() << std::endl;
    for (int d : snakeHeadData)
    {
        of << d << " ";
    }
    of << std::endl;
    for (int g : goalData)
    {
        of << g << " ";
    }
    of << std::endl;
    of.close();
}

void DataRecorder::importFromFile(const char *fileName)
{
    snakeIndex = 0;
    goalIndex = 0;
    std::ifstream inf(fileName);
    assert(inf.is_open());

    int snakeDataSize = 0, goalDataSize = 0;
    inf >> row >> col >> snakeDataSize >> goalDataSize;

    int val = 0;
    for (int i = 0; i < snakeDataSize; i++)
    {
        inf >> val;
        snakeHeadData.push_back(val);
    }

    for (int i = 0; i < goalDataSize; i++)
    {
        inf >> val;
        goalData.push_back(val);
    }
    inf.close();
}

int DataRecorder::popSnakeData()
{
    if (modeSelection.getMode() != MODE_REPLAY)
    {
        return -1;
    }
    assert(snakeIndex >= 0 && snakeIndex < int(snakeHeadData.size()));
    return snakeHeadData[snakeIndex++];
}

int DataRecorder::popGoalData()
{
    if (modeSelection.getMode() != MODE_REPLAY)
    {
        return -1;
    }
    assert(goalIndex >= 0 && goalIndex < int(goalData.size()));
    return goalData[goalIndex++];
}

bool DataRecorder::empty()
{
    if (snakeIndex == int(snakeHeadData.size()))
    {
        assert(goalIndex == int(goalData.size()));
    }

    return snakeIndex == int(snakeHeadData.size());
}
