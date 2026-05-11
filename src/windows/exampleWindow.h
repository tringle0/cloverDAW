#pragma once
#include "../headers/window.h"
#include "imgui.h"

class ExampleWindow : public IWindow {
public:
    ExampleWindow::ExampleWindow(std::string name) {
        this->name = name;
    }
    void update() {
        ImGui::Text("hello world!");
        if (ImGui::Button("button")) {
            // do something
        }
	}
};