#pragma once

#include "State.h"
#include "Gui.h"

class SettingsState :
	public State
{
private:
	// Variables
	sf::Texture backgroundTexture;
	sf::RectangleShape background;
	sf::Font font;

	std::map<std::string, gui::Button*> buttons;
	std::map<std::string, gui::DropDownList*> dropDownLists;

	sf::Text optionsText;
	std::vector<sf::VideoMode> modes;

	// Initializer functions
	void initVariables();
	void initFonts();
	void initKeybinds();
	void initGui();
	void resetGui();

public:
	// Constructors/Destructors
	SettingsState(StateData* state_data);
	virtual ~SettingsState();


	// Accessors


	// Functions
	void updateInput(const float& dt);
	void updateGui(const float& dt);
	void update(const float& dt);
	void renderGui(sf::RenderTarget&);
	void render(sf::RenderTarget* target = nullptr);
};

