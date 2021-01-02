#ifndef OBSTACLEPAINTER_H
#define OBSTACLEPAINTER_H

#include "screen_painter.h"

class ObstaclePainter : public ScreenPainter
{
public:
    ObstaclePainter(int radius);
    void draw(QPainter& painter, int x, int y);
};

#endif // OBSTACLEPAINTER_H
