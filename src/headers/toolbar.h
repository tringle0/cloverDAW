#pragma once
#include "tool.h"
#include <vector>

class toolbar{
private:
    int currentToolIndex;
    std::vector<tool> tools;
public:
    void chooseTool(int toolIndex);
};
