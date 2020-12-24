#include "body_painter.h"
#include <QBrush>
#include <QPen>

BodyPainter::BodyPainter(int radius) : ScreenPainter(radius) {}

void BodyPainter::draw(QPainter& painter, int x, int y)
{
    QPen pen(Qt::black);
    QBrush brush(Qt::black);
    painter.setPen(pen);
    painter.setBrush(brush);

    painter.drawEllipse(QPoint(x, y), radius, radius);
}
