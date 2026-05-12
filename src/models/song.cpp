#pragma once
#include "song.h"

Song::Song() {
	title = "untitled";
	artist = "unknown";
}

void Song::addLayer(Layer* layer) {
	layers.push_back(layer);
}

void Song::selectLayer(int index) {
	selectedLayerIndex = index;
}

void Song::removeLayer() {
	if(!layers.empty())
		layers.erase(layers.begin() + selectedLayerIndex);
}

Layer* Song::getLayer() {
	return layers.at(selectedLayerIndex);
}