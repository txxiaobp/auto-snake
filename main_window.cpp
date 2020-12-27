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
    SCREEN_UNDEFINED = 0,
    SCREEN_AVAILABLE,
    SCREEN_SNAKE_BODY,
    SCREEN_SNAKE_HEAD,
    SCREEN_WALL,
    SCREEN_SEED
};

const int BAR_HEIGHT = 20;


MainWindow::MainWindow(ScreenData& data,
                       Snake& snake,
                       int radius,
                       DataRecorder& dataRecorder,
                       ModeSelection& modeSelection,
                       AlgorithmSelection& algorithmSelection,
                       SpeedSelection& speedSelection,
                       GameControl& gameControl,
                       QWidget *parent)
    : QMainWindow(parent)
    , data(data)
    , dataRecorder(dataRecorder)
    , snake(snake)
    , radius(radius)
    , modeSelection(modeSelection)
    , algorithmSelection(algorithmSelection)
    , speedSelection(speedSelection)
    , gameControl(gameControl)

    , mBar(menuBar())
    , gameMenu(mBar->addMenu("游戏"))
    , algoMenu(mBar->addMenu("寻路算法"))
    , startAction(gameMenu->addAction("开始 (S)"))
    , restartAction(gameMenu->addAction("重新开始 (R)"))
    , pauseAction(gameMenu->addAction("暂停|继续 (P)"))
    , bfsAction(algoMenu->addAction("广度优先搜索"))
    , dijkAction(algoMenu->addAction("Dijkstra"))
    , sBar(statusBar())
    , snakeLengthLabel(new QLabel("蛇身长度: 2"))
    , snakeWalkedLabel(new QLabel("走过的距离: 0"))
    , gameModeLabel(new QLabel("模式: " + modeSelection.getModeString()))
    , algorithmLabel(new QLabel("寻路算法: " + algorithmSelection.getAlgoString()))
    , speedLabel(new QLabel("速度: " + speedSelection.getSpeedString()))
{
    int height = data.row() * 2 * radius;
    int width = data.col() * 2 * radius;
    resize(width, height + 2 * BAR_HEIGHT);

    this->setWindowTitle("贪吃蛇");
    this->setWindowIcon(QIcon("title.jpg"));

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
    sBar->addWidget(gameModeLabel);
    sBar->addWidget(algorithmLabel);
    sBar->addWidget(speedLabel);

    snakeLengthLabel->setFixedSize(80, BAR_HEIGHT);
    snakeWalkedLabel->setFixedSize(100, BAR_HEIGHT);
    gameModeLabel->setFixedSize(80, BAR_HEIGHT);
    algorithmLabel->setFixedSize(150, BAR_HEIGHT);
    speedLabel->setFixedSize(80, BAR_HEIGHT);

    connectSignals();

    painter = new QPainter(this);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    setPainterMap(radius);

    timerId = startTimer(speedSelection.getSpeed());
}


MainWindow::~MainWindow()
{
    delete painter;
    delete painterMap[SCREEN_SNAKE_BODY];
    delete painterMap[SCREEN_SNAKE_HEAD];
    delete painterMap[SCREEN_WALL];
    delete painterMap[SCREEN_SEED];
}

void MainWindow::connectSignals()
{
    void (Snake:: *lengthIncrease)(int) = &Snake::snakeLengthIncrease;
    void (MainWindow:: *snakeLength)(int) = &MainWindow::setSnakeLength;
    connect(&snake, lengthIncrease, this, snakeLength);

    void (Snake:: *walkedIncrease)(int) = &Snake::snakeWalkedIncrease;
    void (MainWindow:: *walkedCount)(int) = &MainWindow::setSnakeWalkedCount;
    connect(&snake, walkedIncrease, this, walkedCount);
}
void MainWindow::changeAlgorithm(Algorithm_e algo)
{
    if (modeSelection.getMode() == MODE_REPLAY)
    {
        qDebug() << "Replay the game, cannot change the algorithm";
        return;
    }
    if (algo == ALGORITHM_MAX)
    {
        algo = algorithmSelection.getAlgo();
        algo = Algorithm_e((algo + 1) % ALGORITHM_MAX);
    }
    algorithmSelection.setAlgo(algo);

    QString str = "寻路算法: ";
    str += algorithmSelection.getAlgoString();
    algorithmLabel->setText(str);
}

void MainWindow::setSnakeLength(int length)
{
    QString str = "蛇身长度: ";
    str += QString(std::to_string(length).data());
    snakeLengthLabel->setText(str);
}

void MainWindow::setSnakeWalkedCount(int count)
{
    QString str = "走过的距离: ";
    str += QString(std::to_string(count).data());
    snakeWalkedLabel->setText(str);
}

void MainWindow::restartGame()
{
    qDebug() << "Restart the game";
    gameControl.setMode(GAME_PLAY_CONTINUE);
    dataRecorder.reset();
    snake.reset();
}

void MainWindow::pauseGame()
{
    qDebug() << "Pause the game";
    gameControl.setMode(GAME_PLAY_PAUSE);
}

void MainWindow::continueGame()
{
    qDebug() << "Continue the game";
    gameControl.setMode(GAME_PLAY_CONTINUE);
}

bool MainWindow::isGamePause()
{
    return gameControl.getMode() == GAME_PLAY_PAUSE;
}

void MainWindow::changeManualMode()
{
    modeSelection.setMode(MODE_MANUAL);
    gameModeLabel->setText("模式： " + modeSelection.getModeString());
}

void MainWindow::changeAutoMode()
{
    modeSelection.setMode(MODE_AUTO);
    gameModeLabel->setText("模式： " + modeSelection.getModeString());
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
    if (gameControl.getMode() == GAME_PLAY_PAUSE)
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
    int timeInterval = speedSelection.getSpeed();
    if(event->delta() < 0)
    {
        if (timeInterval < 200)
        {
            timeInterval = std::min(timeInterval + 10, 200);
        }
    }
    else
    {
        if (timeInterval > 10)
        {
           timeInterval = std::max(timeInterval - 10, 10);
        }
    }
    speedLabel->setText("速度: " + speedSelection.getSpeedString());
    speedSelection.setSpeed(timeInterval);
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

