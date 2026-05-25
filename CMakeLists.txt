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
"src/models/song.h"

"src/models/uniqueCounter.h"

"src/models/tools/drawTool.h"
"src/models/tools/selectTool.h"
"src/models/tools/tool.h"


"src/windows/layerListWindow.h"
"src/windows/layerListWindow.cpp"
"src/windows/gridEditor.h"
"src/windows/gridEditor.cpp"


"src/controllers/windowManager.h"
"src/controllers/windowManager.cpp"
"src/controllers/editor.h"
"src/controllers/playback.h"
"src/controllers/toolbar.h"


"src/windows/synthEditor.h"
"src/windows/synthEditor.cpp"


   "src/models/window.cpp" "src/controllers/audioPlayer.cpp")

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




