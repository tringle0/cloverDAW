#pragma once
#include <string>

//interface for a displayed window
class IWindow {
public:
	//whether the window is open
	bool open;

	//id of the window, needs to be unique for each window
	std::string name;

	//functions
	virtual void onOpen() {};
	virtual void update() {};

};