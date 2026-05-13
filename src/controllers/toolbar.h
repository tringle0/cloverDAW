#pragma once
#include "tool.h"
#include "../models/window.h"
#include <vector>

class toolbar{
private:
    int currentToolInd;

public:

    

    void renderToolbar(); // renders entire toolbar
    void renderCurrTool(int index); // renders currently selected tool
};
