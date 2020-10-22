#include "Node.h"

// Define constructor and call superconstructor
Node::Node(int width, int height, wxWindow *win, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
	: wxPanel(win, id, pos, size, style, name) {
	this->width = width;
	this->height = height;
}

// Define destructor
Node::~Node() {
}

// Returns if the node is closed
// Used when the node has been visited by a shortest path algorithm
bool Node::isClosed() {
	return this->GetBackgroundColour() == RED;
}

// Returns if the node is open
// Used when a node has beeen registered by a shortest path algorithm but hasn't yet been visited
bool Node::isOpen() {
	return this->GetBackgroundColour() == GREEN;
}

// Returns if the node is a barrier
bool Node::isBarrier() {
	return this->GetBackgroundColour() == BLACK;
}

// Returns if the node is the start node
bool Node::isStart() {
	return this->GetBackgroundColour() == TURQUOISE;
}

// Returns if the node is the end node
bool Node::isEnd() {
	return this->GetBackgroundColour() == ORANGE;
}

// Return if the node is empty
bool Node::isEmpty() {
	return this->GetBackgroundColour() == WHITE;
}

// Returns the neighbours of the node
vector<Node*> Node::getNeighbours() {
	return this->neighbours;
}

// Returns the x coordinate of the node in the grid
int Node::getX() {
	return this->x;
}

// Returns the y coordinate of the node in the grid
int Node::getY() {
	return this->y;
}

// Resets the node
void Node::reset() {
	// If the node is on the edge of the grid, make it a barrier
	if (this->x == 0 || this->x == this->width - 1
		|| this->y == 0 || this->y == this->height - 1) {
		this->makeBarrier();
	}
	// Otherwise, make it empty
	else {
		this->SetBackgroundColour(WHITE);

		// Reset values for shortest path algorithms
		this->fScore = INT_MAX;
		this->gScore = INT_MAX;
		this->distance = INT_MAX;
		this->neighbours.clear();
		draw();
	}
}

// Resets the start and end node
// Used to reset the values without resetting the state of the node when the path is reset
void Node::resetStartEnd() {
	this->fScore = INT_MAX;
	this->gScore = INT_MAX;
	this->neighbours.clear();
}

// Make the node closed
// Used when the node has been visited by a shortest path algorithm
void Node::makeClosed() {
	this->SetBackgroundColour(RED);
	draw();
}

// Make the node open
// Used when a node has beeen registered by a shortest path algorithm but hasn't yet been visited
void Node::makeOpen() {
	this->SetBackgroundColour(GREEN);
	draw();
}

// Make the node an algorithm
void Node::makeBarrier() {
	this->SetBackgroundColour(BLACK);
	draw();
}

// Make the node a start node
void Node::makeStart() {
	this->SetBackgroundColour(TURQUOISE);
	draw();
}

// Make the node an end node
void Node::makeEnd() {
	this->SetBackgroundColour(ORANGE);
	draw();
}

// Make the node a path
void Node::makePath() {
	this->SetBackgroundColour(PURPLE);
	draw();
}

// Draw the node on the screen
// Used when the colour is changed
void Node::draw() {
	Refresh();
	Update();
}

// Get the neighbours of the node if they aren't barriers
void Node::updateNeighbours(Node **nodes) {

	// Right
	if (x < width - 1 && !nodes[y * width + (x + 1)]->isBarrier()) {
		neighbours.push_back(nodes[y * width + (x + 1)]);
	}

	// Left
	if (x > 0 && !nodes[y * width + (x - 1)]->isBarrier()) {
		neighbours.push_back(nodes[y * width + (x - 1)]);
	}

	// Up
	if (y < height - 1 && !nodes[(y + 1) * width + x]->isBarrier()) {
		neighbours.push_back(nodes[(y + 1) * width + x]);
	}

	// Down
	if (y > 0 && !nodes[(y - 1) * width + x]->isBarrier()) {
		neighbours.push_back(nodes[(y - 1) * width + x]);
	}
}

// Add a specific node to the list of neighbours
void Node::addNeighbour(Node *node) {
	neighbours.push_back(node);
}

// Set the position of the node
void Node::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
}

// Get the F Score
// Used by the A* algorithm to store a heuristic for the distance to the enfd
int Node::getFScore() {
	return this->fScore;
}

// Set the F Score
// Used by the A* algorithm to store a heuristic for the distance to the end
void Node::setFScore(int f) {
	fScore = f;
}

// Get the G Score
// Used by the A* algorithm to store the current distance from the start node
int Node::getGScore() {
	return this->gScore;
}

// Set the G Score
// Used by the A* algorithm to store the current distance from the start node
void Node::setGScore(int g) {
	gScore = g;
}

// Get the distance from the start node
// Used by Dijkstra's algorithm 
int Node::getDistance() {
	return this->distance;
}

// Set the distance from the start node
// Used by Dijkstra's algorithm 
void Node::setDistance(int d) {
	distance = d;
}

// Get whether the node has been visited
// Used by BFS and maze generation algorithm to stop nodes being repeated
bool Node::getVisited() {
	return this->visited;
}

// Set whether the node has been visited
// Used by BFS and maze generation algorithm to stop nodes being repeated
void Node::setVisited(bool d) {
	visited = d;
}