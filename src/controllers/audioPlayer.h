#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "../models/synth.h"

class audioPlayer
{
public:

    // Initializes the hardware audio device and stream
    bool init();

    // Shuts down the audio hardware
    void shutdown();

    // Generates the waveform and pushes it straight to speakers
    void update(Synth& currentSynthState);

    // toggles the playing of the waveform
    void triggerWave(bool shouldPlay);

    // changing volume
    void changeVolume(float vol);

    audioPlayer();

private:
    SDL_AudioStream* audioStream = nullptr; 

    bool isPlayTriggered = false;
    float phase = 0.0f;
    float volume = 0.0f; 
    const float sampleRate = 44100.0f; // default sample rate, how fast speaker cone (or voice coil) moves
    const float frequency = 440.0f; // Fixed A4 note for testing
};