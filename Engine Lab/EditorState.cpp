#include "stdafx.h"
#include "EditorState.h"

// Initializer functions


void EditorState::initVariables()
{
	this->textureRect = sf::IntRect(0, 0, static_cast<int>(this->stateData->gridSize), static_cast<int>(this->stateData->gridSize));
	this->collision = false;
	this->type = TileTypes::DEFAULT;
	this->cameraSpeed = 300.f;
	this->layer = 0;
}

void EditorState::initView()
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



void EditorState::initBackground()
{

}

void EditorState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/coolvetica.ttf")) {
		throw("ERROR::EDITORSTATE::COULD NOT LOAD FONT ");
	}

}

void EditorState::initTexts()
{
	this->cursorText.setFont(this->font);
	this->cursorText.setFillColor(sf::Color::White);
	this->cursorText.setCharacterSize(12);
	this->cursorText.setPosition(this->mousePosView.x, this->mousePosView.y);
	
}

void EditorState::initKeybinds()
{
	std::ifstream ifs("Config/editorstate_keybinds.ini");

	if (ifs.is_open()) {
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2) {
			this->keybinds[key] = this->supportedKeys->at(key2);
		}
	}

	ifs.close();

}

void EditorState::initPauseMenu()
{
	this->pauseMenu = new PauseMenu(*this->window, this->font);

	this->pauseMenu->addButton("LOAD", 400.f, "Load");
	this->pauseMenu->addButton("SAVE", 550.f, "Save");
	this->pauseMenu->addButton("QUIT", 700.f, "Quit");
}

void EditorState::initButtons()
{
	this->buttons["TILE_SHEET"] = new gui::Button(
		10.f, 30.f, 50.f, 80.f,
		&this->font, "TS", 26,
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 250), sf::Color(255, 255, 255, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50)
	);

	this->buttons["PROPS_SHEET"] = new gui::Button(
		10.f, 250.f, 50.f, 80.f,
		&this->font, "PS", 26,
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 250), sf::Color(255, 255, 255, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50)
	);


}

void EditorState::initTileMap()
{
	this->tileMap = new TileMap(this->stateData->gridSize, 10, 10, "Resources/Tiles/tileSheet.png");
	this->propsMap = new TileMap(this->stateData->gridSize, 10, 10, "Resources/Tiles/propsSheet.png");
}


void EditorState::initGui()
{
	// Side bar
	this->sidebar.setSize(sf::Vector2f(80.f, static_cast<float>(this->stateData->gfxSettings->resolution.height)));
	this->sidebar.setFillColor(sf::Color(50, 50, 50, 100));
	this->sidebar.setOutlineThickness(1.f);
	this->sidebar.setOutlineColor(sf::Color(100, 100, 100, 150));

	
	// Selector 
	this->selectorRect.setSize(sf::Vector2f(this->stateData->gridSize, this->stateData->gridSize));

	this->selectorRect.setFillColor(sf::Color(255, 255, 255, 150));
	this->selectorRect.setOutlineThickness(1.f);
	this->selectorRect.setOutlineColor(sf::Color::Green);

	this->selectorRect.setTexture(this->tileMap->getTileTextureSheet());
	this->selectorRect.setTextureRect(this->textureRect);



	// Texture selector
	this->textureSelector = new gui::TextureSelector(
		100.f, 20.f, 512.f, 512.f, 
		this->stateData->gridSize, this->tileMap->getTileTextureSheet()
	);

	this->textureSelectorProps = new gui::TextureSelector(
		100.f, 20.f, 512.f, 512.f,
		this->stateData->gridSize, this->propsMap->getTileTextureSheet()
	);




}


// Constructors/Destructors

EditorState::EditorState(StateData* state_data) 
	: State(state_data) 
{
	this->initVariables();
	this->initView();
	this->initBackground();
	this->initFonts();
	this->initTexts();
	this->initKeybinds();
	this->initPauseMenu();
	this->initButtons();
	this->initTileMap();
	this->initGui();
	
}

EditorState::~EditorState() {
	auto i = this->buttons.begin();
	for (i = this->buttons.begin(); i != this->buttons.end(); ++i) {
		delete i->second;
	}

	delete this->pauseMenu;

	delete this->tileMap;
	delete this->propsMap;

	delete this->textureSelector;
	delete this->textureSelectorProps;

}



//Functions

void EditorState::updateInput(const float& dt)
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

void EditorState::updateEditorInput(const float& dt)
{
	// Move View
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_UP")))) {
		this->view.move(0.f, -this->cameraSpeed * dt);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_DOWN")))) {
		this->view.move(0.f, this->cameraSpeed * dt);
	} 

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_RIGHT")))) {
		this->view.move(this->cameraSpeed * dt, 0.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_LEFT")))) {
		this->view.move(-this->cameraSpeed * dt, 0.f);
	}

	// Add tile to the tilemap
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->getKeyTime()) 
	{
		if (!this->sidebar.getGlobalBounds().contains(sf::Vector2f(this->mousePosWindow))) {
			if (!textureSelector->getHidden()) {
				if (!this->textureSelector->getActive())
				{
					this->tileMap->addTile(this->mousePosGrid.x, this->mousePosGrid.y, 0, this->textureRect, this->collision, this->type);
				}
				else
				{
					this->selectorRect.setTexture(this->tileMap->getTileTextureSheet());
					this->textureRect = this->textureSelector->getTextureSelectorRect();
				}
			}
			else if (!textureSelectorProps->getHidden()) {
				if (!this->textureSelectorProps->getActive())
				{
					this->propsMap->addTile(this->mousePosGrid.x, this->mousePosGrid.y, 0, this->textureRect, this->collision, this->type);
				}
				else
				{
					this->selectorRect.setTexture(this->propsMap->getTileTextureSheet());
					this->textureRect = this->textureSelectorProps->getTextureSelectorRect();
				}
			}
		}


		
	}
	// Remove a tile from the tilemap
	else if((sf::Mouse::isButtonPressed(sf::Mouse::Right) && this->getKeyTime()))  
	{
		if (!this->sidebar.getGlobalBounds().contains(sf::Vector2f(this->mousePosWindow))) {
			if (!this->textureSelector->getActive() && !textureSelector->getHidden())
			{
				this->tileMap->removeTile(this->mousePosGrid.x, this->mousePosGrid.y, 0);
			}
			else if (!this->textureSelectorProps->getActive() && !textureSelectorProps->getHidden()) {
				this->propsMap->removeTile(this->mousePosGrid.x, this->mousePosGrid.y, 0);
			}
		}
		

		
	}

	// Toggle collision
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("COLLISION_TOGGLE"))) && this->getKeyTime()) {
		if (this->collision) {
			this->collision = false;
		}
		else {
			this->collision = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("TYPE_INCREASE"))) && this->getKeyTime()) {
		// CHANGE TO LIMIT ADD
		this->type++;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("TYPE_DECREASE"))) && this->getKeyTime()) {
		if (this->type > 0) {
			this->type--;
		}
	}
	
	
	
}

void EditorState::updateButtons()
{
	/*Updates all the button and handles their functionality*/

	for (auto &i : this->buttons) {
		i.second->update(this->mousePosWindow);
	}

	// Open texture tile sheet
	if (this->buttons["TILE_SHEET"]->isPressed() && this->getKeyTime()) {
		this->textureSelector->toggleTextureSelector();

		// Close other texture sheets
		if (!this->textureSelectorProps->getHidden()) {
			this->textureSelectorProps->toggleTextureSelector();
		}
	}

	// Open texture props sheet
	if (this->buttons["PROPS_SHEET"]->isPressed() && this->getKeyTime()) {
		this->textureSelectorProps->toggleTextureSelector();

		// Close other texture sheets
		if (!this->textureSelector->getHidden()) {
			this->textureSelector->toggleTextureSelector();
		}
	}
	

}

void EditorState::updateGui(const float& dt)
{
	this->textureSelector->update(this->mousePosWindow, dt);
	this->textureSelectorProps->update(this->mousePosWindow, dt);

	
	if (!this->textureSelector->getActive()) {
		this->selectorRect.setTextureRect(this->textureRect);
		this->selectorRect.setPosition(this->mousePosGrid.x * this->stateData->gridSize, this->mousePosGrid.y * this->stateData->gridSize);
	}

	this->cursorText.setPosition(this->mousePosView.x + 100.f, this->mousePosView.y - 50.f);
	std::stringstream ss;
	ss << this->mousePosView.x << " | " << this->mousePosView.y << "\n" <<
		this->mousePosGrid.x << " | " << this->mousePosGrid.y << "\n" <<
		this->textureRect.left << " | " << this->textureRect.top << "\n" <<
		"Collision: " << this->collision << "\n" <<
		"Types: " << this->type << "\n" <<
		"Tiles: " << this->tileMap->getLayerSize(this->mousePosGrid.x, this->mousePosGrid.y, this->layer) << "\n" <<
		"Props: " << this->propsMap->getLayerSize(this->mousePosGrid.x, this->mousePosGrid.y, this->layer);
	this->cursorText.setString(ss.str());
	


}


void EditorState::updatePauseMenuButtons()
{
	if (this->pauseMenu->isButtonPressed("LOAD")) {
		this->tileMap->loadFromFile("Data/tileMap.slmp");
		this->propsMap->loadFromFile("Data/propsMap.slmp");
	}

	if (this->pauseMenu->isButtonPressed("SAVE")) {
		this->tileMap->saveToFile("Data/tileMap.slmp");
		this->propsMap->saveToFile("Data/propsMap.slmp");
		
	}

	if (this->pauseMenu->isButtonPressed("QUIT")) {
		this->endState();
	}
}

void EditorState::update(const float& dt)
{
	this->updateMousePositions(&this->view);
	this->updateKeyTime(dt);
	this->updateInput(dt);
	

	if (!this->pause) {  // Unpause update
		this->updateButtons();
		this->updateGui(dt);
		this->updateEditorInput(dt);
	}
	else {              // Pause update
		this->pauseMenu->update(this->mousePosWindow);
		this->updatePauseMenuButtons();
	}
	// DEBUG MOUSE
	/* 
	system("cls");


	std::cout << "mousePosScreen : " << this->mousePosScreen.x << "  " << this->mousePosScreen.y << std::endl;
	std::cout << "mousePosWindow : " << this->mousePosWindow.x << "  " << this->mousePosWindow.y << std::endl;
	std::cout << "mousePosView : " << this->mousePosView.x << "  " << this->mousePosView.y << std::endl;
	*/
}

void EditorState::renderButtons(sf::RenderTarget& target)
{
	for (auto i : this->buttons) {
		i.second->render(target);
	}

}

void EditorState::renderGui(sf::RenderTarget& target)
{
	if (!this->textureSelector->getActive()) { 
		target.setView(this->view);
		target.draw(this->selectorRect);
	}

	// 
	if (!this->textureSelectorProps->getActive()) {
		target.setView(this->view);
		target.draw(this->selectorRect);
	}

	//
	
	target.setView(this->window->getDefaultView());

	this->textureSelector->render(target);

	//
	this->textureSelectorProps->render(target);
	//

	target.draw(this->sidebar);


	target.setView(this->view);
	target.draw(this->cursorText);


	
}

void EditorState::render(sf::RenderTarget* target)
{
	if (!target) {
		target = this->window;
	}
	
	target->setView(this->view);
	
	this->tileMap->render(*target, this->mousePosGrid);
	//
	this->propsMap->render(*target, this->mousePosGrid);
	//

	target->setView(this->window->getDefaultView());
	this->renderButtons(*target);

	this->renderGui(*target);

	if (this->pause) {  // Pause menu render
		target->setView(this->window->getDefaultView());
		this->pauseMenu->render(*target);
	}
	
	
}

