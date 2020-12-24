#ifndef SEEDPAINTER_H
#define SEEDPAINTER_H

#include "screen_painter.h"

class SeedPainter : public ScreenPainter
{
public:
    SeedPainter(int radius);
    void draw(QPainter& painter, int x, int y);
};

#endif // SEEDPAINTER_H
