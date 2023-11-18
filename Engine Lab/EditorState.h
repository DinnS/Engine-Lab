#pragma once
#include "State.h"
#include "Gui.h"
#include "PauseMenu.h"
#include "TileMap.h"

class State;
class Gui;
class PauseMenu;
class TileMap;


class EditorState :
    public State
{
private:
	// Variables
	sf::View view;

	sf::Font font;
	sf::Text cursorText;
	PauseMenu* pauseMenu;

	std::map<std::string, gui::Button*> buttons;

	std::vector<std::string> tileNames;

	std::map<std::string, TileMap*> tileMaps;


	sf::RectangleShape sidebar;	 

	sf::IntRect textureRect;
	sf::RectangleShape selectorRect;

	std::map<std::string, gui::TextureSelector*> texturesSelector;

	bool collision;
	short type;
	float cameraSpeed;
	int layer;

	// Initializer functions 
	void initVariables();
	void initView();
	void initBackground();
	void initFonts();
	void initTexts();
	void initKeybinds();
	void initPauseMenu();
	void initButtons();
	void initTileMap();
	void initGui();

public:
	// Constructors/Destructors
	EditorState(StateData* state_data);
	virtual ~EditorState();


	//Functions
	void updateInput(const float& dt);
	void updateEditorInput(const float& dt);
	void updateButtons();
	void updateGui(const float& dt);
	void updatePauseMenuButtons();
	void update(const float& dt);
	void renderButtons(sf::RenderTarget& target);
	void renderGui(sf::RenderTarget& target);
	void render(sf::RenderTarget* target = nullptr);

    
};

