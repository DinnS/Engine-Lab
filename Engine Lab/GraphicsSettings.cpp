#include "stdafx.h"
#include "GraphicsSettings.h"

// Constructors / Destructors
GraphicsSettings::GraphicsSettings() {
	this->title = "DEFAULT";
	this->resolution = sf::VideoMode::getDesktopMode();
	this->fullscreen = false;
	this->vSync = false;
	this->frameRateLimit = 120;
	this->contextSettings.antialiasingLevel = 0;
	this->videoModes = sf::VideoMode::getFullscreenModes();
}

GraphicsSettings::~GraphicsSettings()
{

}


// Functions
void GraphicsSettings::saveToFile(std::string path) {
	std::ofstream ofs(path);

	if (ofs.is_open()) {
		ofs << this->title;
		ofs << this->resolution.width;
		ofs << this->resolution.height;
		ofs << this->fullscreen;
		ofs << this->frameRateLimit;
		ofs << this->vSync;
		ofs << this->contextSettings.antialiasingLevel;
	}

	ofs.close();
}

void GraphicsSettings::loadFromFile(std::string path) {
	std::ifstream ifs(path);

	if (ifs.is_open()) {
		std::getline(ifs, this->title);
		ifs >> this->resolution.width;
		ifs >> this->resolution.height;
		ifs >> this->fullscreen;
		ifs >> this->frameRateLimit;
		ifs >> this->vSync;
		ifs >> this->contextSettings.antialiasingLevel;
	}

	ifs.close();
}