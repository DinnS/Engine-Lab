#pragma once

#include "HitboxComponent.h"
#include "MovementComponent.h"
#include "AnimationComponent.h"
#include "AttributeComponent.h"

class HitboxComponent;
class MovementComponent;
class AnimationComponent;
class AttributeComponent;

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
	AttributeComponent* attributeComponent;

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
	void createAttributeComponent(const unsigned level);

	// Accessors
	virtual const sf::Vector2f& getPosition() const;
	virtual const sf::Vector2f getCenter() const;
	virtual const sf::Vector2i getGridPosition(const int gridSizeI) const;
	virtual const sf::FloatRect getGlobalBounds() const;
	virtual const sf::FloatRect getNextPositionBounds(const float& dt) const;
	const float getHitboxComponentWidth() const;
	const float getHitboxComponentHeight() const;

	// Modifiers
	virtual void setPosition(const float x, const float y);

	// Functions
	virtual void move(const float x, const float y, const float& dt);
	virtual void stopVelocity(bool stopLeft = true, bool stopRight = true, bool stopTop = true, bool stopBottom = true);

	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget& target, sf::Shader* shader, const bool show_hitbox) = 0;
};

