#pragma once

#include <wx/wx.h>
#include <vector>

using namespace std;

class Node : public wxPanel {
public:
	// Declare constructor and destructor
	Node(int width, int height, wxWindow *win, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString &name = wxPanelNameStr);
	~Node();

	// Declare getter functions
	bool isClosed();
	bool isOpen();
	bool isBarrier();
	bool isStart();
	bool isEnd();
	bool isEmpty();
	vector<Node*> getNeighbours();
	int getX();
	int getY();

	// Declare setter functions
	void reset();
	void resetStartEnd();
	void makeClosed();
	void makeOpen();
	void makeBarrier();
	void makeStart();
	void makeEnd();
	void makePath();
	void updateNeighbours(Node **nodes);
	void addNeighbour(Node *node);
	void draw();

	// Declare functions used by pathfinding algorithms
	void setPosition(int x, int y);
	int getFScore();
	void setFScore(int f);
	int getGScore();
	void setGScore(int g);
	int getDistance();
	void setDistance(int d);
	bool getVisited();
	void setVisited(bool d);

	// Override operator so the nodes can be sorted
	bool operator < (const Node &other) const {
		return distance < other.distance;
	}

private:
	// Define colours for each state
	const wxColor RED = wxColor(255, 0, 0);
	const wxColor GREEN = wxColor(0, 255, 0);
	const wxColor WHITE = wxColor(255, 255, 255);
	const wxColor BLACK = wxColor(0, 0, 0);
	const wxColor ORANGE = wxColor(255, 165, 0);
	const wxColor PURPLE = wxColor(128, 0, 128);
	const wxColor TURQUOISE = wxColor(64, 224, 208);

	// Declare attributes
	int x;
	int y;
	int width;
	int height;
	int fScore = INT_MAX;
	int gScore = INT_MAX;
	int distance = INT_MAX;
	bool visited = false;
	vector <Node*> neighbours;
};