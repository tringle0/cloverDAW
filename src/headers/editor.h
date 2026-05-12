#pragma once
#include "toolbar.h"
#include "ICommand.h"
#include <stack>


struct visibleArea{
    int topLeftBeat; // x
    int topLeftFrequency; // y
    int width; // pixels
    int height; // pixels
};
class editor{
private:
    visibleArea area;
    int currSelectedlayer;
    std::stack<ICommand> commands;
public:
    void editLayer();
};