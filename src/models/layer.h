#pragma once
#include "synth.h"
#include "note.h"
#include <string>

//a layer represents a set of notes of one instrument type
class Layer {
public:
	Layer(std::string name) { this->name = name; };

	std::string name;
	float volume = 0;


	Synth synth;
	std::vector<Note> notes;
};