#pragma once
#include "synthEditor.h"
#include <imgui.h>

SynthEditor::SynthEditor(std::string name, Synth* synth) {
	this->name = name;
	this->synth = synth;
	closable = true;
	visible = true;
}

void SynthEditor::update() {
	ImGui::Text("synth editor");
	//ImGui::SliderFloat("volume",-1,1);
}