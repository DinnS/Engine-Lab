#pragma once

class GraphicsSettings {
public:

	// Constructors / Destructors
	GraphicsSettings();
	virtual ~GraphicsSettings();

	// Variables
	std::string title;
	sf::VideoMode resolution;
	bool fullscreen;
	bool vSync;
	unsigned frameRateLimit;
	sf::ContextSettings contextSettings;
	std::vector<sf::VideoMode> videoModes;

	// Functions
	void saveToFile(std::string path);

	void loadFromFile(std::string path);
};



