#include "data_recorder.h"
#include <fstream>
#include <cassert>
#include <iostream>

DataRecorder::DataRecorder(ScreenData& screenData, Obstacle& obstacle, ModeSelection& modeSelection)
    : obstacle(obstacle)
    , screenData(screenData)
    , modeSelection(modeSelection)
    , row(screenData.getRow())
    , col(screenData.getCol())
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

void DataRecorder::reset(const char *fileName)
{
    snakeHeadData.clear();
    goalData.clear();
    obstacle.clearObstacle();
    importFromFile(fileName);
}

void DataRecorder::reset()
{
    snakeIndex = 0;
    goalIndex = 0;
}

void DataRecorder::clearData()
{
    snakeHeadData.clear();
    goalData.clear();
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
    std::vector<int> obstacleNode;
    obstacle.getObstacleData(obstacleNode);
    std::ofstream of(fileName);
    assert(of.is_open());

    of << row << " " << col << " " << snakeHeadData.size() << " " << goalData.size() << " " << obstacleNode.size() << " ";

    for (int node : obstacleNode)
    {
        of << node << " ";
    }

    for (int d : snakeHeadData)
    {
        of << d << " ";
    }
    for (int g : goalData)
    {
        of << g << " ";
    }
    of.close();
}

void DataRecorder::importFromFile(const char *fileName)
{
    snakeIndex = 0;
    goalIndex = 0;
    std::ifstream inf(fileName);
    assert(inf.is_open());

    int snakeDataSize = 0, goalDataSize = 0, obstacleCount = 0;
    inf >> row >> col >> snakeDataSize >> goalDataSize >> obstacleCount;

    int val = 0;
    for (int i = 0; i < obstacleCount; i++)
    {
        inf >> val;
        screenData.setType(val, NODE_OBSTACLE);
    }

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
    assert(goalIndex >= 0 && goalIndex < int(goalData.size()));
    return goalData[goalIndex++];
}

bool DataRecorder::empty()
{
    return snakeIndex == int(snakeHeadData.size());
}
