#pragma once
#include <vector>
#include "headers/window.h"

//simple window manager that creates windows
class WindowManager {
public:
	void addWindow(IWindow* window);
	void renderAll();
private:
	std::vector<IWindow*> windows;
};