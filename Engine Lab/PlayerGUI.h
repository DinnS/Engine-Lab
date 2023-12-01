#pragma once

#include "Player.h"
#include "Gui.h"

class Gui;
class Player;
class sf::RectangleShape;

class PlayerGUI
{
private:
	Player* player;

	sf::VideoMode& videoMode;
	sf::Font font;

	// Health Bar
	std::string healthBarString;
	sf::Text healthBarText;
	float healthBarWidth;
	float healthBarHeight;
	float healthBarPositionX;
	float healthBarPositionY;

	sf::RectangleShape healthBarBack;
	sf::RectangleShape healthBarInner;

	// EXP Bar
	std::string expBarString;
	sf::Text expBarText;
	float expBarWidth;
	float expBarHeight;
	float expBarPositionX;
	float expBarPositionY;

	sf::RectangleShape expBarBack;
	sf::RectangleShape expBarInner;

	// Level Bar
	std::string levelBarString;
	sf::Text levelBarText;
	float levelBarWidth;
	float levelBarHeight;
	float levelBarPositionX;
	float levelBarPositionY;

	sf::RectangleShape levelBarBack;
	
	

	// Functions
	
	void initFont();
	void initHealthBar();
	void initExpBar();
	void initLevelBar();

public:
	// Constructor / Destructor
	PlayerGUI(Player* player, sf::VideoMode& videoMode);
	virtual ~PlayerGUI();

	// Functions
	void updateHealthBar();
	void updateExpBar();
	void updateLevelBar();
	void update(const float& dt);

	void renderHealthBar(sf::RenderTarget& target);
	void renderExpBar(sf::RenderTarget& target);
	void renderLevelBar(sf::RenderTarget& target);
	void render(sf::RenderTarget& target);
};