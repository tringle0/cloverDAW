#include "songProperties.h"

#include <imgui.h>
#include "imgui_stdlib.h"
#include <string>

#include "../models/song.h"
#include "../app.h"

SongPropertiesWindow::SongPropertiesWindow(App* app) : IWindow("song properties", app, ImVec2(240, 180), false, false) {}

void SongPropertiesWindow::update(){
	ImGui::InputText("Title", &song->title);
	ImGui::InputText("Artist", &song->artist);
	ImGui::InputFloat("BPM", &song->bpm);

	if(ImGui::Button("save") ){
		app->sm.saveSong();
	}
	ImGui::SameLine();
	if (ImGui::Button("load")) {
		ImGui::OpenPopup("load song");
		
	}

	if (ImGui::BeginPopupModal("load song")) {
		static std::string filePath;
		ImGui::InputText("file path", &filePath);
		if (ImGui::Button("load")) {
			app->sm.loadSong(filePath);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}