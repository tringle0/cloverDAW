#pragma once

class playback{
private:
    double currentBeat;
    bool paused;
public:
    void switchPauseState();
    void restart();
};