#pragma once
#include "synthEditor.h"
#include "../models/synth.h"
#include "../models/layer.h"
#include "../models/window.h"
#include <imgui.h>

Note defaultNote = { 0, 1, 69 };

void LayerEditorWindow::update() {
	ImGui::Text(layer->name.c_str());

	//volume slider
	ImGui::SliderFloat("volume (%)", &layer->volume, 0.0f, 1.0f, "%.3f");
	
	
	//waveform dropdown
	const char* items[] = {"triangle", "sawtooth", "sine", "square", "noise"};
	if (ImGui::Combo("waveform", &selectedWaveForm, items, IM_ARRAYSIZE(items))) {
		layer->synth.waveform = (WaveForm)selectedWaveForm;
	}

	// play waveform
	ImGui::Button("play");
	bool isHeld = ImGui::IsItemActive();
	if (isHeld) {
		app->audio.play({ { &defaultNote, layer} });
	}

	//rename layer button
	ImGui::SameLine();
	if (ImGui::Button("rename") && song->layers.size() > 0) {
		ImGui::OpenPopup("rename", NULL);
	}

	//rename layer popup
	static char buffer[128] = "";
	if (ImGui::BeginPopupModal("rename")) {
		ImGui::InputText("name", buffer, sizeof(buffer));

		if (ImGui::Button("ok")) {
			layer->name = buffer;
			buffer[0] = '\0';
			ImGui::CloseCurrentPopup();
		}


		ImGui::EndPopup();
	}

	ImGui::Text("effects");
	if(ImGui::Button("add effect") ){
		ImGui::OpenPopup("addEffect", NULL);
	}

	const char* effects[] = { "detune"};
	if (ImGui::BeginPopupModal("addEffect")) {
		if (ImGui::Combo("effect", &selectedEffect, effects, IM_ARRAYSIZE(effects))) {

			ImGui::CloseCurrentPopup();
		}
	}
	for (int i = 0; i < layer->effects.size(); i++) {
		ImGui::PushID(i);

		if (ImGui::BeginChild("##panel", ImVec2(ImGui::GetContentRegionAvail().x, 80.0f), true)) {
			ImGui::Text("Panel %d", i);
		}
		ImGui::EndChild();

		ImGui::PopID();
	}
}
