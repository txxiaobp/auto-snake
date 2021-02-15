#include "config.h"
#include <fstream>


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
