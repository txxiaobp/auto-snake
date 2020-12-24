#include "seed.h"
#include "screen_data.h"
#include <cstdlib>
#include <ctime>

void Seed::set(bool initial)
{
    if (!initial)
    {
        data.setType(nodeRow, nodeCol, Node_snake_head);
    }

	srand(time(NULL));
	int row_bound = data.row();
	int col_bound = data.col();

	do
	{
        nodeRow = rand() % row_bound;
        nodeCol = rand() % col_bound;
    } while(data.getType(nodeRow, nodeCol) != Node_available);
	
    data.setType(nodeRow, nodeCol, Node_seed);
}

int Seed::getNode()
{
    return nodeRow * data.col() + nodeCol;
}











