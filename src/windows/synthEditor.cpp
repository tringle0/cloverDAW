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
	if (ImGui::IsItemDeactivated()) {

	}

	//rename layer button
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


}
