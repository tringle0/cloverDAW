#pragma once
#include "synth.h"
#include "note.h"
#include "effect.h"
#include <string>

//a layer represents a set of notes of one instrument type
class Layer {
public:
	Layer(std::string name) { this->name = name; };

	std::string name;
	float volume = 1.f;


	Synth synth;
	std::vector<Note> notes;
	std::vector<IEffect*> effects;
};