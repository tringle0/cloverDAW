#pragma once
#include "../headers/window.h"

class ExampleWindow : public IWindow {
public:
    ExampleWindow(std::string name);
    void update() override;
};