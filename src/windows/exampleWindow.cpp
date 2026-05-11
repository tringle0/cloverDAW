#pragma once
#include "../headers/window.h"
#include "imgui.h"

class ExampleWindow : IWindow {
    std::string name = "example window";
    void update() {
        ImGui::Text("hello world!");
        if (ImGui::Button("button")) {
            // do something
        }
	}
};