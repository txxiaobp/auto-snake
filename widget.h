#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <unordered_map>
#include "screen_data.h"
#include "snake.h"
#include "screen_painter.h"
#include "mode_selection.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


class DataRecorder;


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(ScreenData& data, Snake& snake, int radius, DataRecorder& dataRecorder, ModeSelection& modeSelection, QWidget *parent = nullptr);
    ~Widget();

    void changeAlgorithm(search_method_e method = METHOD_MAX);
    void restartGame();
    void pauseGame();
    void continueGame();
    bool isGamePause();

    void timerEvent(QTimerEvent* ev);
    void paintEvent(QPaintEvent* ev);
    void wheelEvent(QWheelEvent *event);
    virtual void keyPressEvent(QKeyEvent *ev);

private:
    void setPainterMap(int radius);

private:
    Ui::Widget *ui;
    ScreenData& data;
    DataRecorder& dataRecorder;
    Snake& snake;
    std::unordered_map<int, ScreenPainter*> painterMap;
    int radius;
    QPainter* painter;
    int timerId;
    int timeInterval;
    bool isPause;
    ModeSelection& modeSelection;
};
#endif // WIDGET_H
