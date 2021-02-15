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

    bool setRow(int row);
    bool setCol(int col);
    bool setRadius(int radius);
    void setHasObstacles(bool hasObstacles) { this->hasObstacles = hasObstacles; }
    void setHasMovableObstacle(bool hasMovableObstacles) { this->hasMovableObstacles = hasMovableObstacles; }
    void setAlgorithm(Algorithm_e algorithm) { this->algorithm = algorithm; }
    void outputConfig();
    void resetConfig();

    static void outputDefaultConfig();
    static int getMaxRow();
    static int getMaxCol();
    static int getMaxRadius();

private:
    int row;
    int col;
    int radius;
    bool hasObstacles;
    bool hasMovableObstacles;
    Algorithm_e algorithm;

private:
    static const char *configFile;
    static const int MAX_ROW;
    static const int MAX_COL;
    static const int MAX_RADIUS;
    static const int defaultRow;
    static const int defaultCol;
    static const int defaultRadius;
    static const bool defaultHasObstacles;
    static const bool defaultHasMovableObstacles;
    static const Algorithm_e defaultAlgorithm;
};

#endif // CONFIG_H
