#ifndef NODE_H
#define NODE_H

typedef enum
{
    Node_undefined = 0,
	Node_available,
	Node_snake_body,
	Node_snake_head,
	Node_wall,
	Node_seed
} Node_t;

class Snake;

class Node
{
private:
	int row;
	int col;
	Node_t node_type;	

public:
	Node(int row = 0, int col = 0, Node_t node_type = Node_undefined);
	Node_t get_type();
	void set_type(Node_t node_type);
	void set_row(int row);
	void set_col(int col);
	int get_row();
	int get_col();
	bool operator==(const Node& node);
	bool operator!=(const Node& node);
	
	friend class Snake;
};

#endif
