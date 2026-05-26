#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <map>
#include "../models/synth.h"
#include "../models/note.h"
#include "../models/layer.h"
#include "../models/effect.h"

class AudioPlayer
{
public:

    // Initializes the hardware audio device and stream
    bool init();

    // Shuts down the audio hardware
    void shutdown();

    // Generates the waveform and pushes it straight to speakers

    void play(std::vector<std::pair<Note*, Layer*>> toPlay);

    // toggles the playing of the waveform
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