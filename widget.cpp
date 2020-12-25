#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QDebug>
#include <QWheelEvent>

#include "body_painter.h"
#include "head_painter.h"
#include "wall_painter.h"
#include "seed_painter.h"
#include "data_recorder.h"

enum
{
    default_snake_speed = 50
};

enum
{
    Screen_undefined = 0,
    Screen_available,
    Screen_snake_body,
    Screen_snake_head,
    Screen_wall,
    Screen_seed
};

Widget::Widget(ScreenData& data, Snake& snake, int radius, DataRecorder& dataRecorder, ModeSelection& modeSelection, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , data(data)
    , dataRecorder(dataRecorder)
    , snake(snake)
    , radius(radius)
    , timeInterval(default_snake_speed)
    , isPause(false)
    , modeSelection(modeSelection)
{
    int height = data.row() * 2 * radius;
    int width = data.col() * 2 * radius;
    resize(width, height);
    painter = new QPainter(this);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    setPainterMap(radius);

    ui->setupUi(this);   
    timerId = startTimer(timeInterval);
}

Widget::~Widget()
{
    delete ui;
    delete painter;
    delete painterMap[Screen_snake_body];
    delete painterMap[Screen_snake_head];
    delete painterMap[Screen_wall];
    delete painterMap[Screen_seed];
}

void Widget::changeAlgorithm(search_method_e method)
{
    if (modeSelection.getMode() == REPLAY_MODE)
    {
        qDebug() << "Replay the game, cannot change the algorithm";
        return;
    }
    if (method == METHOD_MAX)
    {
        int method = int(snake.getMethod());
        int nextMethod = (method + 1) % METHOD_MAX;
        snake.setMethod(search_method_e(nextMethod));
    }
    else
    {
        snake.setMethod(method);
    }
}

void Widget::restartGame()
{
    qDebug() << "Restart the game";
    isPause = false;
    dataRecorder.reset();
    snake.reset();
}

void Widget::pauseGame()
{
    qDebug() << "Pause the game";
    isPause = true;
}

void Widget::continueGame()
{
    qDebug() << "Continue the game";
    isPause = false;
}

bool Widget::isGamePause()
{
    return isPause;
}

void Widget::setPainterMap(int radius)
{
    painterMap[Screen_available] = nullptr;
    painterMap[Screen_snake_body] = new BodyPainter(radius);
    painterMap[Screen_snake_head] = new HeadPainter(radius);
    painterMap[Screen_wall] = new WallPainter(radius);
    painterMap[Screen_seed] = new SeedPainter(radius);
}

void Widget::timerEvent(QTimerEvent*)
{
    if (isPause)
    {
        return;
    }

    if (snake.move())
    {
        update();
    }
    else
    {
        //exit
        if (modeSelection.getMode() == REPLAY_MODE)
        {
            dataRecorder.exportToFile("data.txt");
        }
        qDebug() << "Game Over!";
        exit(0);
    }
}

void Widget::paintEvent(QPaintEvent*)
{
    QPainter qp(this);

    int row = data.row();
    int col = data.col();

    for (int r = 0; r < row; r++)
    {
        for (int c = 0; c < col; c++)
        {
            ScreenPainter* p = nullptr;
            switch(data.getType(r, c))
            {
            case Screen_snake_body:
                p = painterMap[Screen_snake_body];
                break;

            case Screen_snake_head:
                p = painterMap[Screen_snake_head];
                break;

            case Screen_wall:
                p = painterMap[Screen_wall];
                break;

            case Screen_seed:
                p = painterMap[Screen_seed];
                break;

            default:
                break;
            }

            if (p)
            {
                p->draw(qp, radius + 2 * radius * r, radius + 2 * radius * c);
            }
        }
    }
}

void Widget::wheelEvent(QWheelEvent *event)
{
    killTimer(timerId);
    if(event->delta() < 0)
    {
        if (timeInterval < 200)
        {
            timeInterval = std::min(timeInterval + 10, 200);
            qDebug() << "Speed Down";
        }
    }
    else
    {
        if (timeInterval > 10)
        {
           timeInterval = std::max(timeInterval - 10, 10);
           qDebug() << "Speed Up";
        }
    }
    timerId = startTimer(timeInterval);
}

void Widget::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_C)
    {
        changeAlgorithm();
        return;
    }
    if(ev->key() == Qt::Key_R)
    {
        restartGame();
        return;
    }
    if(ev->key() == Qt::Key_P)
    {
        if (isGamePause())
        {
            continueGame();
        }
        else
        {
            pauseGame();
        }

        return;
    }
}

