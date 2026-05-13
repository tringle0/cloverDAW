#pragma once
#include "../models/song.h"
#include "../models/window.h"
#include "../controllers/windowManager.h"

class LayerListWindow : public IWindow {
public:
	Song* song;
	WindowManager* wm;
	LayerListWindow(std::string name, Song* song, WindowManager* wm);
	void update();
};

