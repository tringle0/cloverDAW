#pragma once
#include <vector>
#include "../models/window.h"

//simple window manager that creates windows
class WindowManager {
public:
	void addWindow(IWindow* window);
	void removeWindow(int remove);
	void renderAll();
private:
	std::vector<IWindow*> windows;
};