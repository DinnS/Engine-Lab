#pragma once
#include "Entity.h"

class Entity;

class Player : public Entity
{
private:
    // Variables
    bool isAttacking;

    // Iniatilizer functions
    void initVariables();
    void initComponents();

public:
    Player(float x, float y, std::map<std::string, sf::Texture*> texture_sheets);
    virtual ~Player();

    // Functions
    void updateAttack();
    void updateAnimation(const float& dt);
    void update(const float& dt);
    void render(sf::RenderTarget& target);

};
