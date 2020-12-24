#include <iostream>
#include <conio.h>
#include "qt_screen.h"

#define Screen_available    ' '
#define Screen_snake_body   '*'
#define Screen_snake_head   '@'
#define Screen_wall         '#'
#define Screen_seed         '$'

void QtScreen::show()
{
	int row = data.row();
	int col = data.col();
	
	for (int r = 0; r < row; r++)
	{
		for (int c = 0; c < col; c++)
		{
			char show_char;
			switch(data.get_type(r, c))
			{				
			case Node_available:
				show_char = Screen_available;
				break;
				
			case Node_snake_body:
				show_char = Screen_snake_body;
				break;
				
			case Node_snake_head:
				show_char = Screen_snake_head;
				break;
				
			case Node_wall:
				show_char = Screen_wall;
				break;
				
			case Node_seed:
				show_char = Screen_seed;
				break;
				
			default:
				break;	
			}
			
			std::cout << show_char;
		}
		std::cout << std::endl;
	}
	delay();
	system("cls");
}

void QtScreen::delay()
{
	long long time = 100 * 1000000;
	for (long long i = 0; i < time; i++) {}
}

	
	
	
	
