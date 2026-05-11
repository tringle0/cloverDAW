#pragma once
#include "windowManager.h"
#include "imgui.h"

void WindowManager::addWindow(IWindow window) {
	windows.push_back(window);
}

void WindowManager::renderAll() {
	for (auto k : windows) {
		ImGui::Begin(k.name)
		k.update();
	}
}