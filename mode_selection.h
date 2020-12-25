#ifndef MODESELECTION_H
#define MODESELECTION_H

typedef enum
{
    MANUAL_MODE,
    AUTO_MODE,
    REPLAY_MODE
}Mode_e;


class ModeSelection
{
public:
    ModeSelection(Mode_e mode) : mode(mode) {}
    Mode_e getMode() { return mode; }
    void setMode(Mode_e mode) { this->mode = mode; }

private:
    Mode_e mode;
};

#endif // MODESELECTION_H
