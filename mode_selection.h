#ifndef MODESELECTION_H
#define MODESELECTION_H

#include <QString>
#include <QObject>

typedef enum
{
    MODE_AUTO,
    MODE_MANUAL,
    MODE_REPLAY,
    MODE_MAX
}Mode_e;


class ModeSelection : public QObject
{
    Q_OBJECT

public:
    ModeSelection(Mode_e mode) : mode(mode) {}
    Mode_e getMode() { return mode; }
    QString getModeString() { return modeString[int(mode)]; }
    void setMode(Mode_e mode) { this->mode = mode; emit modeChanged(); }

signals:
    void modeChanged();

private:
    Mode_e mode;
    QString modeString[MODE_MAX] = { "自动", "手动", "重放" };
};

#endif // MODESELECTION_H
