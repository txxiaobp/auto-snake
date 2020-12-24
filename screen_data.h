#ifndef SCREEN_DATA_H
#define SCREEN_DATA_H

#include <vector>

class Snake;
class Seed;

typedef enum
{
    Node_undefined,
    Node_available,
    Node_snake_body,
    Node_snake_head,
    Node_wall,
    Node_seed
} Node_t;

class ScreenData
{	
private:
    std::vector<std::vector<Node_t>> data;
	
private:
	void set_wall();
	
public:
	ScreenData(int row = 10, int col = 10);
	int row();
	int col();
    Node_t get_type(int row, int col);
	void set_type(int row, int col, Node_t node_type);
	
	friend class Snake;
	friend class Seed;
};

#endif
