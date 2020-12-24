#include "widget.h"
#include "screen_data.h"
#include "snake.h"
#include "seed.h"
#include "data_recorder.h"
#include "qt_screen.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    int row = 30;
    int col = 30;
    int radius = 8;
    bool isReplay = false;

    QApplication a(argc, argv);
    ScreenData screenData(row, col);
    DataRecorder dataRecorder(screenData, isReplay);
    Seed seed(screenData, dataRecorder, isReplay);
    Snake snake(seed, screenData, dataRecorder);
    Widget screen(screenData, snake, radius, dataRecorder, isReplay);

    screen.show();
    return a.exec();
}
