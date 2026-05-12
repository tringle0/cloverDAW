#pragma once
#include "windowManager.h"
#include "imgui.h"

void WindowManager::addWindow(IWindow* window) {
	windows.push_back(window);
}

void WindowManager::removeWindow(int remove) {
	windows.erase(windows.begin() + remove);
}

void WindowManager::renderAll() {
	for (int i = 0; i < windows.size(); i++) {
		IWindow* k = windows.at(i);
		ImGui::Begin(k->name.c_str(), k->closable? &k->visible:NULL);

		k->update();

		ImGui::End();
		if (!k->visible) removeWindow(i);
	}
}