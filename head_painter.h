#ifndef HEADPAINTER_H
#define HEADPAINTER_H

#include "screen_painter.h"
#include "snake.h"
#include <map>


class HeadPainter : public ScreenPainter
{
public:
    HeadPainter(int radius, Snake& snake);
    void draw(QPainter& painter, int x, int y);

private:
    Snake& snake;
    std::map<Direction_e, QImage> imgMap;
};

#endif // HEADPAINTER_H
