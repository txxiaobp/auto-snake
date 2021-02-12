#ifndef MOVABLEOBSTACLEPAINTER_H
#define MOVABLEOBSTACLEPAINTER_H

#include "screen_painter.h"

class MovableObstaclePainter : public ScreenPainter
{
public:
    MovableObstaclePainter(int radius);
    void draw(QPainter& painter, int x, int y);
};

#endif // MOVABLEOBSTACLEPAINTER_H
