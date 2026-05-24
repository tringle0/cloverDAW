#pragma once
#include <cmath>
#include <algorithm>
#include <iostream>
#include <string>
#include "gridEditor.h"

void GridEditor::drawGrid() {
    //draw horizontal lines
    for (int rowCell = 0; rowCell < gridScale.y; rowCell++) {
        float pitch = gridPos.y - rowCell;
        if (pitch < 0 || pitch > 127) continue;
        bool octave = (int)pitch % 12 == 0;

        float y = viewPos.y + rowCell * cellSize.y;
        dl->AddLine({ viewPos.x, y + cellSize.y }, { viewPos.x + viewScale.x, y + cellSize.y }, octave? cHighlight: cRegular);
    }

    //draw vertical lines
    for (int colCell = 0; colCell < gridScale.x*subdivisions; colCell++) {
        int col = (int)gridPos.x + colCell;
        bool quarter = col % subdivisions == 0;
        bool barLine = quarter && col %(subdivisions*timeSig) == 0;

        float x = viewPos.x + colCell * cellSize.x;
        dl->AddLine({ x, viewPos.y }, { x, viewPos.y + viewScale.y }, quarter ? (barLine ? cHighlight : cRegular) : cLight);
    }
}

void GridEditor::drawNotes() {
    for (auto& n : layer->notes) {
        //grid positions relative to top left of grid
        int r = - n.pitch + gridPos.y;
        float c = n.start - gridPos.x;

        //check bounds
        if (r < 0 || r >= gridScale.y) continue; 
        if (c < 0 || c >= gridScale.x) continue; 

        //calculate note positions
        float x1 = viewPos.x + c * cellSize.x * subdivisions;
        float x2 = x1 + n.length * cellSize.x * subdivisions - 1;
        float y1 = viewPos.y + r * cellSize.y + 1;
        float y2 = y1 + cellSize.y - 2;

        //draw notes
        dl->AddRectFilled({ std::max(x1,viewPos.x), y1 }, { std::min(x2,viewPos.x + viewScale.x), y2 }, cHighlight, 0);
    }

}

void GridEditor::scaleGrid() {
    ImVec2 mousePos = ImGui::GetMousePos();

    //convert mouse position to grid coordinates (beat, pitch)
    float mouseBeat = gridPos.x + (mousePos.x - viewPos.x) / (cellSize.x * subdivisions);
    int mousePitch = gridPos.y - (int)((mousePos.y - viewPos.y) / cellSize.y);

    //clamp values
    mouseBeat = std::max(0.f, mouseBeat);
    mousePitch = std::clamp(mousePitch, 0, 127);

    //create an invisible button for detecting whether mouse is inside of grid
    ImGui::SetCursorPos({ (float)margins, (float)margins + ImGui::GetFrameHeight() });
    ImGui::InvisibleButton("##grid", viewScale);

    //manage input
    if (ImGui::IsItemHovered()) {
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
        //add note on left click
        if (ImGui::IsMouseClicked(0)) {
            float snappedBeat = floorf(mouseBeat*subdivisions)/subdivisions;
            std::cout << "placed note" << snappedBeat << "," << mousePitch << std::endl;
            layer->notes.push_back({ snappedBeat, 1/(float)subdivisions, mousePitch });
        }
        //remove note on right click
        if (ImGui::IsMouseClicked(1)) {
            float snappedBeat = floorf(mouseBeat * subdivisions) / subdivisions;
            auto it = std::find_if(layer->notes.begin(), layer->notes.end(), [&](const Note& n) {
                return n.pitch == mousePitch && snappedBeat >= n.start && snappedBeat < n.start + n.length;
            });
            if (it != layer->notes.end()) layer->notes.erase(it);
        }
    }
}

void GridEditor::update() {
    //calculate screen size of a cell
    cellSize = { viewScale.x / gridScale.x / subdivisions, viewScale.y / gridScale.y };

    //display layer name
    if (layer != nullptr) {
        ImGui::Text(layer->name.c_str());
    }
    dl = ImGui::GetWindowDrawList();
    viewPos = { ImGui::GetWindowPos().x + margins, ImGui::GetWindowPos().y + margins + ImGui::GetFrameHeight()};
    viewScale = { ImGui::GetWindowWidth() - 2 * margins, ImGui::GetWindowHeight() - 2 * margins - ImGui::GetFrameHeight() };


    drawGrid();
    if (!song->layers.empty()) {
        layer = song->layers[app->sessionData.selectedLayerIndex];
    }
    if (layer != nullptr) {
        drawNotes();
    }
    scaleGrid();

    
}