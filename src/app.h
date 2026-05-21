#pragma once
#include "controllers/windowManager.h"
#include "models/song.h"
#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#pragma once
#include <imgui_impl_opengl3.h>
#include <SDL3/SDL_opengl.h>
#include "windows/exampleWindow.h" 
#include "windows/layerListWindow.h" 
#include "windows/gridEditor.h" 


class App {
public:
	
	WindowManager wm;
	Song song;

	//rendering related stuff
	SDL_Event event;
	SDL_Window* window = nullptr;
	SDL_GLContext gl_context = nullptr;

	void init();
	void run();
	void shutdown();
};