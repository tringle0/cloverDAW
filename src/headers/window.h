#pragma once;
#include <string>

//interface for a displayed window
class IWindow {
public:
	std::string name;
	virtual void onOpen() {};
	virtual void update() = 0;
private:
	void render();
};