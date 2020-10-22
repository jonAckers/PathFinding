#pragma once

#include <stdlib.h> 
#include <map>

#include "Node.h"

class Maze {
public:
	// Declare constructor and destructor
	Maze(int w, int h);
	~Maze();

	// Declare functions
	void fillGrid(Node **nodes);
	Node *getRandomNeighbour(Node *node);
	void start(Node **nodes);

private:
	// Declare attributes
	int width;
	int height;
	vector<Node*> toVisit;
	map<vector<Node*>, Node*> walls;
};