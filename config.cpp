#include "config.h"
#include <fstream>

const int Config::MAX_ROW = 40;
const int Config::MAX_COL = 50;
const int Config::MAX_RADIUS = 20;
const char *Config::configFile = "config.cfg";
const int Config::defaultRow = 20;
const int Config::defaultCol = 30;
const int Config::defaultRadius = 20;
const bool Config::defaultHasObstacles = true;
const bool Config::defaultHasMovableObstacles = true;
const Algorithm_e Config::defaultAlgorithm = ALGORITHM_BFS;


Config::Config()
{
    int tmpAlgo;
    std::ifstream inf;
    inf.open(Config::configFile);
    if (!inf)
    {
        Config::outputDefaultConfig();
        inf.open(Config::configFile);
    }
    inf >> row >> col >> radius >> hasObstacles >> hasMovableObstacles >> tmpAlgo;
    algorithm = static_cast<Algorithm_e>(tmpAlgo);
    inf.close();
}


void Config::outputDefaultConfig()
{
    std::ofstream outFile(Config::configFile);
    int tmpAlgo = static_cast<int>(Config::defaultAlgorithm);

    outFile << Config::defaultRow << " "
            << Config::defaultCol << " "
            << Config::defaultRadius << " "
            << Config::defaultHasObstacles << " "
            << Config::defaultHasMovableObstacles << " "
            << tmpAlgo;

    outFile.close();
}

void Config::outputConfig()
{
    std::ofstream outFile(Config::configFile);
    int tmpAlgo = static_cast<int>(algorithm);

    outFile << row << " "
            << col << " "
            << radius << " "
            << hasObstacles << " "
            << hasMovableObstacles << " "
            << tmpAlgo;

    outFile.close();
}

bool Config::setRow(int row)
{
    if (row <= 0 || row > Config::MAX_ROW)
    {
        return false;
    }
    this->row = row;
    return true;
}


bool Config::setCol(int col)
{
    if (col <= 0 || col > Config::MAX_COL)
    {
        return false;
    }
    this->col = col;
    return true;
}

bool Config::setRadius(int radius)
{
    if (radius <= 0 || radius > Config::MAX_RADIUS)
    {
        return false;
    }
    this->radius = radius;
    return true;
}

int Config::getMaxRow()
{
    return Config::MAX_ROW;
}

int Config::getMaxCol()
{
    return Config::MAX_COL;
}

int Config::getMaxRadius()
{
    return Config::MAX_RADIUS;
}

void Config::resetConfig()
{
    row = Config::defaultRow;
    col = Config::defaultCol;
    radius = Config::defaultRadius;
    algorithm = Config::defaultAlgorithm;
    hasObstacles = Config::defaultHasObstacles;
    hasMovableObstacles = Config::defaultHasMovableObstacles;
}
