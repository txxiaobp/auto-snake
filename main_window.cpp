#include "main_window.h"
#include <QTimer>
#include <QDebug>
#include <QWheelEvent>
#include <QString>
#include <QMessageBox>
#include <QFileDialog>
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
                       StatusSelection& statusSelection,
                       QWidget *parent)
    : QMainWindow(parent)
    , data(data)
    , dataRecorder(dataRecorder)
    , snake(snake)
    , radius(radius)
    , modeSelection(modeSelection)
    , algorithmSelection(algorithmSelection)
    , speedSelection(speedSelection)
    , statusSelection(statusSelection)

    , mBar(menuBar())
    , gameMenu(mBar->addMenu("游戏"))
    , modeMenu(mBar->addMenu("模式"))
    , algoMenu(mBar->addMenu("寻路算法"))
    , startAction(gameMenu->addAction("开始 (S)"))
    , restartAction(gameMenu->addAction("重新开始 (R)"))
    , pauseAction(gameMenu->addAction("暂停 (P)"))
    , exitAction(gameMenu->addAction("退出 (E)"))
    , autoAction(modeMenu->addAction("自动 (A)"))
    , manualAction(modeMenu->addAction("手动 (M)"))
    , replayAction(modeMenu->addAction("重放"))

    , sBar(statusBar())
    , snakeLengthLabel(new QLabel("蛇身长度: 2"))
    , snakeWalkedLabel(new QLabel("走过的距离: 0"))
    , gameModeLabel(new QLabel("模式: " + modeSelection.getModeString()))
    , gameStatusLabel(new QLabel("状态: " + statusSelection.getStatusString()))
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
    mBar->addMenu(modeMenu);
    mBar->addMenu(algoMenu);

    gameMenu->addAction(startAction);
    gameMenu->addAction(restartAction);
    gameMenu->addAction(pauseAction);
    gameMenu->addAction(exitAction);

    modeMenu->addAction(autoAction);
    modeMenu->addAction(manualAction);
    modeMenu->addAction(replayAction);

    std::map<Algorithm_e, QString> algoStringMap = algorithmSelection.getAlgoStringMap();
    for (auto& pair : algoStringMap)
    {
        QAction *algoAction = algoMenu->addAction(pair.second);
        actionVec.push_back(algoAction);
        algoMenu->addAction(algoAction);
        connect(algoAction, &QAction::triggered, [&](){
            changeAlgorithm(pair.first);
        });
    }

    setStatusBar(sBar);
    sBar->addWidget(snakeLengthLabel);
    sBar->addWidget(snakeWalkedLabel);
    sBar->addWidget(gameModeLabel);
    sBar->addWidget(gameStatusLabel);
    sBar->addWidget(algorithmLabel);
    sBar->addWidget(speedLabel);

    snakeLengthLabel->setFixedSize(80, BAR_HEIGHT);
    snakeWalkedLabel->setFixedSize(100, BAR_HEIGHT);
    gameModeLabel->setFixedSize(65, BAR_HEIGHT);
    gameStatusLabel->setFixedSize(70, BAR_HEIGHT);
    algorithmLabel->setFixedSize(135, BAR_HEIGHT);
    speedLabel->setFixedSize(60, BAR_HEIGHT);

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


    connect(startAction, &QAction::triggered, [&](){
        startGame();
    });
    connect(restartAction, &QAction::triggered, [&](){
        restartGame();
    });
    connect(exitAction, &QAction::triggered, [&](){
        exit(0);
    });

    connect(pauseAction, &QAction::triggered, [&](){
        setStatusMode(GAME_PLAY_PAUSE);
    });

    connect(autoAction, &QAction::triggered, [&](){
        changeMode(MODE_AUTO);
    });
    connect(manualAction, &QAction::triggered, [&](){
        changeMode(MODE_MANUAL);
    });
    connect(replayAction, &QAction::triggered, [&](){
        changeMode(MODE_REPLAY);
    });

}

void MainWindow::changeAlgorithm(Algorithm_e algo)
{
    if (modeSelection.getMode() == MODE_REPLAY)
    {
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

void MainWindow::startGame()
{
    if (statusSelection.getMode() == GAME_PLAY_DEAD)
    {
        restartGame();
        return;
    }
    if (modeSelection.getMode() == MODE_REPLAY)
    {
        QString fileName = QFileDialog::getOpenFileName(this,
                tr("请选择要读取的数据文件"),
                "",
                tr(""));

        if (!fileName.isNull())
        {
             dataRecorder.reset(fileName.toUtf8().data());
             snake.reset();
             setStatusMode(GAME_PLAY_CONTINUE);
        }
        return;
    }

    setStatusMode(GAME_PLAY_CONTINUE);
}
void MainWindow::restartGame()
{
    dataRecorder.reset();
    snake.reset();
    setStatusMode(GAME_PLAY_CONTINUE);
}

void MainWindow::setStatusMode(Game_Mode_e mode)
{
    statusSelection.setMode(mode);
    gameStatusLabel->setText("状态: " + statusSelection.getStatusString());
}

void MainWindow::changeMode(Mode_e mode)
{
    if (modeSelection.getMode() == MODE_REPLAY)
    {
        return;
    }
    modeSelection.setMode(mode);
    gameModeLabel->setText("模式: " + modeSelection.getModeString());
}

void MainWindow::gameEnd()
{
    setStatusMode(GAME_PLAY_DEAD);
    if (modeSelection.getMode() == MODE_REPLAY)
    {
        QMessageBox::StandardButton rb = QMessageBox::question(NULL, "播放完毕", "播放完毕，是否重放?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(rb == QMessageBox::Yes)
        {
            restartGame();
        }
        return;
    }

    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "本局结束", "你想保存本局的数据吗?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        QString fileName = QFileDialog::getSaveFileName(this,
                tr("请选择本局数据的保存路径"),
                "",
                tr(""));

            if (!fileName.isNull())
            {
                 dataRecorder.exportToFile(fileName.toUtf8().data());
            }
    }
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
    if (statusSelection.getMode() != GAME_PLAY_CONTINUE)
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
        gameEnd();
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
    if(ev->key() == Qt::Key_S)
    {
        startGame();
        return;
    }
    if(ev->key() == Qt::Key_R)
    {
        restartGame();
        return;
    }
    if(ev->key() == Qt::Key_M)
    {
        changeMode(MODE_MANUAL);
        return;
    }
    if(ev->key() == Qt::Key_A)
    {
        changeMode(MODE_AUTO);
        return;
    }
    if(ev->key() == Qt::Key_P)
    {
        setStatusMode(GAME_PLAY_PAUSE);
        return;
    }
}

