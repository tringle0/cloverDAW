#pragma once
#include <vector>
#include "headers/window.h"

class WindowManager {
public:
	void addWindow(IWindow* window);
	void renderAll();
private:
	std::vector<IWindow*> windows;
};