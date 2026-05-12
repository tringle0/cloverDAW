#pragma once
#include "layerListWindow.h"
#include "imgui.h"

LayerListWindow::LayerListWindow(std::string name) {
	this->name = name;
}

void LayerListWindow::update() {
	ImGui::Text("layer list");
	const char* items[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango" };
	static int selected_item = 0;

	if (ImGui::ListBox("My List", &selected_item, items, IM_ARRAYSIZE(items), 4)) {
		// Code to run when selection changes
	}
}