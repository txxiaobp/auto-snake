#include "head_painter.h"
#include <QBrush>
#include <QPen>

HeadPainter::HeadPainter(int radius) : ScreenPainter(radius) {}

void HeadPainter::draw(QPainter& painter, int x, int y)
{
    QPen pen(Qt::red);
    QBrush brush(Qt::red);
    painter.setPen(pen);
    painter.setBrush(brush);

    painter.drawEllipse(QPoint(x, y), radius, radius);
}
