#pragma once
#include <vector>
#include <map>
#include <utility>
#include <SDL3/SDL.h>
#include "../models/note.h"
#include "../models/layer.h"


class AudioPlayer
{
public:
    bool init();
    void shutdown();
    void play(std::vector<std::pair<Note*, Layer*>> toPlay);
    void triggerWave(bool shouldPlay);

    AudioPlayer();

private:
    SDL_AudioStream* audioStream = nullptr; 

    std::map<Note*, float> notePhases;

    bool isPlayTriggered = false;
    float phase = 0.0f;
    float volume = 0.0f; 
    float frequency = 440.0f; // Fixed A4 note for default value
    const float sampleRate = 44100.0f; // default sample rate, how fast speaker cone (or voice coil) moves
};