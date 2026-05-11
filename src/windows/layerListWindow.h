#pragma once
#include "../models/layerList.h"
#include "../models/window.h"

class LayerListWindow : public IWindow {
private:
	LayerList* layerList;
public:
	LayerListWindow(std::string name);
	void update();
};

