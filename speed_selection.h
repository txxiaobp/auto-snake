#ifndef SPEED_SELECTION_H
#define SPEED_SELECTION_H

#include <QObject>
#include <QString>
#include "mode_selection.h"

enum
{
    DEFAULT_AUTO_SNAKE_SPEED = 100,
    DEFAULT_MANUAL_SNAKE_SPEED = 200
};

class SpeedSelection : public QObject
{
    Q_OBJECT
public:
    SpeedSelection(ModeSelection& modeSelection)
        : modeSelection(modeSelection)
    {
        setInitSpeed();
        connect(&modeSelection, &ModeSelection::modeChanged, this, &SpeedSelection::setInitSpeed);
    }
    int getSpeed() { return speed; }
    QString getSpeedString() { return std::to_string(speed).data(); }
    void setSpeed(int speed) { this->speed = speed; emit changeSpeed(); }
    void setInitSpeed()
    {
        switch (modeSelection.getMode())
        {
        case MODE_AUTO:
            speed = DEFAULT_AUTO_SNAKE_SPEED;
            break;
        case MODE_MANUAL:
            speed = DEFAULT_MANUAL_SNAKE_SPEED;
            break;
        default:
            break;
        }
    }

signals:
    void changeSpeed();

private:
    int speed;
    ModeSelection& modeSelection;
};

#endif // SPEED_SELECTION_H
