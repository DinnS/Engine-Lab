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
	this->healthBarWidth = gui::percentToPixelX(15.f, this->videoMode);
	this->healthBarHeight = gui::percentToPixelY(3.f, this->videoMode);

	this->healthBarPositionX = gui::percentToPixelX(1.f, this->videoMode);
	this->healthBarPositionY = gui::percentToPixelX(1.5f, this->videoMode);
	
	this->healthBarBack.setSize(sf::Vector2f(this->healthBarWidth, this->healthBarHeight));
	this->healthBarBack.setFillColor(sf::Color(50,50,50, 200));
	this->healthBarBack.setPosition(this->healthBarPositionX, this->healthBarPositionY);

	this->healthBarInner.setSize(sf::Vector2f(this->healthBarWidth, this->healthBarHeight));
	this->healthBarInner.setFillColor(sf::Color(250, 20, 20, 200));
	this->healthBarInner.setPosition(this->healthBarBack.getPosition());

	this->healthBarText.setFont(this->font);
	this->healthBarText.setCharacterSize(gui::calcCharSize(100, this->videoMode));
	
	this->healthBarText.setPosition(
		this->healthBarBack.getPosition().x + (this->healthBarBack.getGlobalBounds().width / 2.f) - gui::percentToPixelX(0.1f, this->videoMode),
		this->healthBarBack.getPosition().y - gui::percentToPixelY(0.2f, this->videoMode)
	);
}

void PlayerGUI::initExpBar()
{
	this->expBarWidth = gui::percentToPixelX(15.f, this->videoMode);
	this->expBarHeight = gui::percentToPixelY(3.f, this->videoMode);

	this->expBarPositionX = gui::percentToPixelX(1.f, this->videoMode);
	this->expBarPositionY = gui::percentToPixelY(8.f, this->videoMode);;
	
	this->expBarBack.setSize(sf::Vector2f(this->expBarWidth, this->expBarHeight));
	this->expBarBack.setFillColor(sf::Color(50, 50, 50, 200));
	this->expBarBack.setPosition(this->expBarPositionX, this->expBarPositionY);

	this->expBarInner.setSize(sf::Vector2f(this->expBarWidth, this->expBarHeight));
	this->expBarInner.setFillColor(sf::Color(20, 20, 250, 200));
	this->expBarInner.setPosition(this->expBarBack.getPosition());


	this->expBarText.setFont(this->font);
	this->expBarText.setCharacterSize(gui::calcCharSize(100, this->videoMode));
	
	this->expBarText.setPosition(
		this->expBarBack.getPosition().x + (this->expBarBack.getGlobalBounds().width / 2.f) - gui::percentToPixelX(0.1f, this->videoMode),
		this->expBarBack.getPosition().y - gui::percentToPixelY(0.2f, this->videoMode)
	);
}

void PlayerGUI::initLevelBar()
{
	this->levelBarWidth = gui::percentToPixelX(2.f, this->videoMode);
	this->levelBarHeight = gui::percentToPixelY(3.f, this->videoMode);

	this->levelBarPositionX = gui::percentToPixelX(1.f, this->videoMode);
	this->levelBarPositionY = gui::percentToPixelY(13.f, this->videoMode);

	this->levelBarBack.setSize(sf::Vector2f(this->levelBarWidth, this->levelBarHeight));
	this->levelBarBack.setFillColor(sf::Color(50, 50, 50, 200));
	this->levelBarBack.setPosition(this->levelBarPositionX, this->levelBarPositionY);

	this->levelBarText.setFont(this->font);
	this->levelBarText.setCharacterSize(gui::calcCharSize(100, this->videoMode));

	this->levelBarText.setPosition(
		this->levelBarBack.getPosition().x + (this->levelBarBack.getGlobalBounds().width / 2.f) - gui::percentToPixelX(0.1f, this->videoMode),
		this->levelBarBack.getPosition().y - gui::percentToPixelY(0.2f, this->videoMode)
	);
}

// Constructor / Destructor	
PlayerGUI::PlayerGUI(Player* player, sf::VideoMode& videoMode)
	: videoMode(videoMode)
{
	this->player = player;

	this->initFont();
	this->initHealthBar();
	this->initExpBar();
	this->initLevelBar();
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
			this->healthBarWidth * percent)), 
			this->healthBarHeight
		)
	);
	
	this->healthBarString = std::to_string(this->player->getAttributeComponent()->health) + " / " + std::to_string(this->player->getAttributeComponent()->healthMax);
	this->healthBarText.setString(this->healthBarString);

}

void PlayerGUI::updateExpBar()
{
	float percent = static_cast<float>(this->player->getAttributeComponent()->exp) / static_cast<float>(this->player->getAttributeComponent()->expNext);

	this->expBarInner.setSize(
		sf::Vector2f(static_cast<float>(std::floor(
			this->expBarWidth * percent)),
			this->expBarHeight
		)
	);

	this->expBarString = std::to_string(this->player->getAttributeComponent()->exp) + " / " + std::to_string(this->player->getAttributeComponent()->expNext);
	this->expBarText.setString(this->expBarString);
}

void PlayerGUI::updateLevelBar()
{
	this->levelBarString = std::to_string(this->player->getAttributeComponent()->level);
	this->levelBarText.setString(this->levelBarString);
}

void PlayerGUI::update(const float& dt)
{
	this->updateHealthBar();
	this->updateExpBar();
	this->updateLevelBar();
}




void PlayerGUI::renderHealthBar(sf::RenderTarget& target)
{
	target.draw(this->healthBarBack);
	target.draw(this->healthBarInner);
	target.draw(this->healthBarText);	
}

void PlayerGUI::renderExpBar(sf::RenderTarget& target)
{
	target.draw(this->expBarBack);
	target.draw(this->expBarInner);
	target.draw(this->expBarText);
}

void PlayerGUI::renderLevelBar(sf::RenderTarget& target)
{
	target.draw(this->levelBarBack);
	target.draw(this->levelBarText);
}

void PlayerGUI::render(sf::RenderTarget& target)
{
	this->renderHealthBar(target);
	this->renderExpBar(target);
	this->renderLevelBar(target);
}
