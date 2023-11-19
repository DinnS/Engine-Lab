#include "stdafx.h"
#include "GameState.h"



// Initializer functions 

void GameState::initDeferredRender()
{
	this->renderTexture.create(
		this->stateData->gfxSettings->resolution.width,
		this->stateData->gfxSettings->resolution.height
	);

	this->renderSprite.setTexture(this->renderTexture.getTexture());
	this->renderSprite.setTextureRect(
		sf::IntRect(
			0, 0, 
			this->stateData->gfxSettings->resolution.width,
			this->stateData->gfxSettings->resolution.height
		)
	);
}

void GameState::initView()
{
	this->view.setSize(
		sf::Vector2f(
			static_cast<float>(this->stateData->gfxSettings->resolution.width),
			static_cast<float>(this->stateData->gfxSettings->resolution.height)
		)
	);

	this->view.setCenter(
		static_cast<float>(this->stateData->gfxSettings->resolution.width) / 2.f,
		static_cast<float>(this->stateData->gfxSettings->resolution.height) / 2.f
	);
}

void GameState::initKeybinds()
{
	std::ifstream ifs("Config/gamestate_keybinds.ini");

	if (ifs.is_open()) {
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2) {
			this->keybinds[key] = this->supportedKeys->at(key2);
		}
	}

	ifs.close();

}


void GameState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/EBGaramond-Medium.ttf")) {
		throw("ERROR::GameState::COULD NOT LOAD FONT ");
	}

}

void GameState::initTextures()
{
	// Maria sprites
	this->textures["IDLE"] = new sf::Texture();
	if (!this->textures["IDLE"]->loadFromFile("Resources/Sprites/Tribe/idle/idle.png")) {
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_TRIBE_IDLE";
	}
	
	this->textures["MOVING"] = new sf::Texture();
	if (!this->textures["MOVING"]->loadFromFile("Resources/Sprites/Tribe/run/run.png")) {
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_TRIBE_MOVING";
	}

	this->textures["ATTACK"] = new sf::Texture();
	if (!this->textures["ATTACK"]->loadFromFile("Resources/Sprites/Tribe/attack/attack.png")) {
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_TRIBE_ATTACK";
	}

	
}

void GameState::initPauseMenu()
{
	this->pauseMenu = new PauseMenu(*this->window, this->font);

	this->pauseMenu->addButton("QUIT", 700.f, "Quit");
}
	

void GameState::initPlayers()
{
	this->player = new Player(0, 0, this->textures);
	
}

void GameState::initTileMap()
{
	this->tileNames = {"terrain", "prop", "flora" };
	for (auto& i : this->tileNames) {
		this->tileMaps[i] = new TileMap(this->stateData->gridSize, 10, 10, "Resources/Tiles/" + i + "Sheet.png");

		this->tileMaps[i]->loadFromFile("Data/" + i + "Map.slmp");
	}
	
}



// Constructors / Destructors
GameState::GameState(StateData* state_data) 
	: State(state_data) {
	this->initDeferredRender();
	this->initView();
	this->initKeybinds();
	this->initFonts();
	this->initTextures();
	this->initPauseMenu();

	this->initPlayers();
	this->initTileMap();
}

GameState::~GameState() {
	delete this->pauseMenu;
	delete this->player;

	for (auto& i : this->tileMaps) {
		delete i.second;
	}
}


// Functions

void GameState::updateView(const float& dt)
{
	this->view.setCenter(std::floor(this->player->getPosition().x + (this->player->getHitboxComponentWidth() / 2)), std::floor(this->player->getPosition().y + (this->player->getHitboxComponentHeight() / 2)));
}


void GameState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeyTime()) {

		if (!this->pause) {
			this->pauseState();
		}
		else {
			this->unpauseState();
		}
	}
}

void GameState::updatePlayerInput(const float& dt)
{

	// Update player input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_LEFT")))) {
		this->player->move(-1.f, 0.f, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_RIGHT")))) {
		this->player->move(1.f, 0.f, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_UP")))) {
		this->player->move(0.f, -1.f, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_DOWN")))) {
		this->player->move(0.f, 1.f, dt);
	}


}

void GameState::updatePauseMenuButtons()
{
	if (this->pauseMenu->isButtonPressed("QUIT")) {
		this->endState();
	}
}

void GameState::updateTileMap(const float& dt)
{
	for (auto& i : this->tileNames) {
		this->tileMaps[i]->update();
		this->tileMaps[i]->updateCollision(this->player, dt);
	}
}

void GameState::update(const float& dt)
{
	this->updateMousePositions(&this->view);
	this->updateKeyTime(dt);
	this->updateInput(dt);
	

	if (!this->pause) {  // Unpause update
		this->updateView(dt);

		this->updatePlayerInput(dt);  

		this->updateTileMap(dt);

		this->player->update(dt);
		
	}
	else {              // Pause update
		this->pauseMenu->update(this->mousePosWindow);
		this->updatePauseMenuButtons();
	}



}

void GameState::render(sf::RenderTarget* target)
{
	if (!target) {
		target = this->window;
	}

	this->renderTexture.clear();

	this->renderTexture.setView(this->view);
	for (auto& i : this->tileNames) {
		this->tileMaps[i]->render(this->renderTexture, this->player->getGridPosition(static_cast<int>(this->stateData->gridSize)));
	}
	this->player->render(this->renderTexture);
	
	if (this->pause) {  // Pause menu render
		this->renderTexture.setView(this->renderTexture.getDefaultView());
		this->pauseMenu->render(this->renderTexture);
	}

	// FINAL RENDER
	this->renderTexture.display();
	this->renderSprite.setTexture(this->renderTexture.getTexture());
	target->draw(this->renderSprite);
}
	