#ifndef SEEDPAINTER_H
#define SEEDPAINTER_H

#include "screen_painter.h"
#include <vector>


class SeedPainter : public ScreenPainter
{
public:
    SeedPainter(int radius);
    void draw(QPainter& painter, int x, int y);

private:
    std::vector<QImage> imgs;
    int curIndex;
    int posX;
    int posY;
};

#endif // SEEDPAINTER_H
