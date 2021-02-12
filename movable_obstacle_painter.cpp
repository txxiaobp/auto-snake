#include "movable_obstacle_painter.h"
#include <QBrush>
#include <QPen>

MovableObstaclePainter::MovableObstaclePainter(int radius)
    : ScreenPainter(radius)
{

}


void MovableObstaclePainter::draw(QPainter& painter, int x, int y)
{
    QRect target(x - radius, y - radius, 2 * radius, 2 * radius);
    QRect source(0.0, 0.0, 90, 89);
    QImage image(":/res/images/movable_obstacle.jpg");

    painter.drawImage(target, image, source);
}
