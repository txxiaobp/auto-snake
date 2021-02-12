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
#include "obstacle_painter.h"


static const int BAR_HEIGHT = 30;
static const int SNAKE_LENGTH_LABEL = 100;
static const int SNAKE_WALKED_LABEL = 130;
static const int GAME_MODE_LABEL = 85;
static const int GAME_STATUS = 95;
static const int OBSTACLE_MODE = 90;
static const int ALGORITHM_LABEL = 210;
static const int SPEED_LABEL = 80;

MainWindow::MainWindow(ScreenData& data,
                       Snake& snake,
                       Seed& seed,
                       int radius,
                       DataRecorder& dataRecorder,
                       Obstacle& obstacle,
                       ModeSelection& modeSelection,
                       AlgorithmSelection& algorithmSelection,
                       SpeedSelection& speedSelection,
                       StatusSelection& statusSelection,
                       QWidget *parent)
    : QMainWindow(parent)
    , data(data)
    , dataRecorder(dataRecorder)
    , snake(snake)
    , seed(seed)
    , obstacle(obstacle)
    , radius(radius)
    , modeSelection(modeSelection)
    , algorithmSelection(algorithmSelection)
    , speedSelection(speedSelection)
    , statusSelection(statusSelection)
    , containObstacle(true)

    , mBar(menuBar())
    , gameMenu(mBar->addMenu("游戏"))
    , modeMenu(mBar->addMenu("模式"))
    , obstacleMenu(mBar->addMenu("障碍物"))
    , algoMenu(mBar->addMenu("寻路算法"))
    , speedMenu(mBar->addMenu("速度控制"))
    , startAction(gameMenu->addAction("开始 (S)"))
    , restartAction(gameMenu->addAction("重新开始 (R)"))
    , pauseContinueAction(gameMenu->addAction("暂停|继续 (P)"))
    , endGameAction(gameMenu->addAction("停止 (E)"))

    , exitAction(gameMenu->addAction("退出 (E)"))
    , autoAction(modeMenu->addAction("自动 (A)"))
    , manualAction(modeMenu->addAction("手动 (M)"))
    , replayAction(modeMenu->addAction("重放"))
    , obstacleModeAction(obstacleMenu->addAction("关闭障碍物"))
    , resetObstacleAction(obstacleMenu->addAction("重新设置障碍物 (O)"))
    , bfsAction(algoMenu->addAction("广度优先搜索"))
    , debfsAction(algoMenu->addAction("双端广度优先搜索"))
    , dijkstraAction(algoMenu->addAction("Dijkstra"))
    , aStarAction(algoMenu->addAction("A*"))
//    , dStarAction(algoMenu->addAction("D*"))
//    , dStarLiteAction(algoMenu->addAction("D* Lite"))
    , speedUpAction(speedMenu->addAction("加速"))
    , speedDownAction(speedMenu->addAction("减速"))
    , sBar(statusBar())
    , snakeLengthLabel(new QLabel("蛇身长度: 2"))
    , snakeWalkedLabel(new QLabel("走过的距离: 0"))
    , gameModeLabel(new QLabel("模式: " + modeSelection.getModeString()))
    , gameStatusLabel(new QLabel("状态: " + statusSelection.getStatusString()))
    , obstacleLabel(new QLabel("障碍物: 有"))
    , algorithmLabel(new QLabel("寻路算法: " + algorithmSelection.getAlgoString()))
    , speedLabel(new QLabel("速度: " + speedSelection.getSpeedString()))
{
    int height = data.getRow() * 2 * radius;
    int width = data.getCol() * 2 * radius;
    resize(width, height + 2 * BAR_HEIGHT);

    this->setWindowTitle("贪吃蛇");
    this->setWindowIcon(QIcon(":/res/images/title.jpg"));

    setMenuBar(mBar);
    setStatusBar(sBar);
    sBar->addWidget(snakeLengthLabel);
    sBar->addWidget(snakeWalkedLabel);
    sBar->addWidget(gameModeLabel);
    sBar->addWidget(gameStatusLabel);
    sBar->addWidget(obstacleLabel);
    sBar->addWidget(algorithmLabel);
    sBar->addWidget(speedLabel);

    snakeLengthLabel->setFixedSize(SNAKE_LENGTH_LABEL, BAR_HEIGHT);
    snakeWalkedLabel->setFixedSize(SNAKE_WALKED_LABEL, BAR_HEIGHT);
    gameModeLabel->setFixedSize(GAME_MODE_LABEL, BAR_HEIGHT);
    gameStatusLabel->setFixedSize(GAME_STATUS, BAR_HEIGHT);
    obstacleLabel->setFixedSize(OBSTACLE_MODE, BAR_HEIGHT);
    algorithmLabel->setFixedSize(ALGORITHM_LABEL, BAR_HEIGHT);
    speedLabel->setFixedSize(SPEED_LABEL, BAR_HEIGHT);

    connectSignals();

    painter = new QPainter(this);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    setPainterMap(radius);

    timerId = startTimer(speedSelection.getSpeed());
}


MainWindow::~MainWindow()
{
    delete painter;
    delete painterMap[NODE_SNAKE_BODY];
    delete painterMap[NODE_SNAKE_HEAD];
    delete painterMap[NODE_WALL];
    delete painterMap[NODE_SEED];
}

void MainWindow::connectSignals()
{
    void (Snake:: *lengthIncrease)(int) = &Snake::snakeLengthIncrease;
    void (MainWindow:: *snakeLength)(int) = &MainWindow::setSnakeLength;
    connect(&snake, lengthIncrease, this, snakeLength);

    void (Snake:: *walkedIncrease)(int) = &Snake::snakeWalkedIncrease;
    void (MainWindow:: *walkedCount)(int) = &MainWindow::setSnakeWalkedCount;
    connect(&snake, walkedIncrease, this, walkedCount);


    connect(startAction, &QAction::triggered, this, [&](){
        startGame();
    });
    connect(restartAction, &QAction::triggered, this, [&](){
        restartGame();
    });
    connect(pauseContinueAction, &QAction::triggered, this, [&](){
        pauseContinueGame();
    });
    connect(obstacleModeAction, &QAction::triggered, this, [&](){
        setObstacleMode(!containObstacle);
    });
    connect(resetObstacleAction, &QAction::triggered, this, [&](){
        resetObstacle();
    });
    connect(endGameAction, &QAction::triggered, this, [&](){
        gameEnd(true);
    });

    connect(exitAction, &QAction::triggered, [&](){
        exit(0);
    });

    connect(autoAction, &QAction::triggered, this, [&](){
        changeMode(MODE_AUTO);
    });
    connect(manualAction, &QAction::triggered, this, [&](){
        changeMode(MODE_MANUAL);
    });
    connect(replayAction, &QAction::triggered, this, [&](){
        changeMode(MODE_REPLAY);
    });
    connect(bfsAction, &QAction::triggered, this, [&](){
        changeAlgorithm(ALGORITHM_BFS);
    });
    connect(debfsAction, &QAction::triggered, this, [&](){
        changeAlgorithm(ALGORITHM_DEBFS);
    });
    connect(dijkstraAction, &QAction::triggered, this, [&](){
        changeAlgorithm(ALGORITHM_DIJKSTRA);
    });
    connect(aStarAction, &QAction::triggered, this, [&](){
        changeAlgorithm(ALGORITHM_ASTAR);
    }); 
//    connect(dStarAction, &QAction::triggered, this, [&](){
//        changeAlgorithm(ALGORITHM_DSTAR);
//    });
//    connect(dStarLiteAction, &QAction::triggered, this, [&](){
//        changeAlgorithm(ALGORITHM_DSTAR_LITE);
//    });
    connect(speedUpAction, &QAction::triggered, this, [&](){
        speedUp();
    });
    connect(speedDownAction, &QAction::triggered, this, [&](){
        speedDown();
    });
}

void MainWindow::pauseContinueGame()
{
    if (statusSelection.getMode() == GAME_PLAY_CONTINUE)
    {
        setStatusMode(GAME_PLAY_PAUSE);
    }
    else if (statusSelection.getMode() == GAME_PLAY_PAUSE)
    {
        setStatusMode(GAME_PLAY_CONTINUE);
    }
}

void MainWindow::setObstacleMode(bool isSet)
{
    assert (isSet != containObstacle);

    if (isSet)
    {
        if (statusSelection.getMode() == GAME_PLAY_CONTINUE || statusSelection.getMode() == GAME_PLAY_PAUSE)
        {
            Game_Mode_e prevGameMode = statusSelection.getMode();
            statusSelection.setMode(GAME_PLAY_PAUSE);
            QMessageBox::information(NULL, "操作错误", "正在进行游戏，无法启用障碍物，请结束后重试。",
                                     QMessageBox::Yes, QMessageBox::Yes);
            statusSelection.setMode(prevGameMode);
        }
        else
        {
            snake.reset();
            obstacle.resetObstacle();
            update();
            containObstacle = true;
        }
    }
    else
    {
        if (statusSelection.getMode() == GAME_PLAY_CONTINUE || statusSelection.getMode() == GAME_PLAY_PAUSE)
        {
            Game_Mode_e prevGameMode = statusSelection.getMode();
            statusSelection.setMode(GAME_PLAY_PAUSE);
            QMessageBox::information(NULL, "操作错误", "正在进行游戏，无法关闭障碍物，请结束后重试。",
                                     QMessageBox::Yes, QMessageBox::Yes);
            statusSelection.setMode(prevGameMode);
        }
        else
        {
            snake.reset();
            obstacle.clearObstacle();
            update();
            containObstacle = false;
        }
    }
    if (containObstacle)
    {
        obstacleLabel->setText("障碍物: 有");
        obstacleModeAction->setText("关闭障碍物");
    }
    else
    {
        obstacleLabel->setText("障碍物: 无");
        obstacleModeAction->setText("启用障碍物");
    }
}

bool MainWindow::resetObstacle()
{
    if (!containObstacle)
    {
        Game_Mode_e prevGameMode = statusSelection.getMode();
        statusSelection.setMode(GAME_PLAY_PAUSE);
        QMessageBox::information(NULL, "操作错误", "未启用障碍物，无法重新设置，请启用障碍物后重试。",
                                 QMessageBox::Yes, QMessageBox::Yes);
        statusSelection.setMode(prevGameMode);
        return containObstacle;
    }
    if (statusSelection.getMode() == GAME_PLAY_CONTINUE || statusSelection.getMode() == GAME_PLAY_PAUSE)
    {
        Game_Mode_e prevGameMode = statusSelection.getMode();
        statusSelection.setMode(GAME_PLAY_PAUSE);
        QMessageBox::information(NULL, "操作错误", "正在进行游戏，无法重新设置障碍物，请结束后重试。",
                                 QMessageBox::Yes, QMessageBox::Yes);
        statusSelection.setMode(prevGameMode);
        return containObstacle;
    }

    snake.reset();
    obstacle.resetObstacle();
    update();
    return true;
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

    setStatusMode(GAME_PLAY_CONTINUE);
}
void MainWindow::restartGame()
{ 
    Game_Mode_e prevGameMode = statusSelection.getMode();
    if (prevGameMode == GAME_PLAY_CONTINUE || prevGameMode == GAME_PLAY_PAUSE)
    {
        statusSelection.setMode(GAME_PLAY_PAUSE);
        QMessageBox::StandardButton rb = QMessageBox::question(NULL, "是否重新开始", "是否结束本局并立即重新开始?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if(rb == QMessageBox::No)
        {
            statusSelection.setMode(prevGameMode);
            return;
        }
    }

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
    Game_Mode_e prevGameMode = statusSelection.getMode();
    if (prevGameMode == GAME_PLAY_CONTINUE || prevGameMode == GAME_PLAY_PAUSE)
    {
        statusSelection.setMode(GAME_PLAY_PAUSE);
        if (modeSelection.getMode() == MODE_AUTO && mode == MODE_MANUAL)
        {
            QMessageBox::StandardButton rb = QMessageBox::question(NULL, "切换模式", "是否切换为手动模式?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if(rb == QMessageBox::Yes)
            {
                modeSelection.setMode(MODE_MANUAL);
            }
            else
            {
                statusSelection.setMode(prevGameMode);
                return;
            }
        }
        else if (modeSelection.getMode() == MODE_MANUAL && mode == MODE_AUTO)
        {
            QMessageBox::StandardButton rb = QMessageBox::question(NULL, "切换模式", "是否切换为自动模式?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if(rb == QMessageBox::Yes)
            {
                modeSelection.setMode(MODE_AUTO);
            }
            else
            {
                statusSelection.setMode(prevGameMode);
                return;
            }
        }
        else if (modeSelection.getMode() == MODE_REPLAY)
        {
            if (mode == MODE_AUTO)
            {
                QMessageBox::information(NULL, "操作错误", "正处于重放模式，无法设置为自动模式，请结束后重试。",
                                         QMessageBox::Yes, QMessageBox::Yes);
                statusSelection.setMode(prevGameMode);
                return;
            }
            else if (mode == MODE_MANUAL)
            {
                QMessageBox::information(NULL, "操作错误", "正处于重放模式，无法设置为手动模式，请结束后重试。",
                                         QMessageBox::Yes, QMessageBox::Yes);
                statusSelection.setMode(prevGameMode);
                return;
            }
        }
    }

    if (modeSelection.getMode() != MODE_REPLAY && mode == MODE_REPLAY)
    {
        if (prevGameMode == GAME_PLAY_CONTINUE || prevGameMode == GAME_PLAY_PAUSE)
        {
            statusSelection.setMode(GAME_PLAY_PAUSE);
            QMessageBox::StandardButton rb = QMessageBox::question(NULL, "切换模式", "游戏进行中，是否切换为重放模式并选择存档文件?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if(rb == QMessageBox::No)
            {
                statusSelection.setMode(prevGameMode);
                return;
            }
        }
        modeSelection.setMode(MODE_REPLAY);

        QString fileName = QFileDialog::getOpenFileName(this,
                tr("请选择要读取的数据文件"),
                "",
                tr(""));

        if (!fileName.isNull())
        {
             dataRecorder.reset(fileName.toUtf8().data());
             snake.reset();
        }
    }

    statusSelection.setMode(prevGameMode);
    modeSelection.setMode(mode);
    gameModeLabel->setText("模式: " + modeSelection.getModeString());
    update();
}

void MainWindow::gameEnd(bool manuallyEnd)
{
    if (manuallyEnd)
    {
        Game_Mode_e prevGameMode = statusSelection.getMode();
        statusSelection.setMode(GAME_PLAY_PAUSE);
        QMessageBox::StandardButton rb = QMessageBox::question(NULL, "结束游戏", "是否结束游戏?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        statusSelection.setMode(prevGameMode);
        if(rb == QMessageBox::No)
        {
            return;
        }
    }

    setStatusMode(GAME_PLAY_DEAD);
    if (modeSelection.getMode() == MODE_REPLAY && manuallyEnd == false)
    {
        QMessageBox::StandardButton rb = QMessageBox::question(NULL, "播放完毕", "播放完毕，是否重放?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(rb == QMessageBox::Yes)
        {
            restartGame();
        }
        return;
    }

    if (dataRecorder.isDataEmpty())
    {
        return;
    }
    if (modeSelection.getMode() == MODE_REPLAY)
    {
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
    painterMap[NODE_AVAILABLE] = new BackPainter(radius);
    painterMap[NODE_SNAKE_BODY] = new BodyPainter(radius);
    painterMap[NODE_SNAKE_HEAD] = new HeadPainter(radius, snake);
    painterMap[NODE_WALL] = new WallPainter(radius);
    painterMap[NODE_SEED] = new SeedPainter(radius);
    painterMap[NODE_OBSTACLE] = new ObstaclePainter(radius);
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
        gameEnd();
    }
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter qp(this);

    int row = data.getRow();
    int col = data.getCol();

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

void MainWindow::speedDown()
{
    killTimer(timerId);
    int timeInterval = speedSelection.getSpeed();
    if (timeInterval < 5000)
    {
        timeInterval = std::min(timeInterval + 10, 5000);
    }
    speedLabel->setText("速度: " + speedSelection.getSpeedString());
    speedSelection.setSpeed(timeInterval);
    timerId = startTimer(timeInterval);
}

void MainWindow::speedUp()
{
    killTimer(timerId);
    int timeInterval = speedSelection.getSpeed();
    if (timeInterval > 1)
    {
       timeInterval = std::max(timeInterval - 10, 1);
    }
    speedLabel->setText("速度: " + speedSelection.getSpeedString());
    speedSelection.setSpeed(timeInterval);
    timerId = startTimer(timeInterval);
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() < 0)
    {
        speedDown();

    }
    else
    {
        speedUp();
    }
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
    if(ev->key() == Qt::Key_O)
    {
        resetObstacle();
        return;
    }
    if(ev->key() == Qt::Key_E)
    {
        gameEnd(true);
        return;
    }
}

