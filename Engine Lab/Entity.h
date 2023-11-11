#pragma once

#include "HitboxComponent.h"
#include "MovementComponent.h"
#include "AnimationComponent.h"

class HitboxComponent;
class MovementComponent;
class AnimationComponent;

class Entity
{
private:
	void initVariables();
	
protected:
	
	sf::Sprite sprite;

	// Components
	HitboxComponent* hitboxComponent;
	MovementComponent* movementComponent;
	AnimationComponent* animationComponent;

	// Animation status checking
	std::string currentAnimationStatus;
	std::string newAnimationStatus;

	

public:
	Entity();
	virtual ~Entity();

	// Component functions
	
	void setTexture(sf::Texture& texture);
	void createHitboxComponent(sf::Sprite& sprite, float offset_x, float offset_y, float width, float height);
	void createMovementComponent(const float movementSpeed, const float acceleration, const float deceleration);
	void createAnimationComponent(std::map<std::string, sf::Texture*> texture_sheets);

	// Accessors
	virtual const sf::Vector2f& getPosition() const;
	virtual const sf::Vector2i getGridPosition(const int gridSizeI) const;
	virtual const sf::FloatRect getGlobalBounds() const;
	virtual const sf::FloatRect getNextPositionBounds(const float& dt) const;
	const float getHitboxComponentWidth() const;
	const float getHitboxComponentHeight() const;

	// Modifiers
	virtual void setPosition(const float x, const float y);

	// Functions
	virtual void move(const float x, const float y, const float& dt);
	virtual void stopVelocity(bool stopVelocityX = true, bool stopVelocityY = true);

	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget& target) = 0;
};
