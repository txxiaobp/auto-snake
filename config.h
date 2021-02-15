#ifndef CONFIG_H
#define CONFIG_H

#include "pub_include.h"

class Config
{
public:
    Config();
    void getConfig(int &row, int &col, int &radius, bool &hasMovableObstacles);
    int getRow() const { return row; }
    int getCol() const { return col; }
    int getRadius() const { return radius; }
    bool getHasObstacles() const { return hasObstacles; }
    bool getHasMovableObstacles() const { return hasMovableObstacles; }
    Algorithm_e getAlgorithm() const { return algorithm; }

    void setHasObstacles(bool hasObstacles) { this->hasObstacles = hasObstacles; }
    void setHasMovableObstacle(bool hasMovableObstacles) { this->hasMovableObstacles = hasMovableObstacles; }
    void setAlgorithm(Algorithm_e algorithm) { this->algorithm = algorithm; }
    void outputConfig();

    static void outputDefaultConfig();

private:
    int row;
    int col;
    int radius;
    bool hasObstacles;
    bool hasMovableObstacles;
    Algorithm_e algorithm;

private:
    static const char *configFile;
    static const int defaultRow;
    static const int defaultCol;
    static const int defaultRadius;
    static const bool defaultHasObstacles;
    static const bool defaultHasMovableObstacles;
    static const Algorithm_e defaultAlgorithm;
};

#endif // CONFIG_H
