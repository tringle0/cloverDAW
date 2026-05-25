#pragma once
#include "../models/window.h"

class SongPropertiesWindow : public IWindow {

public:
	SongPropertiesWindow(App* app) : IWindow("song properties", app, ImVec2(240, 180), false, false) {}

	void update();
};