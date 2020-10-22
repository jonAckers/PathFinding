#pragma once

#include <wx/wx.h>

#include "Main.h"

class App : public wxApp {
public:
	// Declare constructor and destructor
	App();
	~App();

	// Override initialise function
	virtual bool OnInit();

private:
	// Define frame
	Main *frame = nullptr;
};