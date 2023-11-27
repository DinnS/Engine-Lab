#include "stdafx.h"
#include "PlayerGUI.h"

void PlayerGUI::initFont()
{
	if (!this->font.loadFromFile("Fonts/EBGaramond-Medium.ttf")) {
		throw("ERROR::GameState::COULD NOT LOAD FONT ");
	}
}

void PlayerGUI::initHealthBar()
{
	float width = 300.f;
	float height = 30.f;
	float x = 20.f;
	float y = 20.f;


	this->healthBarBack.setSize(sf::Vector2f(width, height));
	this->healthBarBack.setFillColor(sf::Color(50,50,50, 200));
	this->healthBarBack.setPosition(x, y);

	this->healthBarInner.setSize(sf::Vector2f(width, height));
	this->healthBarInner.setFillColor(sf::Color(250, 20, 20, 200));
	this->healthBarInner.setPosition(this->healthBarBack.getPosition());
}

// Constructor / Destructor
PlayerGUI::PlayerGUI(Player* player)
{
	this->player = player;

	this->initFont();
	this->initHealthBar();
}

PlayerGUI::~PlayerGUI()
{
	
}


// Functions

void PlayerGUI::updateHealthBar()
{
}

void PlayerGUI::update(const float& dt)
{
	this->updateHealthBar();
}


void PlayerGUI::renderHealthBar(sf::RenderTarget& target)
{
	target.draw(this->healthBarBack);
	target.draw(this->healthBarInner);
}

void PlayerGUI::render(sf::RenderTarget& target)
{
	this->renderHealthBar(target);
}
