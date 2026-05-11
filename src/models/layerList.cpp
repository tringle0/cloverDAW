#pragma once
#include "layerList.h"

void LayerList::addLayer(Layer* layer) {
	layers.push_back(layer);
}

void LayerList::selectLayer(int index) {
	selectedLayerIndex = index;
}

void LayerList::removeLayer(int index) {
	layers.erase(layers.begin() + index);
}