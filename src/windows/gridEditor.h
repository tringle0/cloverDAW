#pragma once
#include <vector>
#include "../models/window.h"

class GridEditor : public IWindow {
public:
	GridEditor(App *app) : IWindow("piano roll", app, ImVec2(640, 480), false, true) {};
	void update() override;

private:
	//number of cells per beat
	int subdivisions = 4;
	
	//number of beats per measure
	int timeSig = 4;
	
	int margins = 20;

	//viewport (positions)
	ImVec2 viewPos;
	ImVec2 viewScale;
	
	//size of cell (screen size)
	ImVec2 cellSize;

	//grid (in cells)
	ImVec2 gridPos = ImVec2(0, 72);
	ImVec2 gridScale = ImVec2(16,24);

	Layer* layer = nullptr;
	ImDrawList* dl = nullptr;

	//colors
	ImColor cLight = IM_COL32(255, 255, 255, 30);
	ImColor cRegular = IM_COL32(255, 255, 255, 120);
	ImColor cHighlight = IM_COL32(132, 196, 145, 255);

	void drawNote(float start, float length, float pitch);
	void drawGrid();
	void drawNotes();
	void scaleGrid();
	void editNotes();
};