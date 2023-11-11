#pragma once


enum movement_states {
	IDLE = 0,
	MOVING,
	MOVING_LEFT,
	MOVING_RIGHT,
	MOVING_UP,
	MOVING_DOWN
};

class MovementComponent
{
private:
	sf::Sprite& sprite;

	float movementSpeed;
	float acceleration;
	float deceleration;

	sf::Vector2f velocity;

	short unsigned int lastState;


	// Initialize functions

public:

	// Constructors / Destructors
	MovementComponent(sf::Sprite& sprite, float movementSpeed, float acceleration, float deceleration);
	virtual ~MovementComponent();

	// Accessors
	const float& getMovementSpeed() const;
	const sf::Vector2f& getVelocity() const; 

	const bool getState(const short unsigned int state);
	const short unsigned int getLastState();

	// Functions
	void stopVelocity(bool stopX = true, bool stopY = true);


	void move(const float dir_x, const float dir_y, const float& dt);
	void update(const float &dt);
};

