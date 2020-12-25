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
    int row = 18;
    int col = 18;
    int radius = 18;
    Mode_e mode = MODE_AUTO;
    //Mode_e mode = MODE_MANUAL;
    //Mode_e mode = MODE_REPLAY;

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
