#pragma once
#include "../effect.h"

class Detune : public IEffect {
public:
	std::pair<float, float> apply(float amp, float freq) override {
		return {amp, freq * pow(2.0f, detune/12.0f)}
	}



private:
	float detune; // number of semitones
};