#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <vector>
#include <QString>

typedef enum
{
    GAME_PLAY_UNSTART,
    GAME_PLAY_CONTINUE,
    GAME_PLAY_PAUSE,
    GAME_PLAY_DEAD,
    GAME_PLAY_MAX
}Game_Mode_e;

class StatusSelection
{
public:
    StatusSelection(Game_Mode_e gameMode = GAME_PLAY_UNSTART) : gameMode(gameMode) {}
    Game_Mode_e getMode() { return gameMode; }
    void setMode(Game_Mode_e gameMode) { this->gameMode = gameMode; }
    QString getStatusString() { return gameControlString[gameMode]; }

private:
    Game_Mode_e gameMode;
    QString gameControlString[GAME_PLAY_MAX] = { "未开始", "进行中", "暂停", "结束" };
};

#endif // GAMECONTROL_H
