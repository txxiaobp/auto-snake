#include "widget.h"
#include "screen_data.h"
#include "snake.h"
#include "seed.h"
#include "data_recorder.h"
#include "mode_selection.h"
#include "qt_screen.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    int row = 30;
    int col = 30;
    int radius = 8;
    //Mode_e mode = AUTO_MODE;
    Mode_e mode = REPLAY_MODE;

    QApplication a(argc, argv);
    ModeSelection modeSelection(mode);
    ScreenData screenData(row, col);
    DataRecorder dataRecorder(screenData, modeSelection);
    Seed seed(screenData, dataRecorder, modeSelection);
    Snake snake(seed, screenData, dataRecorder, modeSelection);
    Widget screen(screenData, snake, radius, dataRecorder, modeSelection);

    screen.show();
    return a.exec();
}
