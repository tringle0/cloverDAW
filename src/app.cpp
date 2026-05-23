#pragma once
#include "cloverDAW.h"
#include "app.h"

void App::init() {
    //setup SDL
    SDL_Init(SDL_INIT_VIDEO);

    //create window and openGL context
    window = SDL_CreateWindow("CloverDAW", 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    gl_context = SDL_GL_CreateContext(window);

    //imgui
    ImGui::CreateContext();
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);

    //connect imgui to openGL
    ImGui_ImplOpenGL3_Init("#version 130");

    //setup window manager
    //wm.addWindow(new ExampleWindow("example window 1"));
    wm.addWindow(new LayerListWindow(&song, &wm));
    wm.addWindow(new GridEditor(&song, &wm));
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
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}