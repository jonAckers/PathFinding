#include "Main.h"
#include "AStar.h"
#include "Dijkstra.h"
#include "BFS.h"
#include "Maze.h"

// Create event table
wxBEGIN_EVENT_TABLE(Main, wxFrame)
	EVT_CLOSE(onClose)		// Bind close function to close event
wxEND_EVENT_TABLE()

// Create constructor and set up frame
Main::Main() : wxFrame(nullptr, wxID_ANY, "PathFinding", wxPoint(50, 50), wxSize(1086, 809)) {
	// Prevent the user resizing the window
	SetMinSize(GetSize());
	SetMaxSize(GetSize());

	// Set up the control panel for the buttons to be added to
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	controlPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(300, 800));
	controlPanel->SetBackgroundColour(wxColor(255, 255, 255));
	sizer->Add(controlPanel);

	// Add the dropdown list for choosing the algorithm
	wxString algorithms[3] = { wxT("A*"), wxT("Dijkstra"), wxT("Breadth First Search") };
	algoSelector = new wxChoice(controlPanel, wxID_ANY, wxPoint(75, 200), wxSize(150, 100), 3, algorithms);

	// Create the start algorithm button and bind the relevant function
	startButton = new wxButton(controlPanel, wxID_ANY, "Find Path!", wxPoint(75, 300), wxSize(150, 50));
	startButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Main::onStartClicked, this);

	// Create the generate maze button and bind the relevant function
	mazeButton = new wxButton(controlPanel, wxID_ANY, "Generate Maze", wxPoint(75, 400), wxSize(150, 50));
	mazeButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Main::onMazeClicked, this);

	// Create the reset path button and bind the relevant function
	resetPathButton = new wxButton(controlPanel, wxID_ANY, "Reset Path", wxPoint(75, 500), wxSize(150, 50));
	resetPathButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Main::onResetPathClicked, this);

	// Create the reset all button and bind the relevant function
	resetAllButton = new wxButton(controlPanel, wxID_ANY, "Reset All", wxPoint(75, 600), wxSize(150, 50));
	resetAllButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Main::onResetAllClicked, this);

	// Create the array of nodes
	nodes = new Node * [width * height];
	wxGridSizer *grid = new wxGridSizer(width, height, 0, 0);

	// Creat the nodes to populate the array and bind the relevant functions
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			int id = y * width + x;
			nodes[id] = new Node(width, height, this, 10000 + id);
			nodes[id]->setPosition(x, y);
			nodes[id]->reset();
			grid->Add(nodes[id], 1, wxEXPAND | wxALL);

			nodes[id]->Connect(wxEVT_ENTER_WINDOW, (wxObjectEventFunction)&Main::onNodeEnter, NULL, this);
			nodes[id]->Connect(wxEVT_LEFT_DOWN, (wxObjectEventFunction)&Main::onNodeLeftClick, NULL, this);
			nodes[id]->Connect(wxEVT_RIGHT_DOWN, (wxObjectEventFunction)&Main::onNodeRightClick, NULL, this);
			nodes[id]->Connect(wxEVT_LEFT_UP, (wxObjectEventFunction)&Main::onLeftUp, NULL, this);
			nodes[id]->Connect(wxEVT_RIGHT_UP, (wxObjectEventFunction)&Main::onRightUp, NULL, this);

		}
	}

	// Add the components to the frame
	sizer->Add(grid, 1, wxEXPAND | wxALL);
	this->SetSizer(sizer);
	grid->Layout();
}

// Destroy objects when the application closes
Main::~Main() {
	// Wait for the thread to terminate
	if (algorithmThread.joinable()) {
		algorithmThread.join();
	}

	// Dereference pointers
	delete[]nodes;
	delete[]controlPanel;
}

// Remove the generated path by resetting all cells
void Main::resetPath() {
	if (algorithmRunning) {
		return;
	}

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			int id = y * width + x;
			if (nodes[id]->isStart() || nodes[id]->isEnd()) {
				nodes[id]->resetStartEnd();
			}
			else if (!nodes[id]->isBarrier()) {
				nodes[id]->reset();
			}
		}
	}
}

// Runs when the mouse is over a node
void Main::onNodeEnter(wxMouseEvent &evt) {
	if (algorithmRunning) {
		return;
	}

	// Get the node the mouse is over
	int x = (evt.GetId() - 10000) % width;
	int y = (evt.GetId() - 10000) / width;
	int id = y * width + x;

	// Populate node
	if (leftDown && nodes[id]->isEmpty()) {
		// If there is no start node, make the current node the start
		if (!startSet) {
			nodes[id]->makeStart();
			start = nodes[id];
			startSet = true;
		}
		// If there is no end node, make the current node the end
		else if (!endSet) {
			nodes[id]->makeEnd();
			end = nodes[id];
			endSet = true;
		}
		// Otherwise, make it a barrier
		else {
			nodes[id]->makeBarrier();
		}
	}
	// Reset the node
	else if (rightDown) {
		if (nodes[id]->isStart()) {
			start = nullptr;
			startSet = false;
		}

		if (nodes[id]->isEnd()) {
			end = nullptr;
			endSet = false;
		}

		nodes[id]->reset();
	}

	evt.Skip();
}

// Runs when a node is left clicked on
void Main::onNodeLeftClick(wxMouseEvent &evt) {
	if (algorithmRunning) {
		return;
	}

	leftDown = true;

	// Get node being clicked on
	int x = (evt.GetId() - 10000) % width;
	int y = (evt.GetId() - 10000) / width;
	int id = y * width + x;

	// Check there's nothing already in the cell
	if (nodes[id]->isEmpty()) {
		// If there is no start node, make the current node the start
		if (!startSet) {
			nodes[id]->makeStart();
			start = nodes[id];
			startSet = true;
		}
		// If there is no end node, make the current node the end
		else if (!endSet) {
			nodes[id]->makeEnd();
			end = nodes[id];
			endSet = true;
		}
		// Otherwise, make it a barrier
		else {
			nodes[id]->makeBarrier();
		}
	}

	evt.Skip();
}

// Runs when a node is right clicked on
void Main::onNodeRightClick(wxMouseEvent &evt) {
	if (algorithmRunning) {
		return;
	}

	rightDown = true;

	// Get the node that's clicked
	int x = (evt.GetId() - 10000) % width;
	int y = (evt.GetId() - 10000) / width;
	int id = y * width + x;

	// Reset the node
	if (nodes[id]->isStart()) {
		start = nullptr;
		startSet = false;
	}

	if (nodes[id]->isEnd()) {
		end = nullptr;
		endSet = false;
	}

	nodes[id]->reset();
	
	evt.Skip();
}

// Runs if the left click is lifted
void Main::onLeftUp(wxMouseEvent &evt) {
	leftDown = false;
	evt.Skip();
}

// Runs if the right click is lifted
void Main::onRightUp(wxMouseEvent &evt) {
	rightDown = false;
	evt.Skip();
}

// Runs when the start generating path button is clicked
void Main::onStartClicked(wxCommandEvent &evt) {
	if (algorithmRunning) {
		return;
	}

	// Wait for any running threads to finish
	if (algorithmThread.joinable()) {
		algorithmThread.join();
	}

	// Get the selected algorithm
	std::string algoString = std::string(algoSelector->GetStringSelection().mb_str());

	// Only run if there is a start and end node
	if (startSet && endSet) {
		// If the user hasn't picked an algorithm, tell them
		if (algoString.compare("") == 0) {
			wxMessageBox("Select an algorithm...");
		}
		else {
			// Reset the current path
			resetPath();
			algorithmRunning = true;

			// If A* is selected, run the A* algorithm
			if (algoString.compare("A*") == 0) {
				AStar *algorithm = new AStar(width, height);
				algorithmThread = std::thread(&AStar::start, *algorithm, std::ref(algorithmRunning), std::ref(nodes), std::ref(start), std::ref(end));
			}
			// If Dijkstra is selected, run Dijkstra's algorithm
			else if (algoString.compare("Dijkstra") == 0) {
				Dijkstra *algorithm = new Dijkstra(width, height);
				algorithmThread = std::thread(&Dijkstra::start, *algorithm, std::ref(algorithmRunning), std::ref(nodes), std::ref(start), std::ref(end));
			}
			// If Breadth First Search is selected, run Breadth First Search algorithm
			else if (algoString.compare("Breadth First Search") == 0) {
				BFS *algorithm = new BFS(width, height);
				algorithmThread = std::thread(&BFS::start, *algorithm, std::ref(algorithmRunning), std::ref(nodes), std::ref(start), std::ref(end));
			}
		}
	}
	// Tell the user they haven't added a start or end node
	else {
		wxMessageBox("Add start/end node...");
	}

	evt.Skip();
}

// Runs when the generate maze button is clicked
void Main::onMazeClicked(wxCommandEvent &evt) {
	if (algorithmRunning) {
		return;
	}

	// Reset the grid
	startSet = false;
	endSet = false;

	// Run the algorithm
	Maze *generator = new Maze(width, height);
	generator->start(nodes);
}

// Runs when the reset path button is clicked
void Main::onResetPathClicked(wxCommandEvent &evt) {
	if (algorithmRunning) {
		return;
	}

	// Reset the path
	resetPath();

	evt.Skip();
}

// Runs when the reset all button is clicked
void Main::onResetAllClicked(wxCommandEvent &evt) {
	if (algorithmRunning) {
		return;
	}

	// Reset the grid
	startSet = false;
	endSet = false;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			nodes[y * width + x]->reset();
		}
	}

	evt.Skip();
}

// Runs when the application is closed
void Main::onClose(wxCloseEvent &evt) {
	this->Show(false);
	this->Destroy();
}