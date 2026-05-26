#include "audioPlayer.h"
#include "../models/synth.h"
#include "../models/note.h"
#include "../models/layer.h"
#include "../models/effect.h"

#include <iostream>

AudioPlayer::AudioPlayer() {}


bool AudioPlayer::init()
{
    // initial format of audio player
    SDL_AudioSpec spec;
    spec.format = SDL_AUDIO_F32;
    spec.channels = 1; // subject to change 
    spec.freq = static_cast<int>(sampleRate);

    // Create a stream tied to the default audio output device
    audioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
    std::cout << "Audio initilized, stream=" << audioStream << "\n";
    if (!audioStream)
    {
        std::cout << ("Unable to open audioStream: %s", SDL_GetError());
        return false;
    }

    // Unpause the stream so it can recieve and give audio data
    SDL_ResumeAudioStreamDevice(audioStream);
    return true;
}

void AudioPlayer::shutdown() // shut down and reset current audiostream
{
    if (audioStream)
    {
        SDL_DestroyAudioStream(audioStream);
        audioStream = nullptr;
    }
}

void AudioPlayer::triggerWave(bool shouldPlay)
{
    isPlayTriggered = shouldPlay;
    // Only executes when button not being held down
    if (!isPlayTriggered)
    {
        phase = 0.0f; // Reset the phase on release
        if (audioStream)
        {
            SDL_ClearAudioStream(audioStream); // Instantly flushes remaining sound
        }
    }
}


float toPerceivedVolume(float linear) {
    if (linear <= 0.0f) return 0.0f;
    float dB = (linear - 1.0f) * 60.0f; 
    return std::pow(10.0f, dB / 20.0f);
}

float midiToFreq(int midi) {
    return 440.0f * std::pow(2.0f, (midi - 69) / 12.0f);
}

void AudioPlayer::play(std::vector<std::pair<Note*, Layer*>> toPlay) {
    if (!audioStream) return; // if no audioStream created, return 

    int queuedBytes = SDL_GetAudioStreamQueued(audioStream);    //amount of bytes that are current in the buffer
    int bufferSize = 8192 * sizeof(float);                      //length of buffer, in bytes
    if (queuedBytes >= bufferSize) return; // don't need to add more buffer if there's already enough

    int samplesNeeded = (bufferSize - queuedBytes) / sizeof(float); //number of samples to generate
    std::vector<float> buffer(samplesNeeded, 0.0f);

    std::vector<Note*> toRemove;
    for (auto& kv : notePhases) {
        bool stillActive = false;
        for (auto& pair : toPlay)
            if (pair.first == kv.first) { stillActive = true; break; }
        if (!stillActive)
            toRemove.push_back(kv.first);
    }
    for (Note* n : toRemove)
        notePhases.erase(n);

    // generate and mix each active note
    for (auto& pair : toPlay) {
        Note* note = pair.first;
        Layer* layer = pair.second;

        //individual sample
        float freq = midiToFreq(note->pitch);
        float amplitude = toPerceivedVolume(layer->volume);

        //apply effects
        for (IEffect* e : layer->effects) {
            std::pair<float, float> applied = e->apply(amplitude, freq);
            freq = applied.second;
            amplitude = applied.first;
        }


        float phaseDelta = freq / sampleRate;
        float& localPhase = notePhases[note]; // each note gets its own phase per buffer fill

        for (int i = 0; i < samplesNeeded; i++) {
            float sampleVal = 0.0f;

            switch (layer->synth.waveform) {
            case WaveForm::sine:
                sampleVal = std::sin(localPhase * 2.0f * 3.14159265f);
                break;
            case WaveForm::sawtooth:
                sampleVal = (localPhase * 2.0f) - 1.0f;
                break;
            case WaveForm::square:
                sampleVal = (localPhase < 0.5f) ? 1.0f : -1.0f;
                break;
            case WaveForm::triangle:
                sampleVal = (localPhase < 0.5f) ? (4.0f * localPhase) - 1.0f : 3.0f - (4.0f * localPhase);
                break;
            case WaveForm::noise:
                sampleVal = (static_cast<float>(std::rand()) / RAND_MAX) * 2.0f - 1.0f;
                break;
            }

            buffer[i] += sampleVal * amplitude;

            localPhase += phaseDelta;
            if (localPhase >= 1.0f) localPhase -= 1.0f;
        }
    }

    // normalize by number of active notes to prevent clipping
    for (int i = 0; i < samplesNeeded; i++) {
        buffer[i] = (buffer[i] / toPlay.size());
    }

    SDL_PutAudioStreamData(audioStream, buffer.data(), static_cast<int>(buffer.size() * sizeof(float)));

}