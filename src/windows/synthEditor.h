#include <imgui.h>
#include "../models/window.h"
#include "../app.h"

//forward declarations
class Layer;
class App;

class LayerEditorWindow : public IWindow{
private:
	Layer* layer;
	int selectedWaveForm; //for the dropdown

public:
	LayerEditorWindow(int layerIndex, App* app) : IWindow("synth editor", app, ImVec2(360,240), true, false) {
		layer = song->layers.at(layerIndex);
		selectedWaveForm = layer->synth.waveform;
	}

	
	void update();
};