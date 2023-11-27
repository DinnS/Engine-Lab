#include "stdafx.h"
#include "AttributeComponent.h"

// Constructor / Destructor 
AttributeComponent::AttributeComponent(unsigned level)
{
	// Leveling 
	this->level = level;
	this->exp = 0;
	this->expNext = static_cast<unsigned>((50/3) * (pow(this->level + 1, 3) - 6 * pow(this->level + 1, 2) + ((this->level + 1)* 17) -12));
	this->attributePoints = 2;

	// Attributes
	int strength = 1;
	int agility = 1;
	int intelligence = 1;

	int vitality = 1;
	int dexterity = 1;

	this->updateLevel();
	this->updateStats(true);


}

AttributeComponent::~AttributeComponent()
{

}


// Functions

std::string AttributeComponent::debugPrint() const
{
	std::stringstream ss;

	ss << "Level : " << this->level << "\n" <<
		"Exp : " << this->exp << "\n" <<
		"ExpNext : " << this->expNext << "\n" <<
		"Attributes: " << this->attributePoints << "\n";


	return ss.str();
}

void AttributeComponent::gainExp(const unsigned exp)
{
	this->exp += exp;

	this->updateLevel();
}

void AttributeComponent::updateLevel()
{
	while (this->exp >= this->expNext) {
		++this->level;
		this->exp -= this->expNext;
		
		this->expNext = static_cast<unsigned>((50 / 3) * (pow(this->level, 3) - 6 * pow(this->level, 2) + (this->level * 17) - 12));
		++this->attributePoints;
	}
	
}

void AttributeComponent::updateStats(const bool reset)
{
	this->healthMax = this->vitality * 5 + this->vitality + this->strength / 2;
	this->damageMin = this->strength * 2 + this->strength / 4;
	this->damageMax = this->strength * 2 + this->strength / 2;
	
	this->accuracy = this->dexterity * 4 + this->dexterity / 2;
	this->defence = this->agility * 2 + this->agility / 4;
	this->luck = this->intelligence * 2 + this->intelligence / 5;


	if (reset) {
		this->health = this->healthMax;
	}
	
}

void AttributeComponent::update()
{
	
}
