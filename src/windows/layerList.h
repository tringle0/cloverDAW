#pragma once
#include "../models/song.h"
#include "../models/window.h"
#include "../controllers/windowManager.h"

//foward declarations
class App;
class UniqueCounter;

class LayerListWindow : public IWindow {
private:
	static UniqueCounter nextLayerDefaultName;

public:
	LayerListWindow(App *app) : IWindow("layer list", app, ImVec2(250, 450), false, false) {};
	void update() override;
};

