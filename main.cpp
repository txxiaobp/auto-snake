#include "screen_data.h"
#include "snake.h"
#include "seed.h"
#include "data_recorder.h"
#include "mode_selection.h"
#include "algorithm_selection.h"
#include "speed_selection.h"
#include "obstacle.h"
#include "game_control.h"
#include "main_window.h"
#include "config.h"
#include <QApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    Config config;

    int row = config.getRow();
    int col = config.getCol();
    int radius = config.getRadius();

    QApplication a(argc, argv);
    ModeSelection modeSelection;
    AlgorithmSelection algorithmSelection(config);
    SpeedSelection speedSelection(modeSelection);
    StatusSelection statusSelection;
    ScreenData screenData(row, col);
    Obstacle obstacle(screenData, config);
    DataRecorder dataRecorder(screenData, obstacle, modeSelection);
    Seed seed(screenData, dataRecorder, modeSelection);
    Snake snake(seed, screenData, dataRecorder, modeSelection, algorithmSelection);

    MainWindow mainWindow(screenData, snake, seed, radius, dataRecorder, obstacle, modeSelection, algorithmSelection, speedSelection, statusSelection, config);

    mainWindow.show();
    return a.exec();
}
