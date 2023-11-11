#include "stdafx.h"
#include "Game.h"

// Static functions


// Initialize functions

void Game::initVariables()
{
	this->window = nullptr;
	
	this->dt = 0.f;

	this->gridSize = 64.f;
}

void Game::initGraphicsSettings()
{
	this->gfxSettings.loadFromFile("Config/graphics.ini");

}


void Game::initWindow()
{
	// Create a SFML window


	if (this->gfxSettings.fullscreen) {
		this->window = new sf::RenderWindow(
			this->gfxSettings.resolution, 
			this->gfxSettings.title, 
			sf::Style::Fullscreen, 
			this->gfxSettings.contextSettings
		);
	}
	else {
		this->window = new sf::RenderWindow(
			this->gfxSettings.resolution,
			this->gfxSettings.title,
			sf::Style::Titlebar | sf::Style::Close , 
			this->gfxSettings.contextSettings
		);
	}
	
	this->window->setFramerateLimit(this->gfxSettings.frameRateLimit);
	this->window->setVerticalSyncEnabled(this->gfxSettings.vSync);
}

void Game::initKeys()
{
	std::ifstream ifs("Config/supportedKeys.ini");

	if (ifs.is_open()) {
		std::string key = "";
		int key_value = 0;

		while (ifs >> key >> key_value) {
			this->supportedKeys[key] = key_value;
		}
	}
	

	ifs.close();

	/*this->supportedKeys["Escape"] = sf::Keyboard::Key::Escape;
	this->supportedKeys["A"] = sf::Keyboard::Key::A;
	this->supportedKeys["D"] = sf::Keyboard::Key::D;
	this->supportedKeys["W"] = sf::Keyboard::Key::W;
	this->supportedKeys["S"] = sf::Keyboard::Key::S;*/


	// Debug
	/*for (auto i : this->supportedKeys) {
		std::cout << i.first << " " << i.second << std::endl;
	}*/
}


void Game::initStateData()
{
	this->stateData.window = this->window;
	this->stateData.gfxSettings = &this->gfxSettings;
	this->stateData.supportedKeys = &this->supportedKeys;
	this->stateData.states = &this->states;
	this->stateData.gridSize = this->gridSize;
}


void Game::initStates()
{
	this->states.push(new MainMenuState(&this->stateData));
}



// Constructors/Destructors
Game::Game()
{
	this->initVariables();
	this->initGraphicsSettings();
	this->initWindow();
	this->initKeys();
	this->initStateData();
	this->initStates();
	
}

Game::~Game()
{
	delete this->window;

	while (!this->states.empty()) {
		delete this->states.top();
		this->states.pop();
	}
}


// Functions

// Regular
void Game::endApplication()
{
	std::cout << "Ending applicantion" << std::endl;
}


// Update
void Game::updateDt()
{
	// Updates dt variable to render one frame
	this->dt = this->dtClock.restart().asSeconds();
}

void Game::updateSfEvents()
{
	while (this->window->pollEvent(this->sfEvent)) {
		if (this->sfEvent.type == sf::Event::Closed) {
			this->window->close();
		}
	}
}

void Game::update()
{
	this->updateSfEvents();

	if (!this->states.empty()) {

		if (this->window->hasFocus()) {
			this->states.top()->update(this->dt);

			if (this->states.top()->getQuit()) {
				this->states.top()->endState();
				delete this->states.top();
				this->states.pop();
			}
		}
	
	}
	else { //Application end
		this->endApplication();
		this->window->close();
	}
	

}


// Render
void Game::render()
{
	this->window->clear();

	if (!this->states.empty()) {
		this->states.top()->render(this->window);
	}
		

	this->window->display();
}


// Core
void Game::run()
{
	// Game loop
	while (this->window->isOpen()) {
		this->updateDt();
		this->update();
		this->render();
	}
}
