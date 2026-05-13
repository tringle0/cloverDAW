#pragma once
#include "synthEditor.h"
#include "../models/synth.h"
#include <imgui.h>

SynthEditor::SynthEditor(std::string name, Synth* synth) {
	this->name = name;
	this->synth = synth;
	closable = true;
	visible = true;

	selectedWaveForm = (int)synth->waveform;
}

void SynthEditor::update() {
	ImGui::Text("synth editor");

	ImGui::SliderFloat("volume (db)", &synth->loudness, 0.0f, 1.0f, "%.3f");
	//convert layer names to char arrays
	const char* items[] = {"triangle", "sawtooth", "sine", "square", "noise"};

	if (ImGui::Combo("waveform", &selectedWaveForm, items, IM_ARRAYSIZE(items))) {
		synth->waveform = (WaveForm)selectedWaveForm;
	}
	
}