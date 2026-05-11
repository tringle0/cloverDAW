#pragma once
#include "synth.h"

//a layer represents a set of notes of one instrument type
class Layer {
	Synth synth;
	std::vector<Synth> notes;
};