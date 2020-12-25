#include "seed.h"
#include "screen_data.h"
#include "data_recorder.h"
#include <cstdlib>
#include <ctime>


void Seed::set(bool initial)
{
    if (!initial)
    {
        data.setType(nodeRow, nodeCol, Node_snake_head);
    }

    if (modeSelection.getMode() == MODE_REPLAY)
    {
        setFromRecorder();
        return;
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
    setData();
}

int Seed::getNode()
{
    return nodeRow * data.col() + nodeCol;
}

void Seed::setData()
{
    dataRecorder.pushGoalData(getNode());
}

void Seed::setFromRecorder()
{
    if (nodeRow != INVALID_ROW_OR_COL && nodeCol != INVALID_ROW_OR_COL)
    {
        data.setType(nodeRow, nodeCol, Node_available);
    }

    int nextGoal = dataRecorder.popGoalData();
    nodeRow = nextGoal / data.col();
    nodeCol = nextGoal % data.col();
    data.setType(nodeRow, nodeCol, Node_seed);
}










