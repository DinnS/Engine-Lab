#pragma once
class AttributeComponent
{
private:
	// Leveling 
	unsigned level;
	unsigned exp;
	unsigned expNext;
	unsigned attributePoints;

	// Attributes
	int strength;
	int agility;
	int intelligence;

	int vitality;
	int dexterity;
	
	// Stats
	int health;
	int healthMax;
	int damageMin;
	int damageMax;
	int accuracy;
	// int dodge;
	int defence;
	int luck;

public:
	// Initializer functions 
	AttributeComponent(unsigned level);
	virtual ~AttributeComponent();


	// Accessor
	

	// Functions
	std::string debugPrint() const;

	void gainExp(const unsigned exp);

	void updateLevel();
	void updateStats(const bool reset);

	void update();


};

