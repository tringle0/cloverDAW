#pragma once
#include <string>
#include <imgui.h>
#include "uniqueCounter.h"
#include "../app.h"

//forward declaration to avoid circular declarations
class WindowManager;
class Song;
class App;

//interface for a displayed window
class IWindow {
private:
	//generates unique ids for each window
	static UniqueCounter uc;

protected:
	//id of the window
	std::string name = "";
	//unique identifier for duplicate windows
	int id = 0;

	//display options
	bool closable = true;
	bool scalable = true;
	ImVec2 size = ImVec2(480,360);

	//references
	App* app = nullptr;
	WindowManager* windowManager = nullptr;
	Song* song = nullptr;

	//code to put actual contents of the window
	virtual void update() {};

public:
	//constructor
	IWindow(std::string name, App* app, ImVec2 size = ImVec2(480, 360), bool closable = true, bool scalable = true);

	//render function: call to render the window: returns true if closed
	//this should only be called by windowManager
	void render(bool &windowClosed);

	virtual void onClose();

	std::string getName() { return name; };
};