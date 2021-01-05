#include "wall_painter.h"
#include <QBrush>
#include <QPen>

WallPainter::WallPainter(int radius) : ScreenPainter(radius) {}

void WallPainter::draw(QPainter& painter, int x, int y)
{
    QRect target(x - radius, y - radius, 2 * radius, 2 * radius);
    QRect source(0.0, 0.0, 90, 89);
    QImage image(":/res/images/wall.jpg");

    painter.drawImage(target, image, source);
}
