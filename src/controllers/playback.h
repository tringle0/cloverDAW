#pragma once
#include "../models/window.h"

class playback{
private:
    double currentBeat;
    double startBeat;
    bool paused;
public:
    void switchPauseState();
    void restart();

    void renderPlaybackPanel();
};