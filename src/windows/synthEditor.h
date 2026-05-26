#pragma once
#include "../models/window.h"

//forward declarations
class Layer;
class App;

class LayerEditorWindow : public IWindow{
private:
	Layer* layer;
	int selectedWaveForm; //for the dropdown
	int selectedEffect; //for the dropdown

public:
	LayerEditorWindow(int layerIndex, App* app);
	void update();
};