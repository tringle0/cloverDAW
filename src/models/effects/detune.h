#pragma once
#include "../effect.h"
#include <imgui.h>

class Detune : public IEffect {
public:
	std::pair<float, float> apply(float amp, float freq) override {
		return {amp, freq * pow(2.0f, detune/12.0f)}
	}

	void render() override {
		ImGui::SliderFloat("detune (semitones)", &Detune->detune, -24.0f, 24.0f, "%.3f");
	}
	float detune; // number of semitones

};