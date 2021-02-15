#include "config.h"
#include <fstream>


const char *Config::configFile = "config.cfg";
const int Config::defaultRow = 20;
const int Config::defaultCol = 30;
const int Config::defaultRadius = 20;
const bool Config::hasMovableObstacles = true;


void Config::loadConfigFile(int &row, int &col, int &radius, bool &hasMovableObstacles)
{
    std::ifstream inf;
    inf.open(Config::configFile);
    if (!inf)
    {
        Config::outputDefaultConfig();
        inf.open(Config::configFile);
    }
    inf >> row >> col >> radius >> hasMovableObstacles;
    inf.close();
}


void Config::outputDefaultConfig()
{
    std::ofstream outFile(Config::configFile);
    outFile << Config::defaultRow << " " << Config::defaultCol << " " << Config::defaultRadius << " " << Config::hasMovableObstacles;
    outFile.close();
}
