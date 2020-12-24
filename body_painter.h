#ifndef BODYPAINTER_H
#define BODYPAINTER_H

#include "screen_painter.h"

class BodyPainter : public ScreenPainter
{
public:
    BodyPainter(int radius);
    void draw(QPainter& painter, int x, int y);
};

#endif // BODYPAINTER_H
