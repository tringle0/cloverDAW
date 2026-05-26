cmake_minimum_required(VERSION 3.10)

project ("cloverDAW" C CXX)


find_package(JUCE CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Add source to this project's executable.
add_executable (cloverDAW


"src/cloverDAW.h"
"src/cloverDAW.cpp"
"src/app.h"
"src/app.cpp"

"src/models/layer.h"
"src/models/synth.h"
"src/models/note.h"
"src/models/window.h"
"src/models/window.cpp"
"src/models/song.h"
"src/models/uniqueCounter.h"
"src/models/tools/drawTool.h"
"src/models/tools/selectTool.h"
"src/models/tools/tool.h"


"src/windows/layerList.h"
"src/windows/layerList.cpp"
"src/windows/gridEditor.h"
"src/windows/gridEditor.cpp"
"src/windows/synthEditor.h"
"src/windows/synthEditor.cpp"
"src/windows/songProperties.h"
"src/windows/songProperties.cpp"
"src/windows/playback.h"
"src/windows/playback.cpp"



"src/controllers/windowManager.h"
"src/controllers/windowManager.cpp"
"src/controllers/audioPlayer.h"
"src/controllers/audioPlayer.cpp"
"src/controllers/notePlayer.h"
"src/controllers/notePlayer.cpp"



 "src/models/effects/detune.h" "src/models/effects/eq.h")

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




