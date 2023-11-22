#pragma once

#include "Tile.h"
#include "Entity.h"

class Tile;
class Entity;

class TileMap
{
private:
	void clear();

	float gridSizeF;
	int gridSizeI;
	int layers;
	sf::Vector2i numberTiles;
	sf::Vector2f worldSize;
	std::vector<std::vector<std::vector< std::vector<Tile*>>>> map;
	std::stack<Tile*> defferedRenderStack;
	std::string textureFileName;
	sf::Texture tileTextureSheet;
	sf::RectangleShape collisionBox;

	// Culling
	int fromX;
	int toX;
	int fromY;
	int toY;
	int layer;

public:
	// Constructors/Destructors
	TileMap(float gridSize, int width, int height, std::string texture_file_name);
	virtual ~TileMap();

	// Accessors
	const sf::Texture* getTileTextureSheet() const;
	const int getLayerSize(const int x, const int y, const int layer) const;

	// Functions
	void addTile(const int x, const int y, const int z, const sf::IntRect& texture_rect, const bool& collision, const short& type);
	void removeTile(const int x, const int y, const int z);


	void saveToFile(const std::string file_name);
	void loadFromFile(const std::string file_name);

	void updateCollision(Entity* entity, const float& dt);
	
	void update();
	void render(sf::RenderTarget& target, const sf::Vector2i& gridPosition, const bool collisionDebug = false);
	void renderDeferred(sf::RenderTarget& target);
	

	
};

