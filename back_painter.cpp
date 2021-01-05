#include "back_painter.h"
#include <QBrush>
#include <QPen>

BackPainter::BackPainter(int radius) : ScreenPainter(radius) {}

void BackPainter::draw(QPainter& painter, int x, int y)
{
    QPen pen(Qt::white);
    QBrush brush(Qt::white);
    painter.setPen(pen);
    painter.setBrush(brush);

    painter.drawRect(x - radius, y - radius, 2 * radius, 2 * radius);
}
