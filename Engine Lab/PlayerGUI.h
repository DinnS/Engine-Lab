#pragma once

#include "Player.h"

class Player;
class sf::RectangleShape;

class PlayerGUI
{
private:
	Player* player;

	sf::Font font;

	sf::RectangleShape healthBarBack;
	sf::RectangleShape healthBarInner;

	// Functions
	
	void initFont();
	void initHealthBar();

public:
	// Constructor / Destructor
	PlayerGUI(Player* player);
	virtual ~PlayerGUI();

	// Functions
	void updateHealthBar();
	void update(const float& dt);

	void renderHealthBar(sf::RenderTarget& target);
	void render(sf::RenderTarget& target);
};