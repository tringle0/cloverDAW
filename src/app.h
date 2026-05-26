#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "controllers/windowManager.h"
#include "models/song.h"
#include "controllers/notePlayer.h"
#include "models/sessionData.h"
#include "controllers/audioPlayer.h"
#include "controllers/saveManager.h"

class App {
public:

	SessionData sessionData;
	WindowManager wm;
	SaveManager sm;
	Song song;
	AudioPlayer audio;
	NotePlayer nPlayer;

	//rendering related stuff
	SDL_Event event;
	SDL_Window* window = nullptr;
	SDL_GLContext gl_context = nullptr;

	void init();
	void run();
	void shutdown();
};