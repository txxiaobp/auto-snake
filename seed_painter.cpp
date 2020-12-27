#include "seed_painter.h"
#include <QBrush>
#include <QPen>
#include <cstdlib>
#include <ctime>
#include <iostream>

SeedPainter::SeedPainter(int radius)
    : ScreenPainter(radius)
    , curIndex(0)
    , posX(-1)
    , posY(-1)
{
    imgs.push_back(QImage(":/res/images/seed1.jpg"));
    imgs.push_back(QImage(":/res/images/seed2.jpg"));
    imgs.push_back(QImage(":/res/images/seed3.jpg"));
    imgs.push_back(QImage(":/res/images/seed4.jpg"));
    imgs.push_back(QImage(":/res/images/seed5.jpg"));
    imgs.push_back(QImage(":/res/images/seed6.jpg"));
    imgs.push_back(QImage(":/res/images/seed7.jpg"));
    imgs.push_back(QImage(":/res/images/seed8.jpg"));
    imgs.push_back(QImage(":/res/images/seed9.jpg"));
    imgs.push_back(QImage(":/res/images/seed10.jpg"));
    imgs.push_back(QImage(":/res/images/seed11.jpg"));
    imgs.push_back(QImage(":/res/images/seed12.jpg"));
    imgs.push_back(QImage(":/res/images/seed13.jpg"));
}

void SeedPainter::draw(QPainter& painter, int x, int y)
{
    QRect target(x - radius, y - radius, 2 * radius, 2 * radius);
    QRect source(0.0, 0.0, 49, 49);

    if (posX != x && posY != y)
    {
        srand(time(NULL));
        curIndex = rand() % imgs.size();
        posX = x;
        posY = y;
    }

    painter.drawImage(target, imgs[curIndex], source);


}
