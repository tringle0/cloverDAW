#pragma once
#include "../models/synth.h"
#include "../models/window.h"
class SynthEditor : public IWindow{
public:
	Synth* synth;
	SynthEditor(std::string name, Synth* synth);

	int selectedWaveForm;
	void update();
};