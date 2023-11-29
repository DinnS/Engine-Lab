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
    // Constructor / Destructor
    Player(float x, float y, std::map<std::string, sf::Texture*> texture_sheets);
    virtual ~Player();

    // Accessors
    AttributeComponent* getAttributeComponent();

    // Functions
    void gainHealth(const int health);
    void loseHealth(const int health);

    void gainEXP(const int exp);
    void loseEXP(const int exp);
   

    void updateAttack();
    void updateAnimation(const float& dt);
    void update(const float& dt);
    void render(sf::RenderTarget& target);

};

