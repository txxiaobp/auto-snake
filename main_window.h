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
               int radius,
               DataRecorder& dataRecorder,
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
    ScreenData& data;
    DataRecorder& dataRecorder;
    Snake& snake;
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
    QAction *startAction;
    QAction *restartAction;
    QAction *pauseAction;
    QAction *exitAction;
    QAction *autoAction;
    QAction *manualAction;
    QAction *replayAction;
    std::vector<QAction *> actionVec;
    QStatusBar *sBar;
    QLabel * snakeLengthLabel;
    QLabel * snakeWalkedLabel;
    QLabel * gameModeLabel;
    QLabel * gameStatusLabel;
    QLabel * algorithmLabel;
    QLabel * speedLabel;

private:
    void setPainterMap(int radius);
    void connectSignals();
    void gameEnd();

signals:

};

#endif // MAINWINDOW_H
