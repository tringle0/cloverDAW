#pragma once
#include "song.h"

Song::Song() {
}

void Song::addLayer(Layer* layer) {
	layers.push_back(layer);
}

void Song::selectLayer(int index) {
	selectedLayerIndex = index;
}

void Song::removeLayer(int index) {
	layers.erase(layers.begin() + index);
}