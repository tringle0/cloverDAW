#pragma once
#include <vector>
#include "layer.h"

class Song {
public:
	std::string title, artist;

	Song();

	int selectedLayerIndex = 0;
	std::vector<Layer*> layers;

	Layer* getLayer();

	void addLayer(Layer* layer);
	void selectLayer(int index);
	void removeLayer();
};