#pragma once

#include <wx/wx.h>
#include <thread>
#include <atomic>

#include "Node.h"

class Main : public wxFrame {
public:
	// Declare constructor and destructor
	Main();
	~Main();

	// Declare action functions
	void resetPath();
	void onNodeEnter(wxMouseEvent &evt);
	void onNodeLeftClick(wxMouseEvent &evt);
	void onNodeRightClick(wxMouseEvent &evt);
	void onLeftUp(wxMouseEvent &evt);
	void onRightUp(wxMouseEvent &evt);
	void onMazeClicked(wxCommandEvent &evt);
	void onStartClicked(wxCommandEvent &evt);
	void onResetPathClicked(wxCommandEvent &evt);
	void onResetAllClicked(wxCommandEvent &evt);
	void onClose(wxCloseEvent &evt);

private:
	// Declare attributes
	int width = 55;		// Width of grid in number of cells
	int height = 55;	// Height of grid in number of cells

	atomic_bool algorithmRunning = false;	// Stores if path is being generated
	std::thread algorithmThread;			// The thread that the algorithm will be executed in

	bool startSet = false;			// Stores if the start node has been placed
	bool endSet = false;			// Stores if the end node has been placed
	bool leftDown = false;			// Stores if the left mouse button is down
	bool rightDown = false;			// Stores if the right mouse button is down

	Node **nodes;					// Stores the grid of nodes
	Node *start = nullptr;			// Stores the start node
	Node *end = nullptr;			// Stores the end node

	wxPanel *controlPanel;			// The panel that the control buttons are in
	wxChoice *algoSelector;			// The dropdown list of algorithms
	wxString algorithms[3];			// The list of algorithms for the dropdown list
	wxButton *startButton;			// The button that starts the path generation
	wxButton *mazeButton;			// The button that starts the maze generation
	wxButton *resetPathButton;		// The button that resets the path but leaves the nodes placed
	wxButton *resetAllButton;		// The button that resets the whole grid
	
	wxDECLARE_EVENT_TABLE();
};
