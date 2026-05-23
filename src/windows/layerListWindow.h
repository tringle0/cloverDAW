#pragma once
#include "../models/song.h"
#include "../models/window.h"
#include "../controllers/windowManager.h"
#include "../models/uniqueCounter.h"

class LayerListWindow : public IWindow {
private:
	static UniqueCounter nextLayerDefaultName;

public:
	int selectedLayerIndex = 0;
	LayerListWindow(Song* song, WindowManager* wm) : IWindow("layer list", song, wm, ImVec2(250, 450), true, false) {};
	void update() override;
};

