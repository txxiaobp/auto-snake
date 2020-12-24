#ifndef SCREENPAINTER_H
#define SCREENPAINTER_H

#include <QPainter>
#include <QWidget>

class ScreenPainter
{
public:
    ScreenPainter(int radius) : radius(radius) {}
    virtual void draw(QPainter& painter, int x, int y) = 0;
    virtual ~ScreenPainter() {}

protected:
    int radius;
};

#endif // SCREENPAINTER_H
