#include "audioPlayer.h"
#include <iostream>

audioPlayer::audioPlayer() {}


bool audioPlayer::init()
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

void audioPlayer::shutdown() // shut down and reset current audiostream
{
    if (audioStream)
    {
        SDL_DestroyAudioStream(audioStream);
        audioStream = nullptr;
    }
}

void audioPlayer::triggerWave(bool shouldPlay)
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

void audioPlayer::changeVolume(float vol)
{
    volume = vol;
}



void audioPlayer::update(Synth& currentSynthState)
{
    std::cout << "updated, t=" << isPlayTriggered << " q=" << SDL_GetAudioStreamQueued(audioStream) << "\n";
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
    int bufferTarget = 1024 * sizeof(float); // buffer target

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