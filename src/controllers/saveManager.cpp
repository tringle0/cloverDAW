#include <string>
#include <fstream> 
#include <windows.h>
#include <shlobj.h>
#include <nlohmann/json.hpp>
#include "saveManager.h"
#include "../app.h"
#include "../models/song.h"
#include <iostream>

std::string getDownloadsFolder() {
    PWSTR path = nullptr;
    SHGetKnownFolderPath(FOLDERID_Downloads, 0, nullptr, &path);

    // convert wide string to regular string
    int size = WideCharToMultiByte(CP_UTF8, 0, path, -1, nullptr, 0, nullptr, nullptr);
    std::string result(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, path, -1, &result[0], size, nullptr, nullptr);

    CoTaskMemFree(path);
    result.resize(size - 1); 
    return result;
}

void SaveManager::init(App* app) {
    this->app = app;
}

void SaveManager::saveSong() {
    nlohmann::json j;

    Song* song = &app->song;
    path = getDownloadsFolder() + "\\" + song->title + ".json";

    std::cout << "Saving to: " << path << "\n";

    j["title"] = song->title;
    j["artist"] = song->artist;
    j["bpm"] = song->bpm;
    j["timeSig"] = song->timeSig;

    j["layers"] = nlohmann::json::array();
    for (Layer* layer : song->layers) {
        nlohmann::json jLayer;
        jLayer["name"] = layer->name;
        jLayer["volume"] = layer->volume;
        jLayer["waveform"] = (int)layer->synth.waveform;

        jLayer["notes"] = nlohmann::json::array();
        for (Note& note : layer->notes) {
            jLayer["notes"].push_back({
                { "start",  note.start  },
                { "length", note.length },
                { "pitch",  note.pitch  }
                });
        }

        j["layers"].push_back(jLayer);
    }

    std::ofstream file(path);
    file << j.dump(4);  // 4 = indent spaces
}

void SaveManager::loadSong(std::string filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) return;

    nlohmann::json j;
    file >> j;

    Song* loaded = new Song();
    loaded->title = j["title"];
    loaded->artist = j["artist"];
    loaded->bpm = j["bpm"];
    loaded->timeSig = j["timeSig"];

    for (auto& jLayer : j["layers"]) {
        Layer* layer = new Layer("loading");
        layer->name = jLayer["name"];
        layer->volume = jLayer["volume"];
        layer->synth.waveform = (WaveForm)jLayer["waveform"].get<int>();

        for (auto& jNote : jLayer["notes"]) {
            Note note;
            note.start = jNote["start"];
            note.length = jNote["length"];
            note.pitch = jNote["pitch"];
            layer->notes.push_back(note);
        }
        loaded->layers.push_back(layer);
    }

    Song* song = &app->song;
    for (Layer* l : song->layers) delete l;  // delete old layers
    *song = *loaded;                          // copy loaded into song
    delete loaded;
}