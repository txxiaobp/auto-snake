#ifndef SEED_H
#define SEED_H

class ScreenData;

class Seed
{
private:
	ScreenData& data;
    int nodeRow;
    int nodeCol;

public:
    Seed(ScreenData& data) : data(data) { set(true); }
    void set(bool initial = false);
    int getNode();
};

#endif
