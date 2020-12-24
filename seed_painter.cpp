#include "seed_painter.h"
#include <QBrush>
#include <QPen>

SeedPainter::SeedPainter(int radius) : ScreenPainter(radius) {}

void SeedPainter::draw(QPainter& painter, int x, int y)
{
    QPen pen(Qt::red);
    QBrush brush(Qt::red);
    painter.setPen(pen);
    painter.setBrush(brush);

    painter.drawRect(x - radius, y - radius, 2 * radius, 2 * radius);
}
