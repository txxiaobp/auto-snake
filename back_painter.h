#ifndef BACKPAINTER_H
#define BACKPAINTER_H

#include "screen_painter.h"

class BackPainter : public ScreenPainter
{
public:
    BackPainter(int radius);
    void draw(QPainter& painter, int x, int y);
};

#endif // BACKPAINTER_H
