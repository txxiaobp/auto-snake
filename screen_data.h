#ifndef SCREEN_DATA_H
#define SCREEN_DATA_H

#include <vector>
#include <map>

class Snake;
class Seed;

typedef enum
{
    NODE_AVAILABLE,
    NODE_SNAKE_BODY,
    NODE_SNAKE_HEAD,
    NODE_WALL,
    NODE_SEED,
    NODE_OBSTACLE
} Node_t;

class ScreenData
{	
public:
	ScreenData(int row = 10, int col = 10);
    void reset(int newRow, int newCol);
    int getRow();
    int getCol();
    int getTotalSize() { return getRow() * getCol(); }
    Node_t getType(int row, int col);
    Node_t getType(int node);
    void setType(int node, Node_t node_type);
	void setType(int row, int col, Node_t node_type);
    bool inArea(int row, int col);
    int getNode(int nodeRow, int nodeCol);
    std::pair<int,int> getNodePair(int node);
    std::vector<int> aroundNodes(int node);
	
	friend class Snake;
	friend class Seed;

private:
    std::vector<std::vector<Node_t>> data;

private:
    void set_wall();
};

#endif
