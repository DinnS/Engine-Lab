#include "stdafx.h"
#include "MovementComponent.h"

// Constructors / Destructors
MovementComponent::MovementComponent(sf::Sprite &sprite, float movementSpeed, float acceleration, float deceleration) 
	: sprite(sprite), movementSpeed(movementSpeed), acceleration(acceleration) ,deceleration(deceleration)
{

}

MovementComponent::~MovementComponent()
{

}


// Accesors
const float& MovementComponent::getMovementSpeed() const
{
	return this->movementSpeed;
}

const sf::Vector2f& MovementComponent::getVelocity() const
{
	return this->velocity;
}

const bool MovementComponent::getState(const short unsigned int state)
{
	switch (state) {
	case IDLE:
		if (this->velocity.x == 0.f && this->velocity.y == 0.f) {
			return true;
		}
		break;

	case MOVING:
		if (!this->velocity.x == 0.f || !this->velocity.y == 0.f) {
			return true;
		}

		break;

	case MOVING_LEFT:
		if (this->velocity.x < 0.f) {
			this->lastState = state;
			return true;
		}


		break;

	case MOVING_RIGHT:
		if (this->velocity.x > 0.f) {
			this->lastState = state;
			return true;
		}
		break;

	case MOVING_UP:
		if (this->velocity.y < 0.f) {
			this->lastState = state;
			return true;
		}
		break;

	case MOVING_DOWN:
		if (this->velocity.y > 0.f) {
			this->lastState = state;
			return true;
		}
		break;
	}


	return false;

}

const short unsigned int MovementComponent::getLastState()
{
	return lastState;
}



// Functions


void MovementComponent::stopVelocity(bool stopLeft, bool stopRight, bool stopTop, bool stopBottom)
{
	/* reset the velocity to 0.*/
	if (stopLeft && this->velocity.x < 0.f) {
		this->velocity.x = 0.f;
	}
	
	if (stopRight && this->velocity.x > 0.f) {
		this->velocity.x = 0.f;
	}

	if (stopTop && this->velocity.y < 0.f) {
		this->velocity.y = 0.f;
	}

	if (stopBottom && this->velocity.y > 0.f) {
		this->velocity.y = 0.f;
	}
	
	/*if (stopVelocityY) {
		this->velocity.y = 0.f;
	}*/
	
}

void MovementComponent::move(const float dir_x, const float dir_y, const float &dt)
{

	this->velocity.x += this->acceleration * dir_x * dt;
	this->velocity.y += this->acceleration * dir_y * dt;
} 


void MovementComponent::update(const float& dt)
{
	/* Decelerates the movement and check the maximum moveSpeed */
	// X
	if (this->velocity.x > 0.f) {  // Check for right
		// Movement speed check positive
		if (this->velocity.x > this->movementSpeed) {
			this->velocity.x = this->movementSpeed;
		}

		// Deceleration x positive
		this->velocity.x -= deceleration * dt;
		if (this->velocity.x < 0.f) {
			this->velocity.x = 0.f;
		}

		
	}
	else if (this->velocity.x < 0.f) { // Check for left

		// Movement speed check negative
		if (this->velocity.x < -this->movementSpeed) {
			this->velocity.x = -this->movementSpeed;
		}

		// Deceleration x negative
		this->velocity.x += deceleration * dt;
		if (this->velocity.x > 0.f) {
			this->velocity.x = 0.f;
		}

	
	}

	// Y
	if (this->velocity.y > 0.f) {  // Check for up
		// Movement speed check positive
		if (this->velocity.y > this->movementSpeed) {
			this->velocity.y = this->movementSpeed;
		}



		// Deceleration y positive
		this->velocity.y -= deceleration * dt;
		if (this->velocity.y < 0.f) {
			this->velocity.y = 0.f;
		}
	}
	else if (this->velocity.y < 0.f) { // Check for down

		// Movement speed check negative
		if (this->velocity.y < -this->movementSpeed) {
			this->velocity.y = -this->movementSpeed;
		}


		// Deceleration y negative
		this->velocity.y += deceleration * dt;
		if (this->velocity.y > 0.f) {
			this->velocity.y = 0.f;
		}
	}

	// Final move
	this->sprite.move(this->velocity * dt);
}
