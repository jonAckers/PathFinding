#include "Maze.h"

// Define constuctor
Maze::Maze(int w, int h) {
	this->width = w;
	this->height = h;
}

// Define destructor
Maze::~Maze() {
}

// Fill the grid in a lattice pattern so some walls can be removed creating a maze
void Maze::fillGrid(Node **nodes) {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			// If the node is in an even row or column, make it a barrier
			if (x % 2 == 0 || y % 2 == 0) {
				nodes[y * width + x]->makeBarrier();
			}
			// Otherwise, reset it manually update the neighbours
			// (Update needs to be manual because these are barriers that are being added as neighbours which would normally be ignored)
			else {
				nodes[y * width + x]->reset();
				toVisit.push_back(nodes[y * width + x]);
				nodes[y * width + x]->setVisited(false);
				
				// Right
				if (x < width - 2 && !nodes[y * width + (x + 2)]->isBarrier()) {
					nodes[y * width + x]->addNeighbour(nodes[y * width + (x + 2)]);
					vector<Node*> edge{ nodes[y * width + x], nodes[y * width + (x + 2)] };
					walls.insert({ edge, nodes[y * width + (x + 1)] });
				}

				// Left
				if (x > 1 && !nodes[y * width + (x - 2)]->isBarrier()) {
					nodes[y * width + x]->addNeighbour(nodes[y * width + (x - 2)]);
					vector<Node*> edge{ nodes[y * width + x], nodes[y * width + (x - 2)] };
					walls.insert({ edge, nodes[y * width + (x - 1)] });
				}

				// Up
				if (y < height - 2 && !nodes[(y + 2) * width + x]->isBarrier()) {
					nodes[y * width + x]->addNeighbour(nodes[(y + 2) * width + x]);
					vector<Node*> edge{ nodes[y * width + x], nodes[(y + 2) * width + x] };
					walls.insert({ edge, nodes[(y + 1) * width + x] });
				}

				// Down
				if (y > 1 && !nodes[(y - 2) * width + x]->isBarrier()) {
					nodes[y * width + x]->addNeighbour(nodes[(y - 2) * width + x]);
					vector<Node*> edge{ nodes[y * width + x], nodes[(y - 2) * width + x] };
					walls.insert({ edge, nodes[(y - 1) * width + x] });
				}
			}
		}
	}
}

// Choose a random neighbour so it can be deleted
Node *Maze::getRandomNeighbour(Node *node) {
	vector<Node*> neighbours = node->getNeighbours();
	int i = rand() % neighbours.size();

	return neighbours[i];
}

// Run the algorithm
void Maze::start(Node **nodes) {
	// Setup the grid
	fillGrid(nodes);

	// Choose a randpm node to start at
	int i = rand() % toVisit.size();
	Node *current = toVisit[i];

	// Count the number of nodes visited
	int visited = 0;

	// Loop until every node is visited
	while (visited < toVisit.size()) {
		// Get a random neighbour
		Node *neighbour = getRandomNeighbour(current);

		// If the neighbour hasn't been visited, add it as an edge and reset the wall
		if (!neighbour->getVisited()) {
			vector<Node*> edge = { current, neighbour };
			walls.at(edge)->reset();

			neighbour->setVisited(true);
			visited++;
		}

		current = neighbour;
	}
}