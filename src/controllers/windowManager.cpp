#pragma once
#include "windowManager.h"
#include "imgui.h"
#include "../models/window.h"
#include <iostream>

void WindowManager::addWindow(IWindow* window) {
	windows.push_back(window);
}

void WindowManager::removeWindow(int index) {
	windows.erase(windows.begin() + index);
	std::string s = windows.at(index)->getName();
}

void WindowManager::renderAll() {
	for (int i = 0; i < windows.size(); i++) {
		IWindow* k = windows.at(i);

		bool windowOpen = true;
		k->render(windowOpen);
		if (!windowOpen) {
			k->onClose();
			windows.erase(windows.begin() + i--);
		}
	}
}