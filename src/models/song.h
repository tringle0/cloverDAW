#pragma once
#include <vector>
#include "layer.h"

class Song {
public:
	std::string title = "untitled";
	std::string artist = "unknown";
	std::vector<Layer*> layers;
};