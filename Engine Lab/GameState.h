#pragma once
#include "State.h"
#include "PauseMenu.h"
#include "TileMap.h"

class State;
class PauseMenu;
class TileMap;
class Player;
class sf::View;
class sf::Font;
class sf::RenderTexture;

class GameState : public State
{
private:
	sf::View view;
	sf::RenderTexture renderTexture;
	sf::Sprite renderSprite;

	sf::Font font;
	PauseMenu* pauseMenu;

	Player* player;
	
	std::vector<std::string> tileNames;
	std::map<std::string, TileMap*> tileMaps;

	// Initializer functions 
	void initDeferredRender();
	void initView();
	void initKeybinds();
	void initFonts();
	void initTextures();
	void initPauseMenu();
	void initPlayers();
	void initTileMap();

public:
	GameState(StateData* state_data);
	virtual ~GameState();

	//Functions
	void updateView(const float& dt);
	void updateInput(const float& dt);
	void updatePlayerInput(const float& dt);
	void updatePauseMenuButtons();
	void updateTileMap(const float& dt);
	void update(const float& dt);
	void render(sf::RenderTarget* target = nullptr);
};

