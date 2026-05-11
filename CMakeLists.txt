cmake_minimum_required(VERSION 3.10)

project ("cloverDAW" C CXX)


find_package(JUCE CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Add source to this project's executable.
add_executable (cloverDAW "src/cloverDAW.cpp" "cloverDAW.h"  "src/headers/layer.h" "src/headers/synth.h" "src/headers/note.h" "src/headers/window.h" "src/windowManager.h" "src/windows/exampleWindow.cpp")

target_include_directories(cloverDAW PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include
)


target_link_libraries(cloverDAW PRIVATE
    SDL3::SDL3
    imgui::imgui
    opengl32
    juce::juce_core
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
)




