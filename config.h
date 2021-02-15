#ifndef CONFIG_H
#define CONFIG_H


class Config
{
public:

    static void loadConfigFile(int &row, int &col, int &radius, bool &hasMovableObstacles);
    static void outputDefaultConfig();

private:
    static const char *configFile;
    static const int defaultRow;
    static const int defaultCol;
    static const int defaultRadius;
    static const bool hasMovableObstacles;
};

#endif // CONFIG_H
