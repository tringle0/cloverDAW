#pragma once

//forward declarations
class App;

class SaveManager {
private:
	App* app;
	std::string path = "";

public:
	void init(App* app);
	void saveSong();
	void loadSong(std::string filePath);
};