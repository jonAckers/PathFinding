#pragma once

#include <queue>
#include <map>

#include "Node.h"

class BFS {
public:
	// Declare the constructor and destructor
	BFS(int w, int h);
	~BFS();

	// Declare functions
	void setup(Node **nodes);
	void reconstructPath(map<Node*, Node*> comeFrom, Node *current);
	void start(atomic_bool &running, Node **nodes, Node *start, Node *end);

private:
	// Declare attributes
	int width;
	int height;
};
