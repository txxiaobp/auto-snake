#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QStatusBar>
#include <QLabel>
#include <QMainWindow>
#include <vector>

#include <QWidget>
#include <unordered_map>
#include "screen_data.h"
#include "snake.h"
#include "obstacle.h"
#include "screen_painter.h"
#include "mode_selection.h"
#include "speed_selection.h"
#include "game_control.h"

class DataRecorder;


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(ScreenData& data,
               Snake& snake,
               Seed& seed,
               int radius,
               DataRecorder& dataRecorder,
               Obstacle& obstacle,
               ModeSelection& modeSelection,
               AlgorithmSelection& algorithmSelection,
               SpeedSelection& speedSelection,
               StatusSelection& statusSelection,
               QWidget *parent = nullptr);
    ~MainWindow();

    void changeAlgorithm(Algorithm_e algo = ALGORITHM_MAX);
    void startGame();
    void restartGame();
    void setStatusMode(Game_Mode_e mode);
    void changeMode(Mode_e mode);

    void timerEvent(QTimerEvent* ev);
    void paintEvent(QPaintEvent* ev);
    void wheelEvent(QWheelEvent *event);
    virtual void keyPressEvent(QKeyEvent *ev);

public slots:
    void setSnakeLength(int length);
    void setSnakeWalkedCount(int count);

private:
    void setPainterMap(int radius);
    void connectSignals();
    void gameEnd();
    void speedUp();
    void speedDown();
    void resetObstacle();
    void pauseContinueGame();

private:
    ScreenData& data;
    DataRecorder& dataRecorder;
    Snake& snake;
    Seed& seed;
    Obstacle& obstacle;
    std::unordered_map<int, ScreenPainter*> painterMap;
    int radius;
    QPainter* painter;
    ModeSelection& modeSelection;
    AlgorithmSelection& algorithmSelection;
    SpeedSelection& speedSelection;
    StatusSelection& statusSelection;
    int timerId;

private:
    QMenuBar * mBar;
    QMenu *gameMenu;
    QMenu *modeMenu;
    QMenu *algoMenu;
    QMenu *speedMenu;
    QAction *startAction;
    QAction *restartAction;
    QAction *pauseAction;
    QAction *resetObstacleAction;
    QAction *exitAction;
    QAction *autoAction;
    QAction *manualAction;
    QAction *replayAction;
    QAction *bfsAction;
    QAction *dijkstraAction;
    QAction *aStarAction;
    QAction *dStarAction;
    QAction *dStarLiteAction;
    QAction *speedUpAction;
    QAction *speedDownAction;
    QStatusBar *sBar;
    QLabel * snakeLengthLabel;
    QLabel * snakeWalkedLabel;
    QLabel * gameModeLabel;
    QLabel * gameStatusLabel;
    QLabel * algorithmLabel;
    QLabel * speedLabel;
};

#endif // MAINWINDOW_H
