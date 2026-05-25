#pragma once
#include <vector>
#include <cmath>
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
	int lengthBeats() {
		float maxBeat = 0;
		for (Layer* l : layers) {
			for (Note n : l->notes) {
				maxBeat = std::max(maxBeat, n.start + n.length);
			}
		}
		return (int)std::ceil(maxBeat);
	}
};