	#include "stdafx.h"
#include "Gui.h"

// Function
const float gui::percentToPixelX(const float percent, const sf::VideoMode& videoMode)
{
	/*
	*  Converts a percentage value to pixels relative to the current resolution in the x-axis.
	*
	*  @param		float percent				The percentage value.
	*  @param		sf::VideoMode& videoMode	The current VideoMode for a window (resolution)
	*
	*  @return		float						The calculated pixel value
	*
	*/

	return std::floor(static_cast<float>(videoMode.width) * (percent / 100.f));
}

const float gui::percentToPixelY(const float percent, const sf::VideoMode& videoMode)
{
	/*
	*  Converts a percentage value to pixels relative to the current resolution in the y-axis.
	*
	*  @param		float percent				The percentage value.
	*  @param		sf::VideoMode& videoMode	The current VideoMode for a window (resolution)
	*
	*  @return		float						The calculated pixel value
	*
	*/


	return std::floor(static_cast<float>(videoMode.height) * (percent / 100.f));
}


const unsigned gui::calcCharSize(const unsigned modifier, const sf::VideoMode& videoMode)
{
	/*
	*  Calculate the character size for text using the current resolution and a constant
	*
	*  @param		int							The value which we divide for getting dynamic size
	*  @param		unsigned modifier			Used to modify the character size
	*
	*  @return		unsigned					The calculated character size value
	*
	*/

	return static_cast<unsigned>(videoMode.width + videoMode.height) / (modifier);
}

// BUTTON =======================================================================

// Constructors/Destructors

gui::Button::Button(float x, float y, float width, float height,
	sf::Font* font, std::string text,unsigned character_size,
	sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
	sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
	sf::Color outline_idle_color, sf::Color outline_hover_color, sf::Color outline_active_color,
	unsigned short id
)
{
	this->buttonState = BTN_IDLE;
	this->id = id;

	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setFillColor(idle_color);	
	this->shape.setOutlineThickness(1.f);
	this->shape.setOutlineColor(outline_idle_color);

	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setFillColor(text_idle_color);
	this->text.setCharacterSize(character_size);
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y 
		// this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 2.f
	);

	this->textIdleColor = text_idle_color;
	this->textHoverColor = text_hover_color;
	this->textActiveColor = text_active_color; 

	this->idleColor = idle_color;
	this->hoverColor = hover_color;
	this->activeColor = active_color;


	this->outlineIdleColor = outline_idle_color;
	this->outlineHoverColor = outline_hover_color;
	this->outlineActiveColor = outline_active_color;

}

gui::Button::~Button()
{
}

// Accessors

const bool gui::Button::isPressed() const
{
	if (this->buttonState == BTN_ACTIVE) {
		return true;
	}
	return false;
}

const std::string gui::Button::getText() const
{
	return this->text.getString();
}

const unsigned short& gui::Button::getId() const
{
	return this->id;
}


// Modifiers
void gui::Button::setText(const std::string text)
{
	this->text.setString(text);
}

void gui::Button::setId(const unsigned short id)
{
	this->id = id;
}


//Functions
void gui::Button::update(const sf::Vector2i& mousePosWindow)
{
	/* Update the booleans for hoverand pressed */
	this->buttonState = BTN_IDLE;

	// Hover
	if (this->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) {
		this->buttonState = BTN_HOVER;

		// Pressed
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			this->buttonState = BTN_ACTIVE;
		}
	}

	switch (this->buttonState) {
	case BTN_IDLE:
		this->shape.setFillColor(this->idleColor);
		this->text.setFillColor(this->textIdleColor);
		this->shape.setOutlineColor(this->outlineIdleColor);
		break;
	case BTN_HOVER:
		this->shape.setFillColor(this->hoverColor);
		this->text.setFillColor(this->textHoverColor);
		this->shape.setOutlineColor(this->outlineHoverColor);
		break;
	case BTN_ACTIVE:
		this->shape.setFillColor(this->activeColor);
		this->text.setFillColor(this->textActiveColor);
		this->shape.setOutlineColor(this->outlineActiveColor);
		break;
	default:
		this->shape.setFillColor(sf::Color::Red);
		this->text.setFillColor(sf::Color::Blue);
		this->shape.setOutlineColor(sf::Color::Green);
		break;
	}

}

void gui::Button::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
	target.draw(this->text);
}



// DROP DOWN LIST ===============================================================

// Constructors/Destructors
gui::DropDownList::DropDownList(
	float x, float y, float width, float height,
	sf::Font& font, std::string list[],
	unsigned numberOfElements, unsigned default_index
)
	: font(font), showList(false), keyTime(0.f), keyTimeMax(2.f)
{


	this->activeElement = new gui::Button(
		x, y, width, height,
		&this->font, list[default_index], 18,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 200)
	);

	for (unsigned int i = 0; i < numberOfElements; i++) {
		this->list.push_back(
			new gui::Button(
				x, y + (height * (i + 1)), width, height,
				&this->font, list[i], 15,
				sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
				sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
				sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(20, 20, 20, 0),
				i
			)
		);
	}


}

gui::DropDownList::~DropDownList()
{
	delete this->activeElement;
	for (unsigned i = 0; i < this->list.size(); i++) {
		delete this->list[i];
	}
}


// Accessors
const bool gui::DropDownList::getKeyTime()
{
	if (this->keyTime >= this->keyTimeMax) {
		this->keyTime = 0.f;
		return true;
	}
	
	return false;
	
}

const unsigned short& gui::DropDownList::getActiveElementId() const
{
	return this->activeElement->getId();
}


// Functions
void gui::DropDownList::updateKeyTime(const float& dt)
{
	if (this->keyTime < this->keyTimeMax) {
		this->keyTime += 10.f * dt;
	}
}


void gui::DropDownList::update(const sf::Vector2i& mousePosWindow, const float& dt)
{
	this->updateKeyTime(dt);

	this->activeElement->update(mousePosWindow);


	// Show or hide list
	if (this->activeElement->isPressed() && this->getKeyTime()) {
		if (!this->showList) {
			this->showList = true;
		}
		else {
			this->showList = false;
		}
	}


	// List interaction
	if (this->showList) {
		for (auto& i : this->list) {
			i->update(mousePosWindow);

			if (i->isPressed() && this->getKeyTime()) {
				this->showList = false;
				this->activeElement->setText(i->getText());
				this->activeElement->setId(i->getId());
			}
		}
	}
	
}

void gui::DropDownList::render(sf::RenderTarget& target)
{
	this->activeElement->render(target);

	if (this->showList) {
		for (auto& i : this->list) {
			i->render(target);
		}
	}
}
 


// TEXTURE SELECTOR ==============================================================


// Constructors/Destructors

gui::TextureSelector::TextureSelector(float x, float y, float width, float height, float gridSize, const sf::Texture* texture_sheet)
{
	// Init variables

	// If cursor in textureSelector
	this->active = false;

	// Hide or show textureSelector
	this->hidden = true;

	this->gridSize = gridSize;

	
	// Bounds settings
	this->bounds.setSize(sf::Vector2f(width, height));
	this->bounds.setPosition(x, y);
	this->bounds.setFillColor(sf::Color(50, 50, 50, 100));
	this->bounds.setOutlineThickness(1.f);
	this->bounds.setOutlineColor(sf::Color(255, 255, 255, 200));


	// SpriteSheet settings
	this->spriteSheet.setTexture(*texture_sheet);
	this->spriteSheet.setPosition(x, y);


	// Check spritesheet size
	if (this->spriteSheet.getGlobalBounds().width > this->bounds.getGlobalBounds().width) 
	{
		this->spriteSheet.setTextureRect(sf::IntRect(0, 0, static_cast<int>(this->bounds.getGlobalBounds().width), static_cast<int>(this->spriteSheet.getGlobalBounds().height)));
	}

	if (this->spriteSheet.getGlobalBounds().height > this->bounds.getGlobalBounds().height) 
	{
		this->spriteSheet.setTextureRect(sf::IntRect(0, 0, static_cast<int>(this->spriteSheet.getGlobalBounds().width), static_cast<int>(this->bounds.getGlobalBounds().height)));
	}

	// Selector settings
	this->selector.setPosition(x, y);
	this->selector.setSize(sf::Vector2f(this->gridSize, this->gridSize));
	this->selector.setFillColor(sf::Color::Transparent);
	this->selector.setOutlineThickness(1.f);
	this->selector.setOutlineColor(sf::Color::Red);

	// Texture Rect
	this->textureSelectorRect.width = static_cast<int>(gridSize);
	this->textureSelectorRect.height = static_cast<int>(gridSize);
}

gui::TextureSelector::~TextureSelector()
{
	
}


// Accessors

const bool& gui::TextureSelector::getActive() const
{
	return this->active;
}

const bool& gui::TextureSelector::getHidden() const
{
	return this->hidden;
}

const sf::IntRect& gui::TextureSelector::getTextureSelectorRect() const
{
	return this->textureSelectorRect;
}



// Functions


void gui::TextureSelector::toggleTextureSelector() {
	if (this->hidden) {
		this->hidden = false;
	}
	else {
		this->hidden = true;
	}
}


void gui::TextureSelector::update(const sf::Vector2i& mousePosWindow, const float& dt)
{
	
	if (!this->hidden) {
		// Checking bounds active
		if (this->bounds.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
		{
			this->active = true;
		}
		else {
			this->active = false;
		}


		if (this->active)
		{
			this->mousePosGrid.x = (mousePosWindow.x - static_cast<int>(this->bounds.getPosition().x)) / static_cast<unsigned>(this->gridSize);
			this->mousePosGrid.y = (mousePosWindow.y - static_cast<int>(this->bounds.getPosition().y)) / static_cast<unsigned>(this->gridSize);

			this->selector.setPosition(
				this->bounds.getPosition().x + this->mousePosGrid.x * this->gridSize,
				this->bounds.getPosition().y + this->mousePosGrid.y * this->gridSize
			);

			// Update texture rectangle
			this->textureSelectorRect.left = static_cast<int>(this->selector.getPosition().x - this->bounds.getPosition().x);
			this->textureSelectorRect.top = static_cast<int>(this->selector.getPosition().y - this->bounds.getPosition().y);
		}
	}
}

void gui::TextureSelector::render(sf::RenderTarget& target)
{
	if (!this->hidden) {
		target.draw(this->bounds);
		target.draw(this->spriteSheet);

		if (this->active) {
			target.draw(this->selector);
		}
	}

	
}
