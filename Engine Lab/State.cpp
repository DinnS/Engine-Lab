#include "stdafx.h"
#include "State.h"


// Constructors/Destructors

State::State(StateData* state_data)
{
	this->stateData = state_data;
	this->window = state_data->window;
	this->supportedKeys = state_data->supportedKeys;
	this->states = state_data->states;
	this->pause = false;
	this->quit = false;
	this->keyTime = 0.f;
	this->keyTimeMax = 20.f;
	this->gridSize = state_data->gridSize;
}

State::~State()
{
}

// Accessor

const bool& State::getQuit() const
{
	return this->quit;
}

const bool State::getKeyTime()
{
	if (this->keyTime >= this->keyTimeMax) {
		this->keyTime = 0.f;
		return true;
	}
	return false;
}



// Functions


void State::pauseState()
{
	this->pause = true;
}

void State::unpauseState()
{
	this->pause = false;
}



void State::endState()
{
	this->quit = true;
}


void State::updateMousePositions(sf::View* view)
{
	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);

	if(view) 
		this->window->setView(*view);
	this->mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));
	
	this->mousePosGrid = sf::Vector2i(
		static_cast<int>(this->mousePosView.x) / static_cast<int>(this->gridSize),
		static_cast<int>(this->mousePosView.y) / static_cast<int>(this->gridSize)
	);

	this->window->setView(this->window->getDefaultView());
}

void State::updateKeyTime(const float& dt)
{
	if (this->keyTime < this->keyTimeMax) {
		this->keyTime += 60.f * dt;
	}
}
