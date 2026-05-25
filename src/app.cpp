#include "app.h"
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include "windows/layerList.h"
#include "windows/gridEditor.h"
#include "windows/songProperties.h"
#include "windows/playback.h"

void App::init() {
    //setup SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    //create window and openGL context
    window = SDL_CreateWindow("CloverDAW", 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    gl_context = SDL_GL_CreateContext(window);

    //imgui
    ImGui::CreateContext();
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);

    //connect imgui to openGL
    ImGui_ImplOpenGL3_Init("#version 130");

    sessionData = {};
    audio.init();
 


    //setup window manager
    wm.addWindow(new LayerListWindow(this));
    wm.addWindow(new GridEditorWindow(this));
    wm.addWindow(new SongPropertiesWindow(this));
    wm.addWindow(new PlaybackWindow(this));
}

void App::run() {
    bool running = true;
    while (running)
    {
        //handle events
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        // new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        wm.renderAll();

        //only execute if there are layers
        if (!song.layers.empty()) {
            audio.update(song.layers[sessionData.selectedLayerIndex]->synth);
        }

        // render window
        ImGui::Render();

        //clear screen
        glClear(GL_COLOR_BUFFER_BIT);

        //draw window
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);

    }
}

void App::shutdown() {
    //cleanup
    audio.shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}