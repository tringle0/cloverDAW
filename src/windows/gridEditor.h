#pragma once
#include <vector>
#include "../models/window.h"

class GridEditorWindow : public IWindow {
public:
	GridEditorWindow(App *app) : IWindow("piano roll", app, ImVec2(640, 480), false, true) {};
	void update() override;

private:
	const int margins = 30;
	const int labelSize = 10;
	const int blackNotes[5] = {1,3,6,8,10};
	const std::string noteNames[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

	//number of cells per beat
	int subdivisions = 4;

	//number of beats per measure
	int timeSig = 4;

	//viewport
	ImVec2 viewPos;		//pixels
	ImVec2 viewScale;	//pixels
	
	ImVec2 cellSize;	//pixels

	//grid
	ImVec2 gridPos = ImVec2(0, 72);		//cells
	ImVec2 gridScale = ImVec2(16,24);	//cells

	Layer* layer = nullptr;
	ImDrawList* dl = nullptr;

	//colors
	ImColor cLight = IM_COL32(255, 255, 255, 30);
	ImColor cRegular = IM_COL32(255, 255, 255, 120);
	ImColor cHighlight = IM_COL32(132, 196, 145, 255);
	ImU32 cDim = IM_COL32(0, 0, 0, 120);

	void drawNote(float start, float length, float pitch, ImColor color);
	void drawGrid();
	void drawNotes();
	void scaleGrid();
	void editNotes();
};