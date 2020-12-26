#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QStatusBar>
#include <QLabel>
#include <QMainWindow>

#include <QWidget>
#include <unordered_map>
#include "screen_data.h"
#include "snake.h"
#include "screen_painter.h"
#include "mode_selection.h"
#include "speed_selection.h"


class DataRecorder;


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(ScreenData& data, Snake& snake, int radius, DataRecorder& dataRecorder, ModeSelection& modeSelection, AlgorithmSelection& algorithmSelection, QWidget *parent = nullptr);
    ~MainWindow();

    void changeAlgorithm(Algorithm_e algo = ALGORITHM_MAX);
    void restartGame();
    void pauseGame();
    void continueGame();
    bool isGamePause();
    void changeManualMode();
    void changeAutoMode();

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
    SpeedSelection speedSelection;
    int timerId;
    bool isPause;

private:
    QMenuBar * mBar;
    QMenu *gameMenu;
    QMenu *algoMenu;
    QAction *startAction;
    QAction *restartAction;
    QAction *pauseAction;
    QAction *bfsAction;
    QAction *dijkAction;
    QStatusBar *sBar;
    QLabel * snakeLengthLabel;
    QLabel * snakeWalkedLabel;
    QLabel * gameModeLabel;
    QLabel * algorithmLabel;
    QLabel * speedLabel;

private:
    void setPainterMap(int radius);
    void connectSignals();



signals:

};

#endif // MAINWINDOW_H
