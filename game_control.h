#ifndef GAMECONTROL_H
#define GAMECONTROL_H

typedef enum
{
    GAME_PLAY_CONTINUE,
    GAME_PLAY_PAUSE
}Game_Mode_e;

class GameControl
{
public:
    GameControl(Game_Mode_e gameMode = GAME_PLAY_CONTINUE) : gameMode(gameMode) {}
    Game_Mode_e getMode() { return gameMode; }
    void setMode(Game_Mode_e gameMode) { this->gameMode = gameMode; }

private:
    Game_Mode_e gameMode;
};

#endif // GAMECONTROL_H
