#include "Dijkstra.h"

// Define constructor
Dijkstra::Dijkstra(int w, int h) {
	this->width = w;
	this->height = h;
}

// Define destructor
Dijkstra::~Dijkstra() {

}

// Returns the node that is closest to the start node
Node *Dijkstra::minDistance(vector<Node*> nodes) {
	Node *lowest = nodes.at(0);

	for (Node *node : nodes) {
		if (node->getDistance() < lowest->getDistance()) {
			lowest = node;
		}
	}

	return lowest;
}

// Updates the neighbours of each node, adds them to the openset, and sets the distance to the maximum
void Dijkstra::setup(Node **nodes, vector<Node*> *openSet) {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			nodes[y * width + x]->updateNeighbours(nodes);
			openSet->push_back(nodes[y * width + x]);
			nodes[y * width + x]->setDistance(INT_MAX);
		}
	}
}

// Display the path once it has been found
void Dijkstra::reconstructPath(map<Node*, Node*> comeFrom, Node *current) {
	while (!comeFrom.at(current)->isStart()) {
		current = comeFrom.at(current);
		current->makePath();
	}
}

// Run the algorithm
void Dijkstra::start(atomic_bool &running, Node **nodes, Node *start, Node *end) {
	// Stores all of the unvisited nodes in the graph
	vector<Node*> openSet;

	// Maps each node to the one before it in the path
	map<Node*, Node*> comeFrom;

	// Setup the grid
	setup(nodes, &openSet);
	start->setDistance(0);

	// Loop until there are no more nodes to visit
	while (!openSet.empty()) {
		// Get the closest node to the start
		Node *current = minDistance(openSet);

		// Remove the current node from the openset
		openSet.erase(std::remove(openSet.begin(), openSet.end(), current), openSet.end());

		// If the current node is end node, the shortest path has been found
		if (current == end) {
			reconstructPath(comeFrom, current);
			end->makeEnd();
			running = false;
			return;
		}

		// Update the distances for each of the nodes neighbours
		for (Node *neighbour : current->getNeighbours()) {
			// Calculate a possible shortest distance
			int tempDistance = current->getDistance() + 1;

			// If this is the shortest distance, update the neighbour
			if (tempDistance < neighbour->getDistance()) {
				neighbour->setDistance(tempDistance);
				comeFrom.insert({ neighbour, current });
				neighbour->makeOpen();
			}
		}

		// Sleep so the algorithm can be updated on scren
		wxThread::Sleep(10);

		// If the current node isn't the start node, close it so the user can see it has been visited
		if (!current->isStart()) {
			current->makeClosed();
		}
	}

	running = false;
}