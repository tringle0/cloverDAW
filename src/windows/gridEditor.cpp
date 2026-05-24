#pragma once
#include <cmath>
#include <algorithm>
#include <iostream>
#include <string>
#include "gridEditor.h"


void GridEditor::drawNote(float start, float length, float pitch) {
	//grid positions relative to top left of grid
	int r = -pitch + gridPos.y;
	float c = start - gridPos.x;

	//calculate note positions
	float x1 = viewPos.x + c * cellSize.x * subdivisions;
	float x2 = x1 + length * cellSize.x * subdivisions - 1;
	float y1 = viewPos.y + r * cellSize.y + 1;
	float y2 = y1 + cellSize.y - 2;

	//draw notes
	dl->AddRectFilled({ std::max(x1,viewPos.x), y1 }, { std::min(x2,viewPos.x + viewScale.x), y2 }, cHighlight, 0);
}

void GridEditor::drawGrid() {
	//draw horizontal lines
	for (int rowCell = 0; rowCell < gridScale.y; rowCell++) {
		float pitch = gridPos.y - rowCell;
		if (pitch < 0 || pitch > 127) continue;
		bool octave = (int)pitch % 12 == 0;

		float y = viewPos.y + rowCell * cellSize.y;
		dl->AddLine({ viewPos.x, y + cellSize.y }, { viewPos.x + viewScale.x, y + cellSize.y }, octave ? cHighlight : cRegular);
	}

	//draw vertical lines
	for (int colCell = 0; colCell < gridScale.x * subdivisions; colCell++) {
		int col = (int)gridPos.x + colCell;
		bool quarter = col % subdivisions == 0;
		bool barLine = quarter && col % (subdivisions * timeSig) == 0;

		float x = viewPos.x + colCell * cellSize.x;
		dl->AddLine({ x, viewPos.y }, { x, viewPos.y + viewScale.y }, quarter ? (barLine ? cHighlight : cRegular) : cLight);
	}
}

void GridEditor::drawNotes() {
	for (auto& n : layer->notes) {
		drawNote(n.start, n.length, n.pitch);
	}
}


void GridEditor::scaleGrid() {
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

void GridEditor::editNotes() {
	ImVec2 mousePos = ImGui::GetMousePos();

	//convert mouse position to grid coordinates (beat, pitch)
	float mouseBeat = gridPos.x + (mousePos.x - viewPos.x) / (cellSize.x * subdivisions);
	int mousePitch = gridPos.y - (int)((mousePos.y - viewPos.y) / cellSize.y);

	//clamp values
	mouseBeat = std::max(0.f, mouseBeat);
	mousePitch = std::clamp(mousePitch, 0, 127);

	static float dragStart, dragEnd;

	//add note on left click
	if (ImGui::IsMouseClicked(0)) {
		dragStart = floorf(mouseBeat * subdivisions) / subdivisions;
	}
	if (ImGui::IsMouseReleased(0)) {
		dragEnd = ceil(mouseBeat * subdivisions) / subdivisions;
		if (dragEnd >= dragStart) {
			float noteLength = dragEnd-dragStart;
			layer->notes.push_back({ dragStart, noteLength, mousePitch });
		}
	}
	if (ImGui::IsMouseDown(0)) {
		drawNote(dragStart, ceil(mouseBeat * subdivisions) / subdivisions - dragStart, mousePitch);
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

void GridEditor::update() {
	dl = ImGui::GetWindowDrawList();
	viewPos = { ImGui::GetWindowPos().x + margins, ImGui::GetWindowPos().y + margins + ImGui::GetFrameHeight() };
	viewScale = { ImGui::GetWindowWidth() - 2 * margins, ImGui::GetWindowHeight() - 2 * margins - ImGui::GetFrameHeight() };
	cellSize = { viewScale.x / gridScale.x / subdivisions, viewScale.y / gridScale.y };
	if (!song->layers.empty()) layer = song->layers[app->sessionData.selectedLayerIndex];

	//display layer name
	if (layer != nullptr) {
		ImGui::Text(layer->name.c_str());
		drawNotes();
	}

	drawGrid();

	//create an invisible button for detecting whether mouse is inside of grid
	ImGui::SetCursorPos({ (float)margins, (float)margins + ImGui::GetFrameHeight() });
	ImGui::InvisibleButton("##grid", viewScale);
	if (ImGui::IsItemHovered()) {
		scaleGrid();
		if (layer != nullptr) 
			editNotes();
		
	}
}