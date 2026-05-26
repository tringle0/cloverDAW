#pragma once
#include "../models/window.h"

//forward declarations
class App;

class SongPropertiesWindow : public IWindow {
public:
	SongPropertiesWindow(App* app);
	void update();
};