#pragma once
#include <vector>
#include "../models/window.h"
#include "../models/note.h"

class GridEditor : public IWindow {
public:
	GridEditor(Song* song, WindowManager* wm) : IWindow("piano roll", song, wm, ImVec2(640, 480), true, false) {};
	void update() override;

private:
	//number of cells per beat
	int subdivisions = 4;
	
	//number of beats per measure
	int timeSig = 4;

	float margins = 10;

	
	//viewport (positions)
	ImVec2 viewPos = ImVec2(10, 10);
	ImVec2 viewScale = ImVec2(500, 400);
	
	//size of cell (screen size)
	ImVec2 cellSize;

	//grid (in cells)
	ImVec2 gridPos = ImVec2(0, 0);
	ImVec2 gridScale = ImVec2(40,60);

	Layer* layer = nullptr;
	ImDrawList* dl = nullptr;

	//colors
	ImColor cLight = IM_COL32(255, 255, 255, 30);
	ImColor cRegular = IM_COL32(255, 255, 255, 120);
	ImColor cHighlight = IM_COL32(132, 196, 145, 255);


	void drawGrid();
	void drawNotes();
	void scaleGrid();
};