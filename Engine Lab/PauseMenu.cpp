#include "stdafx.h"
#include "PauseMenu.h"


// Constructor / Destructor

PauseMenu::PauseMenu(sf::VideoMode& videoMode, sf::Font& font) : font(font)
{

	// Init background

	this->background.setSize(
		sf::Vector2f(
			static_cast<float>(videoMode.width),
			static_cast<float>(videoMode.height)
		)
	);
	this->background.setFillColor(sf::Color(20, 20, 20, 100));


	// Init container 

	this->container.setSize(
		sf::Vector2f(
			static_cast<float>(videoMode.width) / 3.f,
			static_cast<float>(videoMode.height) / 1.2f
		)
	);
	this->container.setFillColor(sf::Color(20, 20, 20, 200));
	this->container.setPosition(
		sf::Vector2f(
			static_cast<float>(videoMode.width) / 2.f - this->container.getSize().x / 2.f,
			static_cast<float>(videoMode.height) / 2.f - this->container.getSize().y / 2.f
		)
	);

	// Init text
	this->menuText.setFont(this->font);
	this->menuText.setFillColor(sf::Color(255,255, 255, 200));
	this->menuText.setCharacterSize(gui::calcCharSize(50, videoMode));
	this->menuText.setString("PAUSED");
	this->menuText.setPosition(
		this->container.getPosition().x + (((this->container.getSize().x) / 2) - ((this->menuText.getGlobalBounds().width / 2.f))),
		this->container.getPosition().y + gui::percentToPixelY(4.f, videoMode)
	);
}

PauseMenu::~PauseMenu()
{
	auto i = this->buttons.begin();
	for (i = this->buttons.begin(); i != this->buttons.end(); ++i) {
		delete i->second;
	}
}


// Accessor

std::map<std::string, gui::ButtonColor*>& PauseMenu::getButtons()
{
	return this->buttons;
}




// Functions

const bool PauseMenu::isButtonPressed(const std::string key) 
{
	return this->buttons[key]->isPressed();
}

void PauseMenu::addButton(
	const std::string key, 
	const float y,
	const float width, const float height,
	const unsigned char_size, const std::string text)
{
	float x = this->container.getPosition().x + (this->container.getSize().x / 2.f - (width / 2.f));
	
	this->buttons[key] = new gui::ButtonColor(
		x, y, width, height,
		&this->font, text, char_size,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	);
}

void PauseMenu::update(const sf::Vector2i& mousePosWindow)
{
	for (auto& i : this->buttons) {
		i.second->update(mousePosWindow);
	}
}

void PauseMenu::render(sf::RenderTarget& target)
{
	target.draw(this->background);

	target.draw(this->container);

	for (auto i : this->buttons) {
		i.second->render(target);
	}

	target.draw(this->menuText);
}