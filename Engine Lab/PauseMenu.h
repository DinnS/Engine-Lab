#pragma once

#include "Gui.h"

class Gui;

class PauseMenu
{
private:
	sf::Font& font;
	sf::Text menuText;


	sf::RectangleShape background;
	sf::RectangleShape container;

	std::map<std::string, gui::ButtonColor*> buttons;

	// Functions
	

public:
	PauseMenu(sf::VideoMode& videoMode, sf::Font& font);
	virtual ~PauseMenu();	

	// Accessor
	std::map<std::string, gui::ButtonColor*>& getButtons();

	// Functions
	const bool isButtonPressed(const std::string key);
	void addButton(const std::string key, const float y, const float width, const float height, const unsigned char_size, const std::string text);
	void update(const sf::Vector2i& mousePosWindow);
	void render(sf::RenderTarget& target);
};

