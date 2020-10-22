#include "AStar.h"

// Define the constructor
AStar::AStar(int w, int h) {

	this->width = w;
	this->height = h;
}

// Define destructor
AStar::~AStar() {
}

// Returns a heuristic for the distance between nodes
// In this case it is the L distance (i.e. difference in x + difference in y)
int AStar::heuristic(Node *n1, Node *n2) {
	return abs(n1->getX() - n2->getX()) + abs(n1->getY() - n2->getY());
}

// Returns the lowest F score in the open set of nodes
Node *AStar::getLowestFScore(vector<Node*> openSet) {
	Node *lowest = openSet.at(0);

	for (Node *node : openSet) {
		if (node->getFScore() < lowest->getFScore()) {
			lowest = node;
		}
	}

	return lowest;
}

// Updates the neighbours of all the nodes
void AStar::setup(Node **nodes) {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			nodes[y * width + x]->updateNeighbours(nodes);
		}
	}
}

// Display the path once it has been found
void AStar::reconstructPath(map<Node*, Node*> comeFrom, Node *current) {
	while (!comeFrom.at(current)->isStart()) {
		current = comeFrom.at(current);
		current->makePath();
	}
}

// Run the algorithm
void AStar::start(atomic_bool &running, Node **nodes, Node *start, Node *end) {
	// Setup the grid
	setup(nodes);

	// Declare the openset (i.e. the nodes that have been reached)
	vector<Node*> openSet;
	openSet.push_back(start);

	// Maps each node to the one before it in the path
	map<Node*, Node*> comeFrom;

	// Sets the parameters for the start node
	start->setGScore(0);
	start->setFScore(heuristic(start, end));

	while (!openSet.empty()) {
		// Get the closest node to the end
		Node *current = getLowestFScore(openSet);

		// If the current node is the end, the shortest path has been found
		if (current == end) {
			reconstructPath(comeFrom, current);
			end->makeEnd();
			running = false;
			return;
		}

		// Remove the current node from the openset
		openSet.erase(std::remove(openSet.begin(), openSet.end(), current), openSet.end());

		// Loop through each of the current nodes neighbours
		for (Node *neighbour : current->getNeighbours()) {
			// Work out the distance from the start if this node is in the path
			int tempGScore = current->getGScore() + 1;

			// If that is less than the current G score update the node and possible path
			if (tempGScore < neighbour->getGScore()) {
				comeFrom.insert({ neighbour, current });
				neighbour->setGScore(tempGScore);
				neighbour->setFScore(tempGScore + heuristic(neighbour, end));

				// Add the neighbour to the openset if it isn't there already
				if (std::find(openSet.begin(), openSet.end(), neighbour) == openSet.end()) {
					openSet.push_back(neighbour);
					neighbour->makeOpen();
				}
			}
		}

		// Sleep so the algorithm can update on screen
		wxThread::Sleep(10);

		// Close the current node if it isn't the start
		if (!current->isStart()) {
			current->makeClosed();
		}
	}	
	running = false;
}