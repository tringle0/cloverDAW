#include "synthEditor.h"

#include <imgui.h>

#include "../app.h"
#include "../models/synth.h"
#include "../models/layer.h"
#include "../models/effects/detune.h"
#include "../models/effects/eq.h"
#include "../models/song.h"
#include "../models/window.h"


Note defaultNote = { 0, 1, 69 };

LayerEditorWindow::LayerEditorWindow(int layerIndex, App* app) : IWindow("synth editor", app, ImVec2(360, 240), true, false) {
	layer = song->layers.at(layerIndex);
	selectedWaveForm = layer->synth.waveform;
}

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

	const char* effects[] = { "detune", "equalizer"};

	if (ImGui::BeginPopupModal("addEffect")) {
		ImGui::Combo("effect", &selectedEffect, effects, IM_ARRAYSIZE(effects));
		if(ImGui::Button("add effect") ){
			switch (selectedEffect) {
			case 0: //detune
				layer->effects.push_back(new Detune(0));
				break;
			case 1: //eq
				layer->effects.push_back(new EQ());
				break;
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	for (int i = 0; i < layer->effects.size(); i++) {
		ImGui::PushID(i);

		if (ImGui::BeginChild("##panel", ImVec2(ImGui::GetContentRegionAvail().x, 100.f), true)) {
			layer->effects[i]->render();
			if (ImGui::Button("remove")) {
				layer->effects.erase(layer->effects.begin() + i--);
			}
		}
		ImGui::EndChild();

		ImGui::PopID();
	}
}
