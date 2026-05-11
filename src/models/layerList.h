#pragma once
#include <vector>
#include "layer.h"

class LayerList {
private:
	int selectedLayerIndex = 0;
	std::vector<Layer*> layers;

public:
	void addLayer(Layer* layer);
	void selectLayer(int index);
	void removeLayer(int index);
};