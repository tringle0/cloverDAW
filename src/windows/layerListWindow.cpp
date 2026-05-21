#pragma once
#include "layerListWindow.h"
#include "synthEditor.h"
#include "imgui.h"
#include <string>

UniqueCounter LayerListWindow::nextLayerDefaultName;

void LayerListWindow::update() {
	//get convert layer names to char arrays
	std::vector<const char*> items;
	for (Layer* l : song->layers) {
		items.push_back(l->name.c_str());
	}


	ImGui::SetNextItemWidth(225);
	if (ImGui::ListBox("##layer list", &selectedLayerIndex, items.data(), items.size(), 4)) {
		// Code to run when selection changes
	}

	//add layer button
	if (ImGui::Button("add")) {
		song->layers.push_back(new Layer("layer " + std::to_string(nextLayerDefaultName.produceUnique())));
	}

	//remove layer button
	ImGui::SameLine();
	if (song->layers.size() > 0 && ImGui::Button("delete")) {
		song->layers.erase(song->layers.begin() + selectedLayerIndex);
		nextLayerDefaultName.remove(selectedLayerIndex);
		if(selectedLayerIndex > 0) selectedLayerIndex--;
	}



	//edit layer
	ImGui::SameLine();
	if (song->layers.size() > 0 && ImGui::Button("edit")) {
		windowManager->addWindow(new LayerEditor(selectedLayerIndex, song, windowManager ));
	}

	
}
