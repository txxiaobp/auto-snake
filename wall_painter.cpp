#include "wall_painter.h"
#include <QBrush>
#include <QPen>

WallPainter::WallPainter(int radius) : ScreenPainter(radius) {}

void WallPainter::draw(QPainter& painter, int x, int y)
{
    QPen pen(Qt::gray);
    QBrush brush(Qt::gray);
    painter.setPen(pen);
    painter.setBrush(brush);

    painter.drawRect(x - radius, y - radius, 2 * radius, 2 * radius);
}
