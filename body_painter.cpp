#include "body_painter.h"
#include <QBrush>
#include <QPen>

BodyPainter::BodyPainter(int radius) : ScreenPainter(radius) {}

void BodyPainter::draw(QPainter& painter, int x, int y)
{
    QRect target(x - radius, y - radius, 2 * radius, 2 * radius);
    QRect source(0.0, 0.0, 82, 82);
    QImage image("body.jpg");

    painter.drawImage(target, image, source);
}
