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
        //don't draw outside of range
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
        dl->AddRectFilled({ std::max(x1,viewPos.x), y1 }, { std::min(x2,viewPos.x + viewScale.x), y2 }, cHighlight, 0);
    }

}

void GridEditor::scaleGrid() {
    ImGui::SetCursorPos({ 0, 0 });
    ImGui::InvisibleButton("##grid", viewScale);

    ImVec2 mousePos = ImGui::GetMousePos();
    float mouseBeat = gridPos.x + (mousePos.x - viewPos.x) / (cellSize.x * subdivisions);
    int mPitch = gridPos.y - (int)((mousePos.y - viewPos.y) / cellSize.y);
    mouseBeat = std::max(0.f, mouseBeat);
    mPitch = std::clamp(mPitch, 0, 127);

    if (ImGui::IsItemHovered()) {
        ImGuiIO& io = ImGui::GetIO();
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
            float snappedBeat = roundf(mouseBeat*subdivisions)/subdivisions;
            std::cout << "placed note" << snappedBeat << "," << mPitch << std::endl;
            layer->notes.push_back({ snappedBeat, 1/(float)subdivisions, mPitch });
        }
        //remove note on right click
        if (ImGui::IsMouseClicked(1)) {
            float snappedBeat = roundf(mouseBeat * subdivisions) / subdivisions;
            auto it = std::find_if(layer->notes.begin(), layer->notes.end(), [&](const Note& n) {
                return n.pitch == mPitch && snappedBeat >= n.start && snappedBeat < n.start + n.length;
            });
            if (it != layer->notes.end()) layer->notes.erase(it);
        }
    }
}

void GridEditor::update() {
    cellSize = { viewScale.x / gridScale.x / subdivisions, viewScale.y / gridScale.y };

    if (layer != nullptr) {
        ImGui::Text(layer->name.c_str());
        ImGui::Text(std::to_string(gridPos.x).c_str());
        ImGui::Text(std::to_string(gridPos.y).c_str());
    }
    dl = ImGui::GetWindowDrawList();
    viewPos = { ImGui::GetWindowPos().x + margins, ImGui::GetWindowPos().y + margins };
    viewScale = { ImGui::GetWindowWidth() - 2 * margins, ImGui::GetWindowHeight() - 2 * margins };


    ImVec2 clipMin = { viewPos.x, viewPos.y };
    ImVec2 clipMax = { viewPos.x + viewScale.x, viewPos.y + viewScale.y };
    dl->PushClipRect(clipMin, clipMax, true);
    drawGrid();
    if (!song->layers.empty()) {
        layer = song->layers[0];
    }
    if (layer != nullptr) {
        drawNotes();
    }
    scaleGrid();
    dl->PopClipRect();
    
}