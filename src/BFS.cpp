#include "BFS.h"

// Define constructor
BFS::BFS(int w, int h) {
	this->width = w;
	this->height = h;
}

// Define destructor
BFS::~BFS() {
}

// Update the neighbour of each node, set the distance to the maximum, and make it unvisited
void BFS::setup(Node **nodes) {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			nodes[y * width + x]->updateNeighbours(nodes);
			nodes[y * width + x]->setDistance(INT_MAX);
			nodes[y * width + x]->setVisited(false);
		}
	}
}

// Display the path once it has been found
void BFS::reconstructPath(map<Node*, Node*> comeFrom, Node *current) {
	while (!comeFrom.at(current)->isStart()) {
		current = comeFrom.at(current);
		current->makePath();
	}
}

// Run the algorithm
void BFS::start(atomic_bool &running, Node **nodes, Node *start, Node *end) {
	// Setup the grid
	setup(nodes);

	// Create a queue of nodes to visit so they are visited in the order they are found
	queue<Node*> toVisit;

	// Maps each node to the one before it in the path 
	map<Node*, Node*> comeFrom;

	// Sets the parameters for the start node
	start->setVisited(true);
	toVisit.push(start);

	// Loop until every node has been visited
	while (!toVisit.empty()) {
		// Get the first node in the queue and remove it from the queue
		Node *current = toVisit.front();
		toVisit.pop();

		// If the current node is the end, the shortest path has been found
		if (current == end) {
			reconstructPath(comeFrom, current);
			end->makeEnd();
			running = false;
			return;
		}

		// Loop through each neighbour
		for (Node *neighbour : current->getNeighbours()) {
			// If neighbour hasn't been visited, visit it
			if (!neighbour->getVisited()) {
				neighbour->setVisited(true);
				toVisit.push(neighbour);
				comeFrom.insert({ neighbour, current });
				neighbour->makeOpen();
			}
		}

		// Sleep so the algorithm can be displayed on screen
		wxThread::Sleep(10);

		// If the current node is not the start node, close it
		if (!current->isStart()) {
			current->makeClosed();
		}
	}

	running = false;
	return;
}