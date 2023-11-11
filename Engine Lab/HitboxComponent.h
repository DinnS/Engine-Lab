#pragma once


class HitboxComponent
{
private:
	sf::Sprite& sprite;
	sf::RectangleShape hitbox;
	sf::FloatRect nextPosition;

	float offsetX;
	float offsetY;
	float width;
	float height;

public:

	// Construction / Destruction
	HitboxComponent(sf::Sprite& sprite, float offset_x, float offset_y, float width, float height);
	virtual ~HitboxComponent();

	// Accessors
	const sf::Vector2f& getPosition();
	const sf::FloatRect getGlobalBounds() const;
	const sf::FloatRect& getNextPosition(const sf::Vector2f& velocity);
	const float& getWidth() const;
	const float& getHeight() const;

	// Modifier
	void setPosition(const sf::Vector2f& position);
	void setPosition(const float x, const float y);

	// Functions
	bool intersects(const sf::FloatRect& frect);

	void update();
	void render(sf::RenderTarget& target);
};

