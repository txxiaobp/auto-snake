#include "back_painter.h"
#include <QBrush>
#include <QPen>

BackPainter::BackPainter(int radius) : ScreenPainter(radius) {}

void BackPainter::draw(QPainter& painter, int x, int y)
{
    QRect target(x - radius, y - radius, 2 * radius, 2 * radius);
    QRect source(0.0, 0.0, 31, 31);
    QImage image(":/res/images/back.jpg");

    painter.drawImage(target, image, source);
}
