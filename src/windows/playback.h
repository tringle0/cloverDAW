#pragma once
#include <chrono>
#include "../models/window.h"

//forward declarations
class App;

class PlaybackWindow : public IWindow {
private:
	std::chrono::steady_clock::time_point startedTime;	//time when playback started
	std::chrono::steady_clock::time_point now;	//current time

	float startedBeat = 0;	//beat of the song at the start of the playback

	bool loop = false;

public:
	PlaybackWindow(App* app);
	void update();
	void startPlayback();
	void stopPlayback();
};