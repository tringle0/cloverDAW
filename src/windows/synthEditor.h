#pragma once
#include "../models/layer.h"
#include "../models/window.h"
#include "../models/uniqueCounter.h"

class LayerEditor : public IWindow{
private:
	Layer* layer;
	int selectedWaveForm; //for the dropdown

public:
	LayerEditor(int layerIndex, Song* song, WindowManager* wm) : IWindow("synth editor", song, wm) {
		layer = song->layers.at(layerIndex);
		selectedWaveForm = layer->synth.waveform;
	}

	
	void update();
};