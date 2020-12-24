#include "seed.h"
#include "screen_data.h"
#include <cstdlib>
#include <ctime>

void Seed::set(bool initial)
{
    if (!initial)
    {
        data.set_type(nodeRow, nodeCol, Node_snake_head);
    }

	srand(time(NULL));
	int row_bound = data.row();
	int col_bound = data.col();

	do
	{
        nodeRow = rand() % row_bound;
        nodeCol = rand() % col_bound;
    } while(data.get_type(nodeRow, nodeCol) != Node_available);
	
    data.set_type(nodeRow, nodeCol, Node_seed);
}

int Seed::get_node()
{
    return nodeRow * data.col() + nodeCol;
}











