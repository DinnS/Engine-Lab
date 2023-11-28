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
	this->healthBarMaxWidth = 300.f;
	this->healthBarMaxHeight = 30.f;

	float x = 20.f;
	float y = 20.f;

	this->healthBarBack.setSize(sf::Vector2f(this->healthBarMaxWidth, this->healthBarMaxHeight));
	this->healthBarBack.setFillColor(sf::Color(50,50,50, 200));
	this->healthBarBack.setPosition(x, y);

	this->healthBarInner.setSize(sf::Vector2f(this->healthBarMaxWidth, this->healthBarMaxHeight));
	this->healthBarInner.setFillColor(sf::Color(250, 20, 20, 200));
	this->healthBarInner.setPosition(this->healthBarBack.getPosition());


	this->healthBarText.setFont(this->font);
	this->healthBarText.setCharacterSize(25);
	//this->healthBarText.setFillColor(sf::Color::Black);
	this->healthBarText.setPosition(
		(this->healthBarBack.getGlobalBounds().width / 2) - (this->healthBarText.getGlobalBounds().width / 2),
		(this->healthBarBack.getGlobalBounds().height / 2) - (this->healthBarText.getGlobalBounds().height / 2)
	);
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

	float percent = static_cast<float>(this->player->getAttributeComponent()->health) / static_cast<float>(this->player->getAttributeComponent()->healthMax);

	this->healthBarInner.setSize(
		sf::Vector2f(static_cast<float>(std::floor(
			this->healthBarMaxWidth * percent)), 
			this->healthBarMaxHeight
		)
	);
	
	this->healthBarString = std::to_string(this->player->getAttributeComponent()->health) + " / " + std::to_string(this->player->getAttributeComponent()->healthMax);
	this->healthBarText.setString(this->healthBarString);

}

void PlayerGUI::update(const float& dt)
{
	this->updateHealthBar();
}


void PlayerGUI::renderHealthBar(sf::RenderTarget& target)
{
	target.draw(this->healthBarBack);
	target.draw(this->healthBarInner);
	target.draw(this->healthBarText);	
}

void PlayerGUI::render(sf::RenderTarget& target)
{
	this->renderHealthBar(target);
}
