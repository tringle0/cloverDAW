#include "playback.h"
#include <imgui.h>
#include <chrono>
#include "../app.h"
#include "../models/song.h"

float secondsToBeats(float seconds, float bpm) {
	return seconds * (bpm / 60.f);
}

PlaybackWindow::PlaybackWindow(App* app) : IWindow("playback", app, ImVec2(240, 80), false, false) {}

void PlaybackWindow::startPlayback() {
	startedTime = now;
	startedBeat = app->sessionData.playheadBeat;
}

void PlaybackWindow::stopPlayback() {
	app->sessionData.playing = false;
	app->sessionData.playheadBeat = startedBeat;
}

void PlaybackWindow::update() {
	now = std::chrono::steady_clock::now();

	//play button
	if (ImGui::Button(app->sessionData.playing ?  "stop":"play")) {
		//toggle playing state
		app->sessionData.playing = !app->sessionData.playing;
		
		if (app->sessionData.playing) {
			startPlayback();
		}
		else {
			stopPlayback();
		}
	}

	ImGui::SameLine();
	ImGui::Checkbox("loop", &loop);

	//calculate current beat
	float songLength = app->song.lengthBeats();
	float elapsed = std::chrono::duration<float>(now - startedTime).count();
	if (app->sessionData.playing) {
		app->sessionData.playheadBeat = startedBeat + secondsToBeats(elapsed, app->song.bpm);
	}

	//handle when last beat reached
	if (app->sessionData.playheadBeat > songLength) {
		if (loop) {
			app->sessionData.playheadBeat = 0;
			startPlayback();
		}
		else {
			stopPlayback();
		}
	}

	//progress bar
	if( ImGui::SliderFloat("progress", &app->sessionData.playheadBeat, 0, songLength, "%.1f beats")) {
		startPlayback();
	}


}