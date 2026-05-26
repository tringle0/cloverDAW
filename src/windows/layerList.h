#pragma once
#include "../models/window.h"

//foward declarations
class App;

class LayerListWindow : public IWindow {
private:
	static UniqueCounter nextLayerDefaultName;

public:
	LayerListWindow(App *app);
	void update() override;
};

