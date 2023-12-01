#include "stdafx.h"
#include "MainMenuState.h"

// Initializer functions

void MainMenuState::initVariables()
{
}

void MainMenuState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/EBGaramond-Medium.ttf")) {
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT ");
	}

}

void MainMenuState::initKeybinds()
{
	std::ifstream ifs("Config/mainmenustate_keybinds.ini");

	if (ifs.is_open()) {
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2) {
			this->keybinds[key] = this->supportedKeys->at(key2);
		}
	}

	ifs.close();

}

void MainMenuState::initGui()
{
	const sf::VideoMode& videoMode = this->stateData->gfxSettings->resolution;

	// Background
	this->background.setSize(
		sf::Vector2f(
			static_cast<float>(videoMode.width),
			static_cast<float>(videoMode.height)
		)
	);


	if (!this->backgroundTexture.loadFromFile("Resources/Graphics/MainMenu/background.png")) {
		throw "ERROR::MAIN_MENU_STATE::FAILED_TO_LOAD_BACKGROUND_TEXTURE";
	}

	this->background.setTexture(&this->backgroundTexture);

	// Button background
	this->buttonBackground.setSize(
		sf::Vector2f(
			static_cast<float>(videoMode.width / 5),
			static_cast<float>(videoMode.height)
		)
	);

	this->buttonBackground.setPosition(gui::percentToPixelX(6.5f, videoMode), 0.f);
	this->buttonBackground.setFillColor(sf::Color(10, 10, 10, 220));

	// Buttons
	this->buttons["GAME_STATE"] = new gui::Button(
		gui::percentToPixelX(10.f, videoMode), gui::percentToPixelY(30.f, videoMode),
		gui::percentToPixelX(13.f, videoMode), gui::percentToPixelY(6.f, videoMode),
		&this->font, "New Game", gui::calcCharSize(60, videoMode),
		sf::Color(200, 200, 200, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	); 

	this->buttons["SETTINGS_STATE"] = new gui::Button(
		gui::percentToPixelX(10.f, videoMode), gui::percentToPixelY(43.f, videoMode),
		gui::percentToPixelX(13.f, videoMode), gui::percentToPixelY(6.f, videoMode),
		&this->font, "Settings", gui::calcCharSize(60, videoMode),
		sf::Color(200, 200, 200, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	);

	this->buttons["EDITOR_STATE"] = new gui::Button(
		gui::percentToPixelX(10.f, videoMode), gui::percentToPixelY(56.f, videoMode),
		gui::percentToPixelX(13.f, videoMode), gui::percentToPixelY(6.f, videoMode),
		&this->font, "Editor", gui::calcCharSize(60, videoMode),
		sf::Color(200, 200, 200, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	);

	this->buttons["EXIT_STATE"] = new gui::Button(
		gui::percentToPixelX(10.f, videoMode), gui::percentToPixelY(75.f, videoMode),
		gui::percentToPixelX(13.f, videoMode), gui::percentToPixelY(6.f, videoMode),
		&this->font, "Quit", gui::calcCharSize(60, videoMode),
		sf::Color(200, 200, 200, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(100, 100, 100, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	);
}

void MainMenuState::resetGui()
{
	/*
	*  Clear GUI elements and re-initializes the GUI
	*
	*  @return		void
	*/

	auto i = this->buttons.begin();
	for (i = this->buttons.begin(); i != this->buttons.end(); ++i) {
		delete i->second;
	}

	this->initGui();
}

// Constructors/Destructors

MainMenuState::MainMenuState(StateData* state_data) 
	: State(state_data) {
	this->initVariables();
	this->initFonts();
	this->initKeybinds();
	this->initGui();
	this->resetGui();
}

MainMenuState::~MainMenuState() {
	auto i = this->buttons.begin();
	for (i = this->buttons.begin(); i != this->buttons.end(); ++i) {
		delete i->second;
	}
}


// Functions

void MainMenuState::updateInput(const float& dt)
{
	
}

void MainMenuState::updateButtons()
{
	/*Updates all the button and handles their functionality*/

	for (auto i : this->buttons) {
		i.second->update(this->mousePosWindow);
	}

	// New game
	if (this->buttons["GAME_STATE"]->isPressed()) {
		this->states->push(new GameState(this->stateData));
	}

	// Settings
	if (this->buttons["SETTINGS_STATE"]->isPressed()) {
		this->states->push(new SettingsState(this->stateData));
	}

	// Editor
	if (this->buttons["EDITOR_STATE"]->isPressed()) {
		this->states->push(new EditorState(this->stateData));
	}

	// Quit the game
	if (this->buttons["EXIT_STATE"]->isPressed()) {
		this->endState();
	}
}

void MainMenuState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateInput(dt);
	this->updateButtons();
	

	
	/*
	system("cls");

	
	std::cout << "mousePosScreen : " << this->mousePosScreen.x << "  " << this->mousePosScreen.y << std::endl;
	std::cout << "mousePosWindow : " << this->mousePosWindow.x << "  " << this->mousePosWindow.y << std::endl;
	std::cout << "mousePosView : " << this->mousePosView.x << "  " << this->mousePosView.y << std::endl;
	*/
}

void MainMenuState::renderButtons(sf::RenderTarget& target)
{
	for (auto i : this->buttons) {
		i.second->render(target);
	}
	
}

void MainMenuState::render(sf::RenderTarget* target)
{
	if (!target) {
		target = this->window;
	}

	target->draw(this->background);
	target->draw(this->buttonBackground);
	this->renderButtons(*target);
	 

	// Debug cursor
	//sf::Text mouseText;
	//mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
	//mouseText.setFont(this->font);
	//mouseText.setCharacterSize(12);
	//std::stringstream ss;
	//ss << this->mousePosView.x << " | " << this->mousePosView.y;
	//mouseText.setString(ss.str());
	//target->draw(mouseText);
}
