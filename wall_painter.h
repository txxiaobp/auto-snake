#ifndef CIRCLEPAINTER_H
#define CIRCLEPAINTER_H

#include "screen_painter.h"

class WallPainter : public ScreenPainter
{
public:
    WallPainter(int radius);
    void draw(QPainter& painter, int x, int y);
};

#endif // CIRCLEPAINTER_H
