#include "App.h"

wxIMPLEMENT_APP(App);

// Constructor
App::App() {
}

// Destructor
App::~App() {
}

// Create frame to begin application
bool App::OnInit() {
	
	frame = new Main();
	frame->Show();

	return true;
}