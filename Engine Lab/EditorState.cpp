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
	this->buttons["TERRAIN_SHEET"] = new gui::Button(
		10.f, 30.f, 50.f, 80.f,
		&this->font, "TS", 26,
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 250), sf::Color(255, 255, 255, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50)
	);

	this->buttons["PROP_SHEET"] = new gui::Button(
		10.f, 250.f, 50.f, 80.f,
		&this->font, "PS", 26,
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 250), sf::Color(255, 255, 255, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50)
	);

	this->buttons["FLORA_SHEET"] = new gui::Button(
		10.f, 470.f, 50.f, 80.f,
		&this->font, "FS", 26,
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 250), sf::Color(255, 255, 255, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50)
	);

}

void EditorState::initTileMap()
{
	this->tileNames = {"terrain", "prop", "flora"};
	this->lastUsedTileMaps = "";
	for (auto& i : this->tileNames) {
		this->tileMaps[i] = new TileMap(this->stateData->gridSize, 10, 10, "Resources/Tiles/" + i + "Sheet.png");
	}
	
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

	this->selectorRect.setTexture(this->tileMaps[this->tileNames[0]]->getTileTextureSheet());
	this->selectorRect.setTextureRect(this->textureRect);



	// Texture selector
	for (auto& i : this->tileNames) {
		this->texturesSelector[i] = new gui::TextureSelector(
			100.f, 20.f, 512.f, 512.f,
			this->stateData->gridSize, this->tileMaps[i]->getTileTextureSheet()
		);
	}



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

	for (auto& i : this->tileMaps) {
		delete i.second;
	}

	for (auto& i : this->texturesSelector) {
		delete i.second;
	}

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
			for (auto& i : this->tileNames) {
				if (this->lastUsedTileMaps == i) {
					if (!this->texturesSelector[i]->getActive())
					{
						this->tileMaps[i]->addTile(this->mousePosGrid.x, this->mousePosGrid.y, 0, this->textureRect, this->collision, this->type);

					}
					else
					{
						this->selectorRect.setTexture(this->tileMaps[i]->getTileTextureSheet());
						this->textureRect = this->texturesSelector[i]->getTextureSelectorRect();
					}
				}
			}
		}


		
	}
	// Remove a tile from the tilemap
	else if((sf::Mouse::isButtonPressed(sf::Mouse::Right) && this->getKeyTime()))  
	{
		if (!this->sidebar.getGlobalBounds().contains(sf::Vector2f(this->mousePosWindow))) {
		
			for (auto& i : this->tileNames) {
				if (!this->texturesSelector[i]->getActive() && this->lastUsedTileMaps == i)
				{
					this->tileMaps[i]->removeTile(this->mousePosGrid.x, this->mousePosGrid.y, 0);
				}
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
	if (this->buttons["TERRAIN_SHEET"]->isPressed() && this->getKeyTime()) {
		this->texturesSelector[this->tileNames[0]]->toggleTextureSelector();

		// Close other texture sheets
		if (!this->texturesSelector[this->tileNames[1]]->getHidden()) {
			this->texturesSelector[this->tileNames[1]]->toggleTextureSelector();
		}

		if (!this->texturesSelector[this->tileNames[2]]->getHidden()) {
			this->texturesSelector[this->tileNames[2]]->toggleTextureSelector();
		}

		// Save lastTileMap
		this->lastUsedTileMaps = "terrain";
	}

	// Open texture props sheet
	if (this->buttons["PROP_SHEET"]->isPressed() && this->getKeyTime()) {
		this->texturesSelector[this->tileNames[1]]->toggleTextureSelector();

		// Close other texture sheets
		if (!this->texturesSelector[this->tileNames[0]]->getHidden()) {
			this->texturesSelector[this->tileNames[0]]->toggleTextureSelector();
		}

		if (!this->texturesSelector[this->tileNames[2]]->getHidden()) {
			this->texturesSelector[this->tileNames[2]]->toggleTextureSelector();
		}

		// Save lastTileMap
		this->lastUsedTileMaps = "prop";
	}

	// Open texture green sheet
	if (this->buttons["FLORA_SHEET"]->isPressed() && this->getKeyTime()) {
		this->texturesSelector[this->tileNames[2]]->toggleTextureSelector();

		// Close other texture sheets
		if (!this->texturesSelector[this->tileNames[0]]->getHidden()) {
			this->texturesSelector[this->tileNames[0]]->toggleTextureSelector();
		}

		if (!this->texturesSelector[this->tileNames[1]]->getHidden()) {
			this->texturesSelector[this->tileNames[1]]->toggleTextureSelector();
		}

		// Save lastTileMap
		this->lastUsedTileMaps = "flora";
	}
	

}

void EditorState::updateGui(const float& dt)
{

	for (auto& i : this->tileNames) {
		this->texturesSelector[i]->update(this->mousePosWindow, dt);
	}
	
	if (!this->texturesSelector[this->tileNames[0]]->getActive()) {
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
		"Terrain: " << this->tileMaps[tileNames[0]]->getLayerSize(this->mousePosGrid.x, this->mousePosGrid.y, this->layer) << "\n" <<
		"Prop: " << this->tileMaps[tileNames[1]]->getLayerSize(this->mousePosGrid.x, this->mousePosGrid.y, this->layer) << "\n" <<
		"Flora : " << this->tileMaps[tileNames[2]]->getLayerSize(this->mousePosGrid.x, this->mousePosGrid.y, this->layer);
	this->cursorText.setString(ss.str());
	


}


void EditorState::updatePauseMenuButtons()
{

	if (this->pauseMenu->isButtonPressed("LOAD")) {
		for (auto& i : this->tileNames) {
			this->tileMaps[i]->loadFromFile("Data/" + i + "Map.slmp");
		}
	}


	if (this->pauseMenu->isButtonPressed("SAVE")) {
		for (auto& i : this->tileNames) {
			this->tileMaps[i]->saveToFile("Data/" + i + "Map.slmp");
		}
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
	
	for (auto& i : this->tileNames) {
		if (!this->texturesSelector[i]->getActive()) {
			target.setView(this->view);
			target.draw(this->selectorRect);
		}
	}
	
	target.setView(this->window->getDefaultView());


	for (auto& i : this->tileNames) {
		this->texturesSelector[i]->render(target);
	}

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
	

	for (auto& i : this->tileNames) {
		this->tileMaps[i]->render(*target, this->mousePosGrid);
	}

	target->setView(this->window->getDefaultView());
	this->renderButtons(*target);

	this->renderGui(*target);

	if (this->pause) {  // Pause menu render
		target->setView(this->window->getDefaultView());
		this->pauseMenu->render(*target);
	}
	
	
}

