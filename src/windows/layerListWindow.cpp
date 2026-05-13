#pragma once
#include "layerListWindow.h"
#include "synthEditor.h"
#include "imgui.h"
#include <string>

LayerListWindow::LayerListWindow(std::string name, Song* song, WindowManager* wm) {
	this->name = name;
	this->song = song;
	this->wm = wm;
	closable = true;
}

void LayerListWindow::update() {
	ImGui::Text("layer list");

	//get convert layer names sto char arrays
	std::vector<const char*> items;
	for (Layer* l : song->layers) {
		items.push_back(l->name.c_str());
	}

	//the number to put next to a new layer
	static int nextLayerName = 0;

	if (ImGui::ListBox("layer list", &song->selectedLayerIndex, items.data(), items.size(), 4)) {
		// Code to run when selection changes
	}

	//add layer button
	if (ImGui::Button("add")) {
		song->addLayer(new Layer("layer " + std::to_string(nextLayerName)));
		nextLayerName++;
	}

	//remove layer button
	ImGui::SameLine();
	if (ImGui::Button("delete")) {
		song->removeLayer();
		nextLayerName--;
	}

	//rename layer button, change to layer properties in future
	ImGui::SameLine();
	if (ImGui::Button("rename") && song->layers.size() > 0) {
		ImGui::OpenPopup("rename", NULL);
	}

	//rename layer popup
	static char buffer[128] = "";
	if (ImGui::BeginPopupModal("rename")) {
		ImGui::InputText("name", buffer, sizeof(buffer));

		if (ImGui::Button("ok")) {
			song->getLayer()->name = buffer;
			buffer[0] = '\0';
			ImGui::CloseCurrentPopup();
		}


		ImGui::EndPopup();
	}

	if (ImGui::Button("edit synth")) {
		wm->addWindow(new SynthEditor(song->getLayer()->name, & song->getLayer()->synth));
	}

}