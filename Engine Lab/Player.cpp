#include "stdafx.h"
#include "Player.h"

// Initializer functions

void Player::initVariables()
{
	this->isAttacking = false;
}

void Player::initComponents()
{
	
}


// Constructors / Destructors
Player::Player(float x,float y, std::map<std::string, sf::Texture*> texture_sheets)
{
	
	this->initVariables();

	this->setPosition(x, y);

	this->createHitboxComponent(this->sprite, 90.f, 100.f, 80.f, 100.f);
	this->createMovementComponent(500.f, 1000.f, 600.f);
	this->createAnimationComponent(texture_sheets);
	this->createAttributeComponent(1);

	this->animationComponent->addAnimation("IDLE", "LEFT", 10.f, 0, 0, 5, 0, 256, 256);
	this->animationComponent->addAnimation("IDLE", "RIGHT", 10.f, 0, 1, 5, 1, 256, 256);
	this->animationComponent->addAnimation("IDLE", "UP", 10.f, 0, 2, 5, 2, 256, 256);
	this->animationComponent->addAnimation("IDLE", "DOWN", 10.f, 0, 3, 5, 3, 256, 256);

	this->animationComponent->addAnimation("MOVING", "LEFT", 3.f, 0, 0, 11, 0, 256, 256);
	this->animationComponent->addAnimation("MOVING", "RIGHT", 3.f, 0, 1, 11, 1, 256, 256);
	this->animationComponent->addAnimation("MOVING", "UP", 3.f, 0, 2, 11, 2, 256, 256);
	this->animationComponent->addAnimation("MOVING", "DOWN", 3.f, 0, 3, 11, 3, 256, 256);

	this->animationComponent->addAnimation("ATTACK", "LEFT", 5.f, 0, 0, 10, 0, 256, 256);
	this->animationComponent->addAnimation("ATTACK", "RIGHT", 5.f, 0, 1, 10, 1, 256, 256);
	this->animationComponent->addAnimation("ATTACK", "UP", 5.f, 0, 2, 10, 2, 256, 256);
	this->animationComponent->addAnimation("ATTACK", "DOWN", 5.f, 0, 3, 10, 3, 256, 256);
	 

}

Player::~Player()
{

}


// Accessors
AttributeComponent* Player::getAttributeComponent()
{
	return this->attributeComponent;
}




// Functions

void Player::updateAttack()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		this->isAttacking = true;
	}

}

void Player::updateAnimation(const float& dt)
{
	// Change animation
	this->newAnimationStatus = "";

	if (this->isAttacking) {
		this->newAnimationStatus = "ATTACK";
		

		if (this->movementComponent->getLastState() == MOVING_LEFT) {
			if (this->animationComponent->play("ATTACK", "LEFT", dt, true)) {
				this->isAttacking = false;
			}
		}
		else if (this->movementComponent->getLastState() == MOVING_RIGHT) {
			if (this->animationComponent->play("ATTACK", "RIGHT", dt, true)) {
				this->isAttacking = false;
			}
		}
		else if (this->movementComponent->getLastState() == MOVING_UP) {
			if (this->animationComponent->play("ATTACK", "UP", dt, true)) {
				this->isAttacking = false;
			}
		}
		else if (this->movementComponent->getLastState() == MOVING_DOWN) {
			if (this->animationComponent->play("ATTACK", "DOWN", dt, true)) {
				this->isAttacking = false;
			}
		}
		else {
			if (this->animationComponent->play("ATTACK", "RIGHT", dt, true)) {
				this->isAttacking = false;
			}

		}
	}
	else if (this->movementComponent->getState(IDLE)) {
		this->newAnimationStatus = "IDLE";
		if (this->movementComponent->getLastState() == MOVING_LEFT) {
			this->animationComponent->play("IDLE", "LEFT", dt);
		}
		else if (this->movementComponent->getLastState() == MOVING_RIGHT) {
			this->animationComponent->play("IDLE", "RIGHT", dt);
		}
		else if (this->movementComponent->getLastState() == MOVING_UP) {
			this->animationComponent->play("IDLE", "UP", dt);
		}
		else if (this->movementComponent->getLastState() == MOVING_DOWN) {
			this->animationComponent->play("IDLE", "DOWN", dt);
		}
		else {
			this->animationComponent->play("IDLE", "DOWN", dt);

		}
	}
	else if (this->movementComponent->getState(MOVING)) {
		this->newAnimationStatus = "MOVING";
		if (this->movementComponent->getState(MOVING_LEFT)) {
			this->animationComponent->play(
				"MOVING", "LEFT",
				dt,
				this->movementComponent->getVelocity().x, this->movementComponent->getMovementSpeed()
			);

		}
		else if (this->movementComponent->getState(MOVING_RIGHT)) {

			this->animationComponent->play(
				"MOVING", "RIGHT",
				dt,
				this->movementComponent->getVelocity().x, this->movementComponent->getMovementSpeed()
			);

		}
		else if (this->movementComponent->getState(MOVING_UP)) {
			this->animationComponent->play(
				"MOVING", "UP",
				dt,
				this->movementComponent->getVelocity().y, this->movementComponent->getMovementSpeed()
			);

		}
		else if (this->movementComponent->getState(MOVING_DOWN)) {
			this->animationComponent->play(
				"MOVING", "DOWN",
				dt,
				this->movementComponent->getVelocity().y, this->movementComponent->getMovementSpeed()
			);

		}
	}


	if (this->newAnimationStatus != this->currentAnimationStatus) {
		this->currentAnimationStatus = newAnimationStatus;
		this->animationComponent->changeSpritesheet(this->currentAnimationStatus);
	}
}


void Player::update(const float& dt)
{
	this->movementComponent->update(dt);

	this->updateAttack();

	this->updateAnimation(dt);


	// Hitbox component update
	
	this->hitboxComponent->update();
	
	
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);

	this->hitboxComponent->render(target);
	
}
