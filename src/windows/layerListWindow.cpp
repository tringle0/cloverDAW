#pragma once
#include "layerListWindow.h"
#include "imgui.h"

LayerListWindow::LayerListWindow(std::string name) {
	this->name = name;
}

void LayerListWindow::update() {
	ImGui::Text("layer list");
}