#include "audioPlayer.h"
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

void AudioPlayer::changeVolume(float vol)
{
    volume = vol;
}

// Convert MIDI number to frequency
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
        float freq = midiToFreq(note->pitch + layer->synth.detune);
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

            buffer[i] += sampleVal * layer->volume;

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

void AudioPlayer::update(Synth& currentSynthState)
{
    //std::cout << "updated, t=" << isPlayTriggered << " q=" << SDL_GetAudioStreamQueued(audioStream) << "\n";
    if (!audioStream)
    {
        return; // if no audioStream created, return 
    }
    // if button not held down, return
    if (!isPlayTriggered)
    {
        return;
    }
  
    // need a buffer due to slight framerate inconsistencies, otherwise sound will come out as choppy or stuttering

    int queuedBytes = SDL_GetAudioStreamQueued(audioStream); // gets currently queued bytes
    int bufferTarget = 4096 * sizeof(float); // buffer target

    if (queuedBytes >= bufferTarget)
    {
        return; // don't need to add more buffer if there's already enough
    }
    int samplesNeeded = (bufferTarget - queuedBytes) / sizeof(float);

    std::vector<float> buffer(samplesNeeded);

    // calculate change to current phase, i.e location of player in current cycle
    float phaseDelta = frequency / sampleRate; // sampleRate is fixed, determined by hardware

    // Fill the buffer with the waveforms
    for (int i = 0; i < samplesNeeded; i++)
    {
        float sampleVal = 0.0f;

        switch (currentSynthState.waveform)
        {
        case WaveForm::sine:
            sampleVal = std::sin(phase * 2.0f * 3.14159265f);
            break;

        case WaveForm::sawtooth:
            sampleVal = (phase * 2.0f) - 1.0f;
            break;

        case WaveForm::square:
            sampleVal = (phase < 0.5f) ? 1.0f : -1.0f;
            break;

        case WaveForm::triangle:
            sampleVal = (phase < 0.5f) ? (4.0f * phase) - 1.0f : 3.0f - (4.0f * phase);
            break;

        case WaveForm::noise:
            sampleVal = (static_cast<float>(std::rand()) / RAND_MAX) * 2.0f - 1.0f;
            break;
        }

        // volume is set by slider outside of the class
        buffer[i] = sampleVal * volume;

        // increase phase based on the frequency of the note being played
        phase += phaseDelta;
        if (phase >= 1.0f) // wrap it to start if surpases 1.0f
        {
            phase -= 1.0f;
        }
    }

    // pushes buffer list of floats to SLD3, which then pushes it to physical hardware
    SDL_PutAudioStreamData(audioStream, buffer.data(), static_cast<int>(buffer.size() * sizeof(float)));
}