#include "gridEditor.h"
#include <algorithm>
#include <string>
#include <cmath>
#include <imgui.h>
#include "../app.h"
#include "../models/layer.h"
#include "../models/note.h"


GridEditorWindow::GridEditorWindow(App* app) : IWindow("piano roll", app, ImVec2(640, 480), false, true) {}

void GridEditorWindow::drawNote(float start, float length, float pitch, ImColor color) {
	// cells relative to top right corner
	int r = -(int)pitch + (int)gridPos.y;
	float c = start * subdivisions - gridPos.x;  

	// bounds (in cells)
	if (r < 0 || r >= (int)gridScale.y) return;
	if (c + length*subdivisions < 0 || c > gridScale.x) return;

	// cells to pixels
	float x1 = viewPos.x + c * cellSize.x;
	float x2 = viewPos.x + (c + length * subdivisions) * cellSize.x - 1;
	float y1 = viewPos.y + r * cellSize.y + 1;
	float y2 = y1 + cellSize.y - 2;

	dl->AddRectFilled(
		{ std::max(x1, viewPos.x), y1 },
		{ std::min(x2, viewPos.x + viewScale.x), y2 },
		color, 0);
}

void GridEditorWindow::drawGrid() {
	//rows
	for (int relativeRow = 0; relativeRow < gridScale.y; relativeRow++) {
		int pitch = gridPos.y - relativeRow;	
		if (pitch < 0 || pitch > 127) continue;
		float y = viewPos.y + relativeRow * cellSize.y;

		//draw horizontal lines
		bool octave = (int)pitch % 12 == 0;
		dl->AddLine({ viewPos.x, y + cellSize.y }, { viewPos.x + viewScale.x, y + cellSize.y }, octave ? cHighlight : cRegular);

		//highlight black key rows
		bool isBlack = false;
		for (int b : blackNotes)
			if (pitch%12 == b) { isBlack = true; break; }
		if (isBlack) dl->AddRectFilled({ viewPos.x, y }, { viewPos.x + viewScale.x, y + cellSize.y }, cDim, 0);

		//note labels
		std::string noteName = noteNames[pitch%12];
		dl->AddText({ viewPos.x - labelSize, y }, cRegular, noteName.c_str());
	}

	//columns
	for (int relativeCol = 0; relativeCol < gridScale.x; relativeCol++) {
		int col = (int)gridPos.x + relativeCol;
		
		//draw vertical lines
		bool quarter = col % subdivisions == 0;
		bool barLine = quarter && col % (subdivisions * timeSig) == 0;
		float x = viewPos.x + relativeCol * cellSize.x;
		dl->AddLine({ x, viewPos.y }, { x, viewPos.y + viewScale.y }, quarter ? (barLine ? cHighlight : cRegular) : cLight);

		//bar labels
		int measureNum = col / subdivisions / timeSig;
		if (barLine) dl->AddText({ x, viewPos.y - labelSize}, cRegular, std::to_string(measureNum).c_str());
	}
}


void GridEditorWindow::drawNotes() {
	for (auto& n : layer->notes) {
		drawNote(n.start, n.length, n.pitch, cHighlight);
	}
}


void GridEditorWindow::scaleGrid() {
	ImGuiIO& io = ImGui::GetIO();

	//zoom and panning of grid
	if (io.MouseWheel != 0.f) {
		if (io.KeyAlt) {
			if (io.KeyShift) {
				gridScale.x = std::clamp(gridScale.x - io.MouseWheel * 2, 4.f, 200.f);
				gridPos.x = std::max(0.f, gridPos.x);
			}
			else {
				gridPos.x = std::max(0.f, gridPos.x - io.MouseWheel * 2);
			}
		}
		else {
			if (io.KeyShift) {
				gridScale.y = std::clamp(gridScale.y - io.MouseWheel * 2, 4.f, 120.f);
				gridPos.y = std::clamp(gridPos.y, gridScale.y, 127.f);
			}
			else {
				gridPos.y = std::clamp(gridPos.y + io.MouseWheel, gridScale.y, 127.f);
			}
		}
	}
}

void GridEditorWindow::editNotes() {
	ImVec2 mousePos = ImGui::GetMousePos();

	//convert mouse position to grid coordinates (beat, pitch)
	float mouseBeat = gridPos.x + (mousePos.x - viewPos.x) / (cellSize.x * subdivisions);
	int mousePitch = gridPos.y - (int)((mousePos.y - viewPos.y) / cellSize.y);

	//clamp values
	mouseBeat = std::max(0.f, mouseBeat);
	mousePitch = std::clamp(mousePitch, 0, 127);

	static float dragStart, dragEnd;

	//set note start on left click
	if (ImGui::IsMouseClicked(0)) {
		dragStart = floorf(mouseBeat * subdivisions) / subdivisions;
	}
	//add note when released
	if (ImGui::IsMouseReleased(0)) {
		dragEnd = ceil(mouseBeat * subdivisions) / subdivisions;
		if (dragEnd > dragStart) {
			float noteLength = dragEnd-dragStart;
			layer->notes.push_back({ dragStart, noteLength, mousePitch });
		}
	}
	//visualize note placement
	if (ImGui::IsMouseDown(0)) {
		drawNote(dragStart, ceil(mouseBeat * subdivisions) / subdivisions - dragStart, mousePitch, cLight);
	}


	//remove note on right click
	if (ImGui::IsMouseDown(1)) {
		float snappedBeat = floorf(mouseBeat * subdivisions) / subdivisions;
		auto it = std::find_if(layer->notes.begin(), layer->notes.end(), [&](const Note& n) {
			return n.pitch == mousePitch && snappedBeat >= n.start && snappedBeat < n.start + n.length;
			});
		if (it != layer->notes.end()) layer->notes.erase(it);
	}
}

void GridEditorWindow::drawPlayhead() {
	if (!song) return;
	float playheadBeat = app->sessionData.playheadBeat;

	// convert beat to x position
	float x = viewPos.x + (playheadBeat * subdivisions - gridPos.x) * cellSize.x;

	// don't draw if outside view
	if (x < viewPos.x || x > viewPos.x + viewScale.x) return;

	dl->AddLine(
		{ x, viewPos.y },
		{ x, viewPos.y + viewScale.y },
		IM_COL32(255, 80, 80, 255),  // red
		3.0f
	);
}



void GridEditorWindow::update() {
	//calculate variables
	dl = ImGui::GetWindowDrawList();
	viewPos = { ImGui::GetWindowPos().x + margins, ImGui::GetWindowPos().y + margins + ImGui::GetFrameHeight() };
	viewScale = { ImGui::GetWindowWidth() - 2 * margins, ImGui::GetWindowHeight() - 2 * margins - ImGui::GetFrameHeight() };
	cellSize = { viewScale.x / gridScale.x, viewScale.y / gridScale.y };
	if (song->layers.empty()) layer = nullptr;
		else layer = song->layers[app->sessionData.selectedLayerIndex];

	drawGrid();
	drawPlayhead();
	//display layer name
	if (layer != nullptr) {
		ImGui::Text(layer->name.c_str());
		drawNotes();
	}

	//create an invisible button for detecting whether mouse is inside of grid
	ImGui::SetCursorPos({ (float)margins, (float)margins + ImGui::GetFrameHeight() });
	ImGui::InvisibleButton("##grid", viewScale);
	if (ImGui::IsItemHovered()) {
		scaleGrid();
		if (layer != nullptr) editNotes();
	}
}