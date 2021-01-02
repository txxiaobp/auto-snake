#include "obstacle_painter.h"
#include "wall_painter.h"
#include <QBrush>
#include <QPen>

ObstaclePainter::ObstaclePainter(int radius) : ScreenPainter(radius) {}


void ObstaclePainter::draw(QPainter& painter, int x, int y)
{
    QRect target(x - radius, y - radius, 2 * radius, 2 * radius);
    QRect source(0.0, 0.0, 32, 25);
    QImage image(":/res/images/wall.jpg");

    painter.drawImage(target, image, source);
}
