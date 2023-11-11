#include "stdafx.h"
#include "SettingsState.h"

// Initializer functions

void SettingsState::initVariables()
{
	this->modes = sf::VideoMode::getFullscreenModes();
}

void SettingsState::initBackground()
{
	this->background.setSize(
		sf::Vector2f(
			static_cast<float>(this->window->getSize().x),
			static_cast<float>(this->window->getSize().y)
		)
	);

	if (!this->backgroundTexture.loadFromFile("Resources/Graphics/MainMenu/background.png")) {
		throw "ERROR::MAIN_MENU_STATE::FAILED_TO_LOAD_BACKGROUND_TEXTURE";
	}

	this->background.setTexture(&this->backgroundTexture);
}

void SettingsState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/coolvetica.ttf")) {
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT ");
	}

}

void SettingsState::initKeybinds()
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

void SettingsState::initGui()
{
	std::vector<std::string> modes_str;
	for (auto& i : this->modes) {
		modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.height));
	}


	this->dropDownLists["RESOLUTION"] = new gui::DropDownList(
		1000.f, 200.f, 250.f, 100.f,
		this->font, modes_str.data(),
		modes_str.size()
	);



	this->buttons["APPLY"] = new gui::Button(
		185.f, 800.f, 250.f, 70.f,
		&this->font, "Apply", 50,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	);

	this->buttons["BACK"] = new gui::Button(
		600.f, 800.f, 250.f, 70.f,
		&this->font, "Back", 50,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	);


}

void SettingsState::initText()
{
	this->optionsText.setFont(this->font);
	this->optionsText.setPosition(sf::Vector2f(185.f, 220.f));
	this->optionsText.setCharacterSize(30);
	this->optionsText.setFillColor(sf::Color(255, 255, 255, 200));

	this->optionsText.setString(
		"Resolution \n\n\nFullscreen \n\n\nnVsync \n\n\nAntialiazing \n\n\n" 
	);
}

// Constructors/Destructors
SettingsState::SettingsState(StateData* state_data)
	: State(state_data)
{
	this->initVariables();
	this->initBackground();
	this->initFonts();
	this->initKeybinds();
	this->initGui();
	this->initText();
}

SettingsState::~SettingsState()
{
	
	for (auto i = this->buttons.begin(); i != this->buttons.end(); ++i) {
		delete i->second;
	}

	
	for (auto i = this->dropDownLists.begin(); i != this->dropDownLists.end(); ++i) {
		delete i->second;
	}


}

// Accessors


// Functions
void SettingsState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE")))) {
		this->endState();
	}
}

void SettingsState::updateGui(const float& dt)
{
	/*Updates all the gui elements and handles their functionality*/
	
	// Buttons
	for (auto &i : this->buttons) {
		i.second->update(this->mousePosWindow);
	}
	
	// Button functionality



	// Apply selected settings
	if (this->buttons["APPLY"]->isPressed()) {
		
		// REMOVE!!!!!!!!!!!!!!!!!!!!
		this->stateData->gfxSettings->resolution = this->modes[this->dropDownLists["RESOLUTION"]->getActiveElementId()];
		this->window->create(this->stateData->gfxSettings->resolution, this->stateData->gfxSettings->title, sf::Style::Default);
	}
	
	// Back to the menu
	if (this->buttons["BACK"]->isPressed()) {
		this->endState();
	}

	// Drop down lists
	for (auto &i : this->dropDownLists) {
		i.second->update(this->mousePosWindow, dt);
	}

	// Drop down lists functionality



}

void SettingsState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateInput(dt);
	this->updateGui(dt);


	/*
	system("cls");


	std::cout << "mousePosScreen : " << this->mousePosScreen.x << "  " << this->mousePosScreen.y << std::endl;
	std::cout << "mousePosWindow : " << this->mousePosWindow.x << "  " << this->mousePosWindow.y << std::endl;
	std::cout << "mousePosView : " << this->mousePosView.x << "  " << this->mousePosView.y << std::endl;
	*/
}

void SettingsState::renderGui(sf::RenderTarget& target)
{
	for (auto &i : this->buttons) {
		i.second->render(target);
	}


	for (auto& i : this->dropDownLists) {
		i.second->render(target);
	}

}

void SettingsState::render(sf::RenderTarget* target)
{
	if (!target) {
		target = this->window;
	}

	target->draw(this->background);
	this->renderGui(*target);

	target->draw(this->optionsText);

	// Debug cursor
	sf::Text mouseText;
	mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
	mouseText.setFont(this->font);
	mouseText.setCharacterSize(12);
	std::stringstream ss;
	ss << this->mousePosView.x << " | " << this->mousePosView.y;
	mouseText.setString(ss.str());
	target->draw(mouseText);
	
}
