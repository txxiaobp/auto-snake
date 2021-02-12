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
#include <QApplication>

int main(int argc, char *argv[])
{
    int row = 25;
    int col = 25;
    int radius = 16;

    QApplication a(argc, argv);
    ModeSelection modeSelection;
    AlgorithmSelection algorithmSelection;
    SpeedSelection speedSelection(modeSelection);
    StatusSelection statusSelection;
    ScreenData screenData(row, col);
    Obstacle obstacle(screenData);
    DataRecorder dataRecorder(screenData, obstacle, modeSelection);
    Seed seed(screenData, dataRecorder, modeSelection);
    Snake snake(seed, screenData, dataRecorder, modeSelection, algorithmSelection);

    MainWindow mainWindow(screenData, snake, seed, radius, dataRecorder, obstacle, modeSelection, algorithmSelection, speedSelection, statusSelection);

    mainWindow.show();
    return a.exec();
}
