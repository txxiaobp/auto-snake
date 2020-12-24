#ifndef CONSOLE_SCREEN_H
#define CONSOLE_SCREEN_H

#include "screen.h"

class QtScreen : public Screen
{
private:
	void delay();
	
public:
    QtScreen(ScreenData& data, Snake& snake) : Screen(data, snake) {}
	void show();
};

void show(QtScreen& screen);

#endif
