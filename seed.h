#ifndef SEED_H
#define SEED_H

#include "mode_selection.h"


class ScreenData;
class DataRecorder;

const int INVALID_ROW_OR_COL = -1;

class Seed
{
public:
    Seed(ScreenData& data, DataRecorder& dataRecorder, ModeSelection& modeSelection)
        : data(data)
        , dataRecorder(dataRecorder)
        , nodeRow(INVALID_ROW_OR_COL)
        , nodeCol(INVALID_ROW_OR_COL)
        , modeSelection(modeSelection)
            { set(true); }

    void set(bool initial = false);
    int getNode();
    void setData();
    void setFromRecorder();

private:
    ScreenData& data;
    DataRecorder& dataRecorder;
    int nodeRow;
    int nodeCol;
    ModeSelection& modeSelection;
};

#endif
