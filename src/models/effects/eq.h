#pragma once
#include "../effect.h"
#include <imgui.h>
#include <vector>

class EQ : public IEffect {
	std::pair<float, float> apply(float amp, float freq) override {
		if (freq < lowCut) {
			return { amp * lowMult, freq };
		}
		else if (freq < highCut) {
			return { amp * midMult, freq };
		}
		else {
			return { amp * highCut, freq };
		}
	}

	void render() override {
		ImGui::SliderFloat("Low Mult", &lowMult, 0, 3, "%.3f");
		ImGui::SliderFloat("Medium Mult", &midMult, 0, 3, "%.3f");
		ImGui::SliderFloat("High Mult", &highMult, 0, 3, "%.3f");

	}

	float lowCut = 300.0f;
	float highCut = 3000.0f;
	float lowMult = 1.0f;
	float midMult = 1.0f;
	float highMult = 1.0f;
};
