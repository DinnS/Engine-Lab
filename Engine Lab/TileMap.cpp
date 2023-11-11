#include "stdafx.h"
#include "TileMap.h"


void TileMap::clear()
{
	for (int x = 0; x < this->numberTiles.x; x++) {

		for (int y = 0; y < this->numberTiles.y; y++) {
			for (int z = 0; z < this->layers; z++) {
				for (int k = 0; k < this->map[x][y][z].size(); k++) {
					delete this->map[x][y][z][k];
					this->map[x][y][z][k] = nullptr;
				}
				
				this->map[x][y][z].clear();
			}
			this->map[x][y].clear();
		}
		this->map[x].clear();
	}

	this->map.clear();

	// std::cout << this->map.size() << std::endl;
}

// Constructors/Destructors



TileMap::TileMap(float gridSize, int width, int height, std::string texture_file_name)
{
	this->gridSizeF = gridSize;
	this->gridSizeI = static_cast<int>(this->gridSizeF);
	this->numberTiles.x = width;
	this->numberTiles.y = height;
	this->worldSize.x = static_cast<float>(width) * gridSizeF;
	this->worldSize.y = static_cast<float>(height) * gridSizeF;
	this->layers = 1;
	this->textureFileName = texture_file_name;

	this->fromX = 0;
	this->toX = 0;
	this->fromY = 0;
	this->toY = 0;
	this->layer = 0;

	this->map.resize(this->numberTiles.x, std::vector<std::vector <std::vector <Tile*> > >());
	for (int x = 0; x < this->numberTiles.x; x++) {

		for (int y = 0; y < this->numberTiles.y; y++) {
			this->map[x].resize(this->numberTiles.y, std::vector<std::vector<Tile*>>());
	
			for (int z = 0; z < this->layers; z++) {
				this->map[x][y].resize(this->layers, std::vector<Tile*>()); 
			}
		}
	}

	if (!this->tileTextureSheet.loadFromFile(this->textureFileName)) {
		std::cout << "ERROR::TILEMAP::FAILED TO LOAD TILE TEXTURE SHEET" << std::endl;
	}

	this->collisionBox.setSize(sf::Vector2f(this->gridSizeF, this->gridSizeF));
	this->collisionBox.setFillColor(sf::Color(255, 0, 0, 50));
	this->collisionBox.setOutlineColor(sf::Color::Red);
	this->collisionBox.setOutlineThickness(1.f);

}

TileMap::~TileMap()
{
	this->clear();

}



// Accessors


const sf::Texture* TileMap::getTileTextureSheet() const 
{
	return &this->tileTextureSheet;
}


// Functions

void TileMap::addTile(const int x, const int y, const int z, const sf::IntRect& texture_rect, const bool& collision, const short& type)
{
	/* Add tile */
	if (x < this->numberTiles.x && x >= 0 &&
		y < this->numberTiles.y && y >= 0 &&
		z < this->layers && z >= 0
		) {
		//if (this->map[x][y][z] == nullptr) { // Check to add
		/* Ready to tile */
		this->map[x][y][z].push_back(new Tile(x, y, this->gridSizeF, this->tileTextureSheet, texture_rect, collision, type));
		std::cout << "DEBUG: ADDED TILE!" << "\n";
		
	}
}

void TileMap::removeTile(const int x, const int y, const int z)
{
	/* Remove tile */
	if (x < this->numberTiles.x && x >= 0 &&
		y < this->numberTiles.y && y >= 0 &&
		z < this->layers && z >= 0
		) {
		if (!this->map[x][y][z].empty() ) { // Check to add
			/* Remove tile */
			delete this->map[x][y][z][this->map[x][y][z].size() -1];
			this->map[x][y][z].pop_back();
			std::cout << "DEBUG: REMOVED TILE!" << "\n";
		}
	}
}


void TileMap::saveToFile(const std::string file_name)
{
	/* 
	Saves the entire tilemap to a text-file 
	Basic:
	Size x , y
	gridSize
	layers
	texture file

	All tiles:
	gridPos x, y, layer 
	TextureRect x, y, collision, type
	*/

	std::ofstream out_file;
	out_file.open(file_name);

	if (out_file.is_open()) {
		out_file << this->numberTiles.x << " " << this->numberTiles.y << "\n"
			<< this->gridSizeI << "\n"
			<< this->layers << "\n"
			<< this->textureFileName << "\n";

		for (int x = 0; x < this->numberTiles.x; x++) {

			for (int y = 0; y < this->numberTiles.y; y++) {
				for (int z = 0; z < this->layers; z++) {
					if (!this->map[x][y][z].empty()) {
						for (int k = 0; k < this->map[x][y][z].size(); k++)
						{
							out_file << x << " " << y << " " << z << " " << k << this->map[x][y][z][k]->getAsString() << " ";  // CHECK !!!!!!!!
						}
						
					}
					
				}
			}
		}
	}
	else {
		std::cout << "ERROR::TILEMAP::COULD NOT SAVE TO FILE::FILENAME: " << file_name << std::endl;
	}

	out_file.close();


}

void TileMap::loadFromFile(const std::string file_name)
{

	std::ifstream in_file;
	in_file.open(file_name);

	if (in_file.is_open()) {
		sf::Vector2i size;
		int gridSize = 0;
		std::string textureFileName = "";
		int x = 0;
		int y = 0;
		int layers = 0;
		int textureRectX = 0;
		int textureRectY = 0;
		bool collision = false;
		short type = 0;

		// Basics
		in_file >> size.x >> size.y >> gridSize >> layers >> textureFileName;

		// Tiles
		this->gridSizeF = static_cast<float>(gridSize); ;
		this->gridSizeI = gridSize;
		this->numberTiles.x = size.x;
		this->numberTiles.y = size.y;
		this->layers = layers;
		this->textureFileName = textureFileName;

		this->clear();

		this->map.resize(this->numberTiles.x, std::vector<std::vector <std::vector <Tile* > > >());
		for (int x = 0; x < this->numberTiles.x; x++) {

			for (int y = 0; y < this->numberTiles.y; y++) {
				this->map[x].resize(this->numberTiles.y, std::vector <std::vector <Tile*>>());

				for (int z = 0; z < this->layers; z++) {
					this->map[x][y].resize(this->layers, std::vector<Tile*>());

				}
			}
		}

		if (!this->tileTextureSheet.loadFromFile(this->textureFileName)) {
			std::cout << "ERROR::TILEMAP::FAILED TO LOAD TILE TEXTURE SHEET" << std::endl;
		}

		// Load all tiles
		while (in_file >> x >> y >> layers >> textureRectX >> textureRectY >> collision >> type) {
			this->map[x][y][layers].push_back(new Tile(
				x, y, this->gridSizeF, 
				this->tileTextureSheet, 
				sf::IntRect(textureRectX, textureRectY, this->gridSizeI, this->gridSizeI), 
				collision, type
			));
		}
	}
	else {
		std::cout << "ERROR::TILEMAP::COULD NOT LOAD FROM FILE::FILENAME: " << file_name << std::endl;
	}

	in_file.close();
}



void TileMap::updateCollision(Entity* entity, const float& dt)
{
	// World Bounds
	if (entity->getPosition().x < 0.f) {
		entity->setPosition(0.f, entity->getPosition().y);
		entity->stopVelocity(true, false);
	}
	else if (entity->getPosition().x + entity->getGlobalBounds().width > this->worldSize.x) {
		entity->setPosition(this->worldSize.x - entity->getGlobalBounds().width, entity->getPosition().y);
		entity->stopVelocity(true, false);
	}

	if (entity->getPosition().y < 0.f) {
		entity->setPosition(entity->getPosition().x, 0.f);
		entity->stopVelocity(false, true);
	}
	else if (entity->getPosition().y + entity->getGlobalBounds().height > this->worldSize.y) {
		entity->setPosition(entity->getPosition().x, this->worldSize.y - entity->getGlobalBounds().height);
		entity->stopVelocity(false, true);
	}

	// Tiles
	this->layer = 0;
	this->fromX = entity->getGridPosition(this->gridSizeI).x - 1;
	if (this->fromX < 0) {
		this->fromX = 0;
	}
	else if (this->fromX > this->numberTiles.x) {
		this->fromX = this->numberTiles.x;
	}

	this->toX = entity->getGridPosition(this->gridSizeI).x + 3;
	if (this->toX < 0) {
		this->toX = 0;
	}
	else if (this->toX > this->numberTiles.x) {
		this->toX = this->numberTiles.x;
	}

	this->fromY = entity->getGridPosition(this->gridSizeI).y - 1;
	if (this->fromY < 0) {
		this->fromY = 0;
	}
	else if (this->fromY > this->numberTiles.y) {
		this->fromY = this->numberTiles.y;
	}

	this->toY = entity->getGridPosition(this->gridSizeI).y + 3;
	if (this->toY < 0) {
		this->toY = 0;
	}
	else if (this->toY > this->numberTiles.y) {
		this->toY = this->numberTiles.y;
	}

	

	for (int x = this->fromX; x < this->toX; x++) {

		for (int y = this->fromY; y < this->toY; y++) {

			for (int k = 0; k < this->map[x][y][this->layer].size(); k++)
			{

				sf::FloatRect playerBounds = entity->getGlobalBounds();
				sf::FloatRect wallBounds = this->map[x][y][this->layer][k]->getGlobalBounds();
				sf::FloatRect nextPositionBounds = entity->getNextPositionBounds(dt);

				if (
					this->map[x][y][this->layer][k]->getCollision() &&
					this->map[x][y][this->layer][k]->intersects(nextPositionBounds)
					) {
					std::cout << "COLLISION" << std::endl;
					// Bottom Collision
					if (playerBounds.top < wallBounds.top
						&& playerBounds.top + playerBounds.height < wallBounds.top + wallBounds.height
						&& playerBounds.left < wallBounds.left + wallBounds.width
						&& playerBounds.left + playerBounds.width > wallBounds.left
						) {
						entity->stopVelocity(false, true);
						entity->setPosition(playerBounds.left, wallBounds.top - playerBounds.height);
					}
					// Top Collision
					else if (playerBounds.top > wallBounds.top
						&& playerBounds.top + playerBounds.height > wallBounds.top + wallBounds.height
						&& playerBounds.left < wallBounds.left + wallBounds.width
						&& playerBounds.left + playerBounds.width > wallBounds.left
						) {
						entity->stopVelocity(false, true);
						entity->setPosition(playerBounds.left, wallBounds.top + wallBounds.height);
					}



					// Right Collision
					if (playerBounds.left < wallBounds.left
						&& playerBounds.left + playerBounds.width < wallBounds.left + wallBounds.width
						&& playerBounds.top < wallBounds.top + wallBounds.height
						&& playerBounds.top + playerBounds.height > wallBounds.top
						) {
						entity->stopVelocity(true, false);
						entity->setPosition(wallBounds.left - playerBounds.width, playerBounds.top);
					}
					// Left Collision
					else if (playerBounds.left > wallBounds.left
						&& playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width
						&& playerBounds.top < wallBounds.top + wallBounds.height
						&& playerBounds.top + playerBounds.height > wallBounds.top
						) {
						entity->stopVelocity(true, false);
						entity->setPosition(wallBounds.left + wallBounds.width, playerBounds.top);
					}
				}
			}

		}
	}
}

void TileMap::update()
{
}

void TileMap::render(sf::RenderTarget& target, const sf::Vector2i& gridPosition, const bool collisionDebug)
{

	if(collisionDebug) { // Vision collision debug
		this->layer = 0;
		this->fromX = gridPosition.x - 4;
		if (this->fromX < 0) {
			this->fromX = 0;
		}
		else if (this->fromX > this->numberTiles.x) {
			this->fromX = this->numberTiles.x;
		}

		this->toX = gridPosition.x + 5;
		if (this->toX < 0) {
			this->toX = 0;
		}
		else if (this->toX > this->numberTiles.x) {
			this->toX = this->numberTiles.x;
		}

		this->fromY = gridPosition.y - 3;
		if (this->fromY < 0) {
			this->fromY = 0;
		}
		else if (this->fromY > this->numberTiles.y) {
			this->fromY = this->numberTiles.y;
		}

		this->toY = gridPosition.y + 5;
		if (this->toY < 0) {
			this->toY = 0;
		}
		else if (this->toY > this->numberTiles.y) {
			this->toY = this->numberTiles.y;
		}

		for (int x = this->fromX; x < this->toX; x++) {

			for (int y = this->fromY; y < this->toY; y++) {
				for (int k = 0; k < this->map[x][y][this->layer].size(); k++)
				{
					this->map[x][y][this->layer][k]->render(target);
					if (this->map[x][y][this->layer][k]->getCollision()) {
						this->collisionBox.setPosition(this->map[x][y][this->layer][k]->getPosition());
						target.draw(this->collisionBox);
					}
				}
				

			}
		}
	}
	else {
		for (auto& x : this->map) {
			for (auto& y : x) {
				for (auto& z : y) {
					for (auto* k : z) {
						
						k->render(target);
						if (k->getCollision()) {
							this->collisionBox.setPosition(k->getPosition());
							target.draw(this->collisionBox);
						}

						
					}
				

				}
			}
		}
	}
}

