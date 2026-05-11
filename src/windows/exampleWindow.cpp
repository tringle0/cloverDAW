#pragma once
#include "exampleWindow.h"
#include <imgui.h>

ExampleWindow::ExampleWindow(std::string name) {
    this->name = name;
}

void ExampleWindow::update() {
    ImGui::Text("hello world!");
    if (ImGui::Button("button")) {
        // do something
    }
}