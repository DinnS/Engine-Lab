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
	const sf::VideoMode& videoMode = this->stateData->gfxSettings->resolution;

	this->pauseMenu = new PauseMenu(this->stateData->gfxSettings->resolution, this->font);

	this->pauseMenu->addButton(
		"QUIT", gui::percentToPixelY(75.f, videoMode),
		gui::percentToPixelX(13.f, videoMode), gui::percentToPixelY(6.f, videoMode), 
		gui::calcCharSize(50, videoMode),
		"Quit"
	);
}

void GameState::initShaders()
{
	if (!this->coreShader.loadFromFile("vertex_shader.vert", "fragment_shader.frag")) {
		std::cout << "ERROR::GAMESTATE::COULD NOT LOAD SHADER." << std::endl;
	}
		
}
	

void GameState::initPlayers()
{
	this->player = new Player(0, 0, this->textures);
	
}

void GameState::initPlayerGUI()
{
	this->playerGUI = new PlayerGUI(this->player, this->stateData->gfxSettings->resolution);
}

void GameState::initTileMap()
{
	this->tileNames = {"terrain", "prop", "flora" };
	for (auto& i : this->tileNames) {
		this->tileMaps[i] = new TileMap(this->stateData->gridSize, 12, 12, "Resources/Tiles/" + i + "Sheet.png");

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
	this->initShaders();

	this->initPlayers();
	this->initPlayerGUI();
	this->initTileMap();
}

GameState::~GameState() {
	delete this->pauseMenu;
	delete this->player;
	delete this->playerGUI;

	for (auto& i : this->tileMaps) {
		delete i.second;
	}
}


// Functions

void GameState::updateView(const float& dt)
{
	// Default camera view
	/*this->view.setCenter(
		std::floor(this->player->getPosition().x + (this->player->getHitboxComponentWidth() / 2)),
		std::floor(this->player->getPosition().y + (this->player->getHitboxComponentHeight() / 2))
	);*/

	// Dynamic camera view
	this->view.setCenter(
		std::floor(this->player->getPosition().x + (static_cast<float>(this->mousePosWindow.x) - static_cast<float>(this->stateData->gfxSettings->resolution.width / 2)) / 10.f),
		std::floor(this->player->getPosition().y + (static_cast<float>(this->mousePosWindow.y) - static_cast<float>(this->stateData->gfxSettings->resolution.height / 2)) / 10.f)
	);



	// Camera Side Block
	if (this->view.getCenter().x - this->view.getSize().x / 2.f < 0.f) {
		this->view.setCenter(0.f + this->view.getSize().x / 2.f, this->view.getCenter().y);
	}
	else if (this->view.getCenter().x + this->view.getSize().x / 2.f > 3000.f) {
		this->view.setCenter(3000.f - this->view.getSize().x / 2.f, this->view.getCenter().y);
	}

	if (this->view.getCenter().y - this->view.getSize().y / 2.f < 0.f) {
		this->view.setCenter(this->view.getCenter().x, 0.f + this->view.getCenter().y / 2.f);
	}
	else if (this->view.getCenter().y + this->view.getSize().y / 2.f > 3000.f) {
		this->view.setCenter(this->view.getCenter().x, 3000.f - this->view.getCenter().y / 2.f);
	}
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
		if(this->getKeyTime())
			this->player->gainEXP(10);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_DOWN")))) {
		this->player->move(0.f, 1.f, dt);
		if (this->getKeyTime())
			this->player->loseEXP(10);
	}




}

void GameState::updatePlayerGUI(const float& dt)
{
	this->playerGUI->update(dt);

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

		this->updatePlayerGUI(dt);
		
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
		this->tileMaps[i]->render(
			this->renderTexture,
			this->player->getGridPosition(static_cast<int>(this->stateData->gridSize)),
			&this->coreShader,
			this->player->getCenter(),
			true, false
		);
	}
	this->player->render(this->renderTexture, &this->coreShader, true);

	for (auto& i : this->tileNames) {
		this->tileMaps[i]->renderDeferred(this->renderTexture, &this->coreShader, this->player->getCenter());
	}
	

	// Render GUI
	this->renderTexture.setView(this->renderTexture.getDefaultView());
	this->playerGUI->render(this->renderTexture);
	

	if (this->pause) {  // Pause menu render
		//this->renderTexture.setView(this->renderTexture.getDefaultView());
		this->pauseMenu->render(this->renderTexture);
	}

	// FINAL RENDER
	this->renderTexture.display();
	//this->renderSprite.setTexture(this->renderTexture.getTexture());
	target->draw(this->renderSprite);
}
	