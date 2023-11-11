#pragma once



class AnimationComponent
{
private:
	class Animation {
	public:
		// Variables
		sf::Sprite& sprite;
		std::map<std::string, sf::Texture*> textureSheets;
		std::string animationStatus;
		float animationTimer; 
		float timer;
		bool done;
		unsigned int width;
		unsigned int height;
		sf::IntRect startRect;
		sf::IntRect currentRect;
		sf::IntRect endRect;

		// Constructors
		Animation(sf::Sprite& sprite, std::map<std::string, sf::Texture*> texture_sheets, std::string animation_status,
			float animation_timer,
			int start_frame_x, int start_frame_y, int frames_x, int frames_y, unsigned int width, unsigned int height
		)
			: sprite(sprite), textureSheets(texture_sheets) , animationStatus(animation_status),
			animationTimer(animation_timer), width(width), height(height), timer(0.f), done(false)
		{
			
			this->startRect = sf::IntRect(start_frame_x * width, start_frame_y * height, width, height);
			this->currentRect = this->startRect;
			this->endRect = sf::IntRect(frames_x * width, frames_y * height, width, height);
			

			this->sprite.setTexture(*this->textureSheets["IDLE"], true);
			this->sprite.setTextureRect(this->startRect);
		}

		// Accessor
		const bool& isDone() const {
			return this->done;
		}

		// Functions
		const bool& play(const float& dt) {
			
			// Update timer
			this->done = false;
			this->timer += 100.f * dt;
			if (this->timer >= this->animationTimer) {
				// reset timer
				this->timer = 0.f;


				// Animate
				if (this->currentRect != this->endRect) {
					this->currentRect.left += this->width;
				}
				else  // Reset
				{  
					this->currentRect.left = this->startRect.left;
					this->done = true;
				}
				
				this->sprite.setTextureRect(this->currentRect);
			}
			return this->done;
		}

		const bool& play(const float& dt, float modifier_percentage) {
			// Update timer
			if (modifier_percentage < 0.7f) {
				modifier_percentage = 0.7f;
			}
			this->done = false;
			this->timer += modifier_percentage * 100.f * dt;
			if (this->timer >= this->animationTimer) {
				// reset timer
				this->timer = 0.f;


				// Animate
				if (this->currentRect != this->endRect) {
					this->currentRect.left += this->width;
				}
				else  // Reset
				{
					this->currentRect.left = this->startRect.left;
					this->done = true;
				}

				this->sprite.setTextureRect(this->currentRect);
			}
			return this->done;
		}
	

		void reset() {
			this->timer = this->animationTimer;
		
			this->currentRect = this->startRect;

		}
	};


	sf::Sprite& sprite;
	std::map<std::string, sf::Texture*> textureSheets;
	std::string animationStatus;
	std::string animation_side;
	std::map<std::string, std::map<std::string, Animation*>> animations;
	Animation* lastAnimation;
	Animation* priorityAnimation;
	

	

public:
	
	// Constructors / Destructors
	AnimationComponent(sf::Sprite& sprite, std::map<std::string, sf::Texture*> texture_sheets);
	virtual ~AnimationComponent();

	// Accessor
	std::string getAnimationStatus();
	const bool& isDone(const std::string animation_status, const std::string animation_side);


	// Functions
	void addAnimation(
		const std::string animation_status,
		const std::string animation_side,
		float animation_timer,
		int start_frame_x, int start_frame_y, int frames_x, int frames_y, unsigned int width, unsigned int height
	);
	void changeSpritesheet(std::string animation_status);

	const bool& play(const std::string animation_status, const std::string animation_side, const float& dt, const bool priority = false);
	const bool& play(const std::string animation_status, const std::string animation_side, const float& dt, const float& modifier, const float& modifier_max, const bool priority = false);
};

