#pragma once
#include <string>
#include <vector>
#include "layer.h"

class Song {
public:
	std::string title = "untitled";
	std::string artist = "unknown";
	std::vector<Layer*> layers;

	float bpm = 120;

	//number of beats per bar
	int timeSig = 4;

	//returns length in beats
	int lengthBeats();
};