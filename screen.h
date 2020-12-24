#ifndef SCREEN_H
#define SCREEN_H

#include "screen_data.h"
#include "snake.h"

class Screen
{
protected:
	ScreenData& data;
	Snake& snake;
	
public:
	Screen(ScreenData& data, Snake& snake) : data(data), snake(snake) {};
	virtual void show() = 0;
};

#endif
