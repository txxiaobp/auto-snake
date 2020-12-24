#include "widget.h"
#include "screen_data.h"
#include "snake.h"
#include "seed.h"
#include "qt_screen.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    int row = 30;
    int col = 30;
    int radius = 8;

    QApplication a(argc, argv);
    ScreenData screenData(row, col);
    Seed seed(screenData);
    Snake snake(seed, screenData);
    Widget screen(screenData, snake, radius);

    screen.show();
    return a.exec();
}
