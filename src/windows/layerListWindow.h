#pragma once
#include "../models/song.h"
#include "../models/window.h"

class LayerListWindow : public IWindow {
public:
	Song* song;
	LayerListWindow(std::string name);
	void update();
};

