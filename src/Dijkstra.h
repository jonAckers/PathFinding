#pragma once

#include <map>
#include <vector>

#include "Node.h"

class Dijkstra {
public:
	// Declare constructor and destructor
	Dijkstra(int w, int h);
	~Dijkstra();

	// Declare functions
	Node *minDistance(vector<Node*> nodes);
	void setup(Node **nodes, vector<Node*> *openSet);
	void reconstructPath(map<Node*, Node*> comeFrom, Node *current);
	void start(atomic_bool &running, Node **nodes, Node *start, Node *end);

private:
	// Declare attributes
	int width;
	int height;
};

