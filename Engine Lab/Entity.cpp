#include "stdafx.h"
#include "Entity.h"

void Entity::initVariables()
{
	this->hitboxComponent = NULL;
	this->movementComponent = NULL;
	this->animationComponent = NULL;
	this->attributeComponent = NULL;

	this->currentAnimationStatus = "";
	this->newAnimationStatus = "";

}

Entity::Entity()
{
	this->initVariables();
	
}

Entity::~Entity()
{
	delete this->hitboxComponent;
	delete this->movementComponent;
	delete this->animationComponent;
	delete this->attributeComponent;
}


// Component functions

void Entity::setTexture(sf::Texture& texture)
{
	this->sprite.setTexture(texture);

}

void Entity::createHitboxComponent(sf::Sprite& sprite, 
	float offset_x, float offset_y,
	float width, float height
)
{
	this->hitboxComponent = new HitboxComponent(sprite, offset_x, offset_y, width, height);
}

void Entity::createMovementComponent(const float movementSpeed, const float acceleration, const float deceleration)
{
	this->movementComponent = new MovementComponent(this->sprite ,movementSpeed, acceleration, deceleration);
}


void Entity::createAnimationComponent(std::map<std::string, sf::Texture*> texture_sheets)
{
	this->animationComponent = new AnimationComponent(this->sprite, texture_sheets);
}

void Entity::createAttributeComponent(const unsigned level)
{
	this->attributeComponent = new AttributeComponent(level);
}




// Accessors


const sf::Vector2f& Entity::getPosition() const
{
	if (this->hitboxComponent) {
		return this->hitboxComponent->getPosition(); 
	} 
	return this->sprite.getPosition();
}

const sf::Vector2f Entity::getCenter() const
{
	if (this->hitboxComponent) {
		return this->hitboxComponent->getPosition() + sf::Vector2f(
			this->hitboxComponent->getGlobalBounds().width / 2.f,
			this->hitboxComponent->getGlobalBounds().height / 2.f
		);
	}

	return this->sprite.getPosition() + sf::Vector2f(
		this->sprite.getGlobalBounds().width / 2.f,
		this->sprite.getGlobalBounds().height / 2.f
	);
}

const sf::Vector2i Entity::getGridPosition(const int gridSizeI) const
{
	if (this->hitboxComponent) {
		return sf::Vector2i(
			static_cast<int>(this->hitboxComponent->getPosition().x) / gridSizeI,
			static_cast<int>(this->hitboxComponent->getPosition().y) / gridSizeI
		);
	}

	return sf::Vector2i(
		static_cast<int>(this->sprite.getPosition().x) / gridSizeI,
		static_cast<int>(this->sprite.getPosition().y) / gridSizeI
	);	

	
}

const sf::FloatRect Entity::getGlobalBounds() const
{
	if (this->hitboxComponent) {
		return this->hitboxComponent->getGlobalBounds();
	}
	return this->sprite.getGlobalBounds();
}

const sf::FloatRect Entity::getNextPositionBounds(const float& dt) const
{
	if (this->hitboxComponent && this->movementComponent) {
		return this->hitboxComponent->getNextPosition(this->movementComponent->getVelocity() * dt);
	}
	return sf::FloatRect(-1.f, -1.f, -1.f, -1.f);
}

const float Entity::getHitboxComponentWidth() const
{
	return hitboxComponent->getWidth();
}

const float Entity::getHitboxComponentHeight() const
{
	return hitboxComponent->getHeight();
}


// Modifiers

void Entity::setPosition(const float x, const float y)
{
	if (this->hitboxComponent) {
		this->hitboxComponent->setPosition(x,y);
	}
	else {
		this->sprite.setPosition(x, y);
	}
	

}


// Functions
	
void Entity::move(const float dir_x, const float dir_y, const float& dt)
{
	if (this->movementComponent) {
		this->movementComponent->move(dir_x, dir_y, dt);
	}


}

void Entity::stopVelocity(bool stopLeft, bool stopRight, bool stopTop, bool stopBottom)
{
	if (this->movementComponent) {
		this->movementComponent->stopVelocity(stopLeft, stopRight, stopTop, stopBottom);
	}
}

void Entity::update(const float& dt)
{

}

void Entity::render(sf::RenderTarget& target, sf::Shader* shader, const bool show_hitbox)
{
	
}
