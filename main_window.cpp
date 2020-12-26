#include "main_window.h"
#include <QTimer>
#include <QDebug>
#include <QWheelEvent>
#include <QString>

#include "body_painter.h"
#include "head_painter.h"
#include "wall_painter.h"
#include "seed_painter.h"
#include "data_recorder.h"
#include "back_painter.h"

enum
{
    DEFAULT_AUTO_SNAKE_SPEED = 100,
    DEFAULT_MANUAL_SNAKE_SPEED = 200
};

enum
{
    SCREEN_UNDEFINED = 0,
    SCREEN_AVAILABLE,
    SCREEN_SNAKE_BODY,
    SCREEN_SNAKE_HEAD,
    SCREEN_WALL,
    SCREEN_SEED
};

const int BAR_HEIGHT = 20;


MainWindow::MainWindow(ScreenData& data, Snake& snake, int radius, DataRecorder& dataRecorder, ModeSelection& modeSelection, QWidget *parent)
    : QMainWindow(parent)
    , mBar(menuBar())
    , gameMenu(mBar->addMenu("Game"))
    , algoMenu(mBar->addMenu("Algorithm"))
    , startAction(gameMenu->addAction("Start (S)"))
    , restartAction(gameMenu->addAction("Retart (R)"))
    , pauseAction(gameMenu->addAction("Pause|Continue (P)"))
    , bfsAction(algoMenu->addAction("BFS"))
    , dijkAction(algoMenu->addAction("Dijkstra"))
    , sBar(statusBar())
    , snakeLengthLabel(new QLabel("Snake Length: 2"))
    , snakeWalkedLabel(new QLabel("Walked Counts: 0"))

    , data(data)
    , dataRecorder(dataRecorder)
    , snake(snake)
    , radius(radius)
    , isPause(false)
    , modeSelection(modeSelection)
{
    int height = data.row() * 2 * radius;
    int width = data.col() * 2 * radius;
    resize(width, height + 2 * BAR_HEIGHT);

    setMenuBar(mBar);
    mBar->addMenu(gameMenu);
    mBar->addMenu(algoMenu);

    gameMenu->addAction(startAction);
    gameMenu->addAction(restartAction);
    gameMenu->addAction(pauseAction);

    algoMenu->addAction(bfsAction);
    algoMenu->addAction(dijkAction);

    setStatusBar(sBar);
    sBar->addWidget(snakeLengthLabel);
    sBar->addWidget(snakeWalkedLabel);

    void (Snake:: *lengthIncrease)(int) = &Snake::snakeLengthIncrease;
    void (MainWindow:: *snakeLength)(int) = &MainWindow::setSnakeLength;
    connect(&snake, lengthIncrease, this, snakeLength);

    void (Snake:: *walkedIncrease)(int) = &Snake::snakeWalkedIncrease;
    void (MainWindow:: *walkedCount)(int) = &MainWindow::setSnakeWalkedCount;
    connect(&snake, walkedIncrease, this, walkedCount);

    painter = new QPainter(this);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    setPainterMap(radius);

    switch (modeSelection.getMode())
    {
    case MODE_AUTO:
        timeInterval = DEFAULT_AUTO_SNAKE_SPEED;
        break;
    case MODE_MANUAL:
        timeInterval = DEFAULT_MANUAL_SNAKE_SPEED;
        break;
    case MODE_REPLAY:
        timeInterval = DEFAULT_MANUAL_SNAKE_SPEED;
        break;
    default:
        break;
    }

    timerId = startTimer(timeInterval);
}


//MainWindow::~MainWindow()
//{
//    delete painter;
//    delete painterMap[SCREEN_SNAKE_BODY];
//    delete painterMap[SCREEN_SNAKE_HEAD];
//    delete painterMap[SCREEN_WALL];
//    delete painterMap[SCREEN_SEED];
//}

void MainWindow::changeAlgorithm(Search_method_e method)
{
    if (modeSelection.getMode() == MODE_REPLAY)
    {
        qDebug() << "Replay the game, cannot change the algorithm";
        return;
    }
    if (method == METHOD_MAX)
    {
        int method = int(snake.getMethod());
        int nextMethod = (method + 1) % METHOD_MAX;
        snake.setMethod(Search_method_e(nextMethod));
    }
    else
    {
        snake.setMethod(method);
    }
}

void MainWindow::setSnakeLength(int length)
{
    std::string str = "Snake Length: ";
    str += std::to_string(length);
    snakeLengthLabel->setText(str.data());
    snakeLengthLabel->setBaseSize(50, BAR_HEIGHT);
}

void MainWindow::setSnakeWalkedCount(int count)
{
    std::string str = "Walked Counts: ";
    str += std::to_string(count);
    snakeWalkedLabel->setText(str.data());
    snakeWalkedLabel->setBaseSize(50, BAR_HEIGHT);
}

void MainWindow::restartGame()
{
    qDebug() << "Restart the game";
    isPause = false;
    dataRecorder.reset();
    snake.reset();
}

void MainWindow::pauseGame()
{
    qDebug() << "Pause the game";
    isPause = true;
}

void MainWindow::continueGame()
{
    qDebug() << "Continue the game";
    isPause = false;
}

bool MainWindow::isGamePause()
{
    return isPause;
}

void MainWindow::changeManualMode()
{
    timeInterval = DEFAULT_MANUAL_SNAKE_SPEED;
    modeSelection.setMode(MODE_MANUAL);
    qDebug() << "Change mode to manual";
}

void MainWindow::changeAutoMode()
{
    timeInterval = DEFAULT_AUTO_SNAKE_SPEED;
    modeSelection.setMode(MODE_AUTO);
    qDebug() << "Change mode to auto";
}

void MainWindow::setPainterMap(int radius)
{
    painterMap[SCREEN_AVAILABLE] = new BackPainter(radius);
    painterMap[SCREEN_SNAKE_BODY] = new BodyPainter(radius);
    painterMap[SCREEN_SNAKE_HEAD] = new HeadPainter(radius, snake);
    painterMap[SCREEN_WALL] = new WallPainter(radius);
    painterMap[SCREEN_SEED] = new SeedPainter(radius);
}

void MainWindow::timerEvent(QTimerEvent*)
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
        if (modeSelection.getMode() != MODE_REPLAY)
        {
            dataRecorder.exportToFile("data.txt");
        }
        qDebug() << "Game Over!";
        exit(0);
    }
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter qp(this);

    int row = data.row();
    int col = data.col();

    for (int r = 0; r < row; r++)
    {
        for (int c = 0; c < col; c++)
        {
            ScreenPainter* p = nullptr;
            p = painterMap[data.getType(r, c)];

            if (p)
            {
                p->draw(qp, radius + 2 * radius * c, BAR_HEIGHT + radius + 2 * radius * r);
            }
        }
    }
}

void MainWindow::wheelEvent(QWheelEvent *event)
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

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Up)
    {
        if (modeSelection.getMode() != MODE_MANUAL)
        {
            return;
        }
        snake.setDirection(DIRECTION_UP);
        return;
    }
    if(ev->key() == Qt::Key_Down)
    {
        if (modeSelection.getMode() != MODE_MANUAL)
        {
            return;
        }
        snake.setDirection(DIRECTION_DOWN);
        return;
    }
    if(ev->key() == Qt::Key_Left)
    {
        if (modeSelection.getMode() != MODE_MANUAL)
        {
            return;
        }
        snake.setDirection(DIRECTION_LEFT);
        return;
    }
    if(ev->key() == Qt::Key_Right)
    {
        if (modeSelection.getMode() != MODE_MANUAL)
        {
            return;
        }
        snake.setDirection(DIRECTION_RIGHT);
        return;
    }

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
    if(ev->key() == Qt::Key_M)
    {
        changeManualMode();
        return;
    }
    if(ev->key() == Qt::Key_A)
    {
        changeAutoMode();
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
