#pragma once
#include <vector>
#include "layer.h"

class Song {
public:
	Song();

	int selectedLayerIndex = 0;
	std::vector<Layer*> layers;

	void addLayer(Layer* layer);
	void selectLayer(int index);
	void removeLayer(int index);
};