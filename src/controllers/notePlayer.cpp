#include "notePlayer.h"
#include "../models/sessionData.h"
#include "../models/song.h"
#include "../models/layer.h"
#include "audioPlayer.h"
#include "../app.h"
#include <iostream>


//NotePlayer::convertFreq(int pitch) {
//	return 440.0f * (pow(2.0, <double> (pitch - 69) / 12));
//}

void NotePlayer::update(App* app) {
	SessionData* currentSession = &app->sessionData;
	Song* currentSong = &app->song;
	std::vector<Layer*> currentLayers = currentSong->layers;
	std::vector<std::pair<Note*, Layer*>> activeNotes;

	// get current beat
	float currentBeat = currentSession->playheadBeat;
	for (int i = 0; i < currentLayers.size(); i++) { // for every layer of the song
		for (int j = 0; j < currentLayers[i]->notes.size(); j++) {
			if (currentBeat > currentLayers[i]->notes[j].start && currentBeat < currentLayers[i]->notes[j].start + currentLayers[i]->notes[j].length) {
				activeNotes.push_back({&currentLayers[i]->notes[j], currentLayers[i]});
			}
		}
	}

	if (currentSession->playing) {
		std::cout << "isPlaying" << std::endl;
		app->audio.play(activeNotes);
	}
}
	
	//pass a vector of a note and a synth 

