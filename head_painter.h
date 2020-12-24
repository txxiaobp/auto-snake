#ifndef HEADPAINTER_H
#define HEADPAINTER_H

#include "screen_painter.h"

class HeadPainter : public ScreenPainter
{
public:
    HeadPainter(int radius);
    void draw(QPainter& painter, int x, int y);
};

#endif // HEADPAINTER_H
