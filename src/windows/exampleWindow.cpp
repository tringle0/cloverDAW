#pragma once
#include "exampleWindow.h"
#include <imgui.h>

void ExampleWindow::update() {
    ImGui::Text("hello world!");
    if (ImGui::Button("button")) {
        // do something
    }
}