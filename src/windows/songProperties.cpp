#include "songProperties.h"
#include "imgui_stdlib.h"
#include <string>

void SongPropertiesWindow::update(){
	ImGui::InputText("Title", &song->title);
	ImGui::InputText("Artist", &song->artist);
	ImGui::InputFloat("BPM", &song->bpm);
}