#pragma once

#include "Player.h"
#include "GraphicsSettings.h"

class Player;
class GraphicsSettings;
class State;

class StateData {
public:
	// Constructors
	StateData() {};

	// Variables
	float gridSize;
	sf::RenderWindow* window;
	GraphicsSettings* gfxSettings;
	std::map <std::string, int>* supportedKeys;
	std::stack<State*>* states;
};

class State
{
private:


protected:
	std::stack<State*>* states;
	StateData* stateData;

	sf::RenderWindow* window;
	std::map<std::string, int>* supportedKeys;
	std::map<std::string, int> keybinds;
	bool pause;
	bool quit;
	float keyTime;
	float keyTimeMax;
	float gridSize;

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	sf::Vector2i mousePosGrid;


	// Resources
	std::map<std::string, sf::Texture*> textures;

	// Function
	virtual void initKeybinds() = 0;

public:

	// Constructors/Destructors
	State(StateData* state_data);
	virtual ~State();


	// Accessor
	const bool& getQuit() const;
	const bool getKeyTime();

	// Functions
	void pauseState();
	void unpauseState();
	void endState();

	virtual void updateMousePositions(sf::View* view = NULL);
	virtual void updateKeyTime(const float& dt);
	virtual void updateInput(const float& dt) = 0;
	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget* target = NULL) = 0;
};

