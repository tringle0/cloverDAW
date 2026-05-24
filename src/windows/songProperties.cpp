#pragma once
#include "../models/window.h"

class SongPropertiesWindow : IWindow{


public:
	SongPropertiesWindow(int layerIndex, App* app) : IWindow("synth editor", app, ImVec2(360, 240), true, false) {
	}
};