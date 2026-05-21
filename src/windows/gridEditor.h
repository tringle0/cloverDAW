#pragma once
#include <vector>
#include "../models/window.h"
#include "../models/note.h"

class GridEditor : public IWindow {
	std::vector<std::vector<Note>> grid;
public:
	void update() override;
	
};