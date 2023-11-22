#include "stdafx.h"
#include "AnimationComponent.h"

// Constructors / Destructors
AnimationComponent::AnimationComponent(sf::Sprite& sprite, std::map<std::string, sf::Texture*> texture_sheets)
	: sprite(sprite), textureSheets(texture_sheets),lastAnimation(NULL), priorityAnimation(NULL)
{

}
	
AnimationComponent::~AnimationComponent()
{
	for (auto& i : this->animations) {
		for (auto& j : i.second) {
			delete j.second;
		}
	}

	for (auto& i : this->textureSheets) {
		delete i.second;
	}
	
}

// Accessor

std::string AnimationComponent::getAnimationStatus()
{
	return this->animationStatus;
}

const bool& AnimationComponent::isDone(const std::string animation_status, const std::string animation_side) 
{
	return this->animations[animation_status][animation_side]->isDone();
}

// Functions

void AnimationComponent::addAnimation(
	const std::string animation_status,
	const std::string animation_side,
	float animation_timer,
	int start_frame_x, int start_frame_y, int frames_x, int frames_y, unsigned int width, unsigned int height
)
{
	this->animations[animation_status][animation_side] = new Animation(
		this->sprite, this->textureSheets, animation_status,
		animation_timer, start_frame_x,
		start_frame_y, frames_x, frames_y, width, height
	);
		
}

void AnimationComponent::changeSpritesheet(std::string animation_status)
{
	if (this->animationStatus != animation_status) {
		this->animationStatus = animation_status;	
		this->sprite.setTexture(*this->textureSheets[this->animationStatus], false);

		//this->sprite.setTextureRect(sf::IntRect(0, 0, 256, 256));
		this->animations[this->animationStatus][this->animation_side]->reset();
	}
	

}

const bool& AnimationComponent::play(
	const std::string animation_status,
	const std::string animation_side, 
	const float& dt, const bool priority
)
{
	this->animation_side = animation_side;

	if (this->priorityAnimation) {  // If there play priority animation
		if (this->priorityAnimation == this->animations[animation_status][animation_side]) {

			// Animation reset
			if (this->lastAnimation != this->animations[animation_status][animation_side]) {
				if (this->lastAnimation == NULL) {
					this->lastAnimation = this->animations[animation_status][animation_side];
				}
				else {
					this->lastAnimation->reset();
					this->lastAnimation = this->animations[animation_status][animation_side];
				}

			}

			// Play

			if (this->animations[animation_status][animation_side]->play(dt)) {
				this->priorityAnimation = NULL;
			}
		}
	}
	else { // Play animation if dont have priority animations
		if (priority) {
			this->priorityAnimation = this->animations[animation_status][animation_side];
		}

		// Animation reset
		if (this->lastAnimation != this->animations[animation_status][animation_side]) {
			if (this->lastAnimation == NULL) {
				this->lastAnimation = this->animations[animation_status][animation_side];
			}
			else {
				this->lastAnimation->reset();
				this->lastAnimation = this->animations[animation_status][animation_side];
			}

		}

		// Play

		this->animations[animation_status][animation_side]->play(dt);
	}

	return this->animations[animation_status][animation_side]->isDone();
}

const bool& AnimationComponent::play(
	const std::string animation_status,
	const std::string animation_side,
	const float& dt, const float& modifier, const float& modifier_max, 
	const bool priority
)
{
	this->animation_side = animation_side;

	if (this->priorityAnimation) {  // If there play priority animation
		if (this->priorityAnimation == this->animations[animation_status][animation_side]) {

			// Animation reset
			if (this->lastAnimation != this->animations[animation_status][animation_side]) {
				if (this->lastAnimation == NULL) {
					this->lastAnimation = this->animations[animation_status][animation_side];
				}
				else {
					this->lastAnimation->reset();
					this->lastAnimation = this->animations[animation_status][animation_side];
				}

			}

			// Play
			if (this->animations[animation_status][animation_side]->play(dt, abs(modifier / modifier_max))) {
				this->priorityAnimation = NULL;
			}
			
		}
	}
	else { // Play animation if dont have priority animations

		if (priority) {
			this->priorityAnimation = this->animations[animation_status][animation_side];
		}

		// Animation reset
		if (this->lastAnimation != this->animations[animation_status][animation_side]) {
			if (this->lastAnimation == NULL) {
				this->lastAnimation = this->animations[animation_status][animation_side];
			}
			else {
				this->lastAnimation->reset();
				this->lastAnimation = this->animations[animation_status][animation_side];
			}

		}

		// Play

		this->animations[animation_status][animation_side]->play(dt, abs(modifier / modifier_max));
	}

	return this->animations[animation_status][animation_side]->isDone();
}
