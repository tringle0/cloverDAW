#pragma once
#include <imgui.h>
#include <string>
#include <iostream>
#include "../app.h"
#include "window.h"

UniqueCounter IWindow::uc;

//constructor
IWindow::IWindow(std::string name, App *app, ImVec2 size, bool closable, bool scalable) {
	id = uc.produceUnique();
	
	this->size = size;
	this->name = name + "##" + std::to_string(id); //## hides the id
	this->closable = closable;
	this->scalable = scalable;

	this->app = app;
	windowManager = &app->wm;
	song = &app->song;

	std::cout << this->name;
}

//render function: call to render the window: returns true if closed
//this should only be called by windowManager
void IWindow::render(bool& windowOpen) {
	ImGui::SetNextWindowSize(size);
	ImGui::Begin(name.c_str(), closable ? &windowOpen : NULL, scalable ? 0 : ImGuiWindowFlags_NoResize);

	update();

	ImGui::End();
};

void IWindow::onClose() {
	uc.remove(id);
}