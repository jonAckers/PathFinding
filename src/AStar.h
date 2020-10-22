#pragma once

#include <map>
#include <vector>

#include "Node.h"

class AStar {
public:
	// Declare constructor and destructor
	AStar(int w, int h);
	~AStar();

	// Declare functions
	int heuristic(Node *n1, Node *n2);
	Node *getLowestFScore(vector<Node*> openSet);
	void setup(Node **nodes);
	void reconstructPath(map<Node*, Node*> comeFrom, Node *current);
	void start(atomic_bool &running, Node **nodes, Node *start, Node *end);

private:
	// Declare attributes
	int width;
	int height;
};