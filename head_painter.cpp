#include "head_painter.h"
#include <QBrush>
#include <QPen>
#include <cassert>

HeadPainter::HeadPainter(int radius, Snake& snake)
    : ScreenPainter(radius)
    , snake(snake)
{
    imgMap[DIRECTION_UP] = QImage(":/res/images/head_up.jpg");
    imgMap[DIRECTION_DOWN] = QImage(":/res/images/head_down.jpg");
    imgMap[DIRECTION_LEFT] = QImage(":/res/images/head_left.jpg");
    imgMap[DIRECTION_RIGHT] = QImage(":/res/images/head_right.jpg");
}

void HeadPainter::draw(QPainter& painter, int x, int y)
{
    QRect target(x - radius, y - radius, 2 * radius, 2 * radius);
    QRect source(0.0, 0.0, 105, 106);
    painter.drawImage(target, imgMap[snake.getDirection()], source);
}
