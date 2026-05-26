#include "layerList.h"
#include <vector>
#include <string>
#include <imgui.h>
#include "../app.h"
#include "../models/layer.h"
#include "../models/uniqueCounter.h"
#include "../windows/synthEditor.h"

UniqueCounter LayerListWindow::nextLayerDefaultName;

LayerListWindow::LayerListWindow(App* app) : IWindow("layer list", app, ImVec2(250, 450), false, false) {}

void LayerListWindow::update() {
	//get convert layer names to char arrays
	std::vector<const char*> items;
	for (Layer* l : song->layers) {
		items.push_back(l->name.c_str());
	}

	//layer list
	ImGui::SetNextItemWidth(225);
	ImGui::ListBox("##layer list", &app->sessionData.selectedLayerIndex, items.data(), items.size(), 20);

	//add layer button
	if (ImGui::Button("add")) {
		song->layers.push_back(new Layer("layer " + std::to_string(nextLayerDefaultName.produceUnique())));
	}

	//remove layer button
	ImGui::SameLine();
	if (song->layers.size() > 0 && ImGui::Button("delete")) {
		song->layers.erase(song->layers.begin() + app->sessionData.selectedLayerIndex);
		nextLayerDefaultName.remove(app->sessionData.selectedLayerIndex);
		if(app->sessionData.selectedLayerIndex > 0) app->sessionData.selectedLayerIndex--;
	}



	//edit layer
	ImGui::SameLine();
	if (song->layers.size() > 0 && ImGui::Button("edit")) {
		windowManager->addWindow(new LayerEditorWindow(app->sessionData.selectedLayerIndex, app ));
	}

	
}
