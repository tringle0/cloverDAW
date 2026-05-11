#pragma once
#include "cloverDAW.h"
#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include <SDL3/SDL_opengl.h>
#include "windowManager.h"
#include "windows/exampleWindow.h" 

int main()
{

    //setup SDL
    SDL_Init(SDL_INIT_VIDEO);

    //create window and openGL context
    SDL_Window* window = SDL_CreateWindow("CloverDAW", 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    //imgui
    ImGui::CreateContext();
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);

    //connect imgui to openGL
    ImGui_ImplOpenGL3_Init("#version 130");


    WindowManager wm;
    wm.addWindow(new ExampleWindow("example window 1"));
    wm.addWindow(new ExampleWindow("example window 2"));


    SDL_Event event;
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

    //cleanup stuff
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}