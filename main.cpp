#include "screen_data.h"
#include "snake.h"
#include "seed.h"
#include "data_recorder.h"
#include "mode_selection.h"
#include "algorithm_selection.h"
#include "speed_selection.h"
#include "game_control.h"
#include "main_window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    int row = 18;
    int col = 18;
    int radius = 16;

    QApplication a(argc, argv);
    ModeSelection modeSelection;
    AlgorithmSelection algorithmSelection;
    SpeedSelection speedSelection(modeSelection);
    StatusSelection statusSelection;
    ScreenData screenData(row, col);
    DataRecorder dataRecorder(screenData, modeSelection);
    Seed seed(screenData, dataRecorder, modeSelection);
    Snake snake(seed, screenData, dataRecorder, modeSelection, algorithmSelection);

    MainWindow mainWindow(screenData, snake, radius, dataRecorder, modeSelection, algorithmSelection, speedSelection, statusSelection);

    mainWindow.show();
    return a.exec();
}
