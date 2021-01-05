#include "seed.h"
#include "screen_data.h"
#include "data_recorder.h"
#include <cstdlib>
#include <ctime>
#include <QDebug>


void Seed::set(bool initial)
{
    if (!initial)
    {
        data.setType(nodeRow, nodeCol, NODE_SNAKE_HEAD);
    }

    if (modeSelection.getMode() == MODE_REPLAY)
    {
        setFromRecorder();
        return;
    }

	srand(time(NULL));
	int row_bound = data.getRow();
	int col_bound = data.getCol();

	do
	{
        nodeRow = rand() % row_bound;
        nodeCol = rand() % col_bound;
    } while(data.getType(nodeRow, nodeCol) != NODE_AVAILABLE);
	

    data.setType(nodeRow, nodeCol, NODE_SEED);
    setData();
}

int Seed::getNode()
{
    return nodeRow * data.getCol() + nodeCol;
}

int Seed::getNodeRow()
{
    return nodeRow;
}

int Seed::getNodeCol()
{
    return nodeCol;
}

void Seed::setData()
{
    dataRecorder.pushGoalData(getNode());
}

void Seed::setFromRecorder()
{
    if (nodeRow != INVALID_ROW_OR_COL && nodeCol != INVALID_ROW_OR_COL)
    {
        data.setType(nodeRow, nodeCol, NODE_AVAILABLE);
    }

    int nextGoal = dataRecorder.popGoalData();
    nodeRow = nextGoal / data.getCol();
    nodeCol = nextGoal % data.getCol();
    data.setType(nodeRow, nodeCol, NODE_SEED);
}










